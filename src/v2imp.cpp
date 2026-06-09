#include "video_to_image_msg_publisher/v2imp.hpp"

VideoPublisherNode::VideoPublisherNode() : Node("video_to_image_msg_publisher")
{
  declare_parameter<std::string>("video_file", "");
  declare_parameter<std::string>("image_topic", "camera_image");
  declare_parameter<std::string>("image_frame", "camera_link");
  declare_parameter<std::string>("image_encoding", "bgr8");
  declare_parameter<int>("fps", 30);
  declare_parameter<bool>("loop", true);

  auto video_file = get_parameter("video_file").as_string();
  auto image_topic = get_parameter("image_topic").as_string();
  image_frame_ = get_parameter("image_frame").as_string();
  image_encoding_ = get_parameter("image_encoding").as_string();
  auto fps = get_parameter("fps").as_int();
  loop_ = get_parameter("loop").as_bool();

  if (video_file.empty())
  {
    RCLCPP_ERROR(get_logger(), "video_file parameter not set");
    return;
  }

  cap_.open(video_file);
  if (!cap_.isOpened())
  {
    RCLCPP_ERROR(get_logger(), "Failed to open video file: %s", video_file.c_str());
    return;
  }

  publisher_ = create_publisher<sensor_msgs::msg::Image>(image_topic, 10);

  int interval_ms = static_cast<int>(1000.0 / fps);

  timer_ =
      create_wall_timer(std::chrono::milliseconds(interval_ms), std::bind(&VideoPublisherNode::publish_image, this));

  RCLCPP_INFO(get_logger(), "Video publisher initialized. Publishing at %ld FPS", fps);
}

void VideoPublisherNode::publish_image()
{
  cv::Mat frame;

  if (!cap_.read(frame))
  {
    RCLCPP_WARN(get_logger(), "End of video reached or failed to read frame");
    if (not loop_)
    {
      timer_->cancel();
    }
    cap_.set(cv::CAP_PROP_POS_FRAMES, 0);
    return;
  }

  cv_bridge::CvImage cv_image;
  cv_image.header.stamp = now();
  cv_image.header.frame_id = image_frame_;
  cv_image.encoding = image_encoding_;
  cv_image.image = frame;

  sensor_msgs::msg::Image::SharedPtr msg = cv_image.toImageMsg();

  publisher_->publish(*msg);
}
