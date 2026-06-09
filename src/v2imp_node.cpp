#include "video_to_image_msg_publisher/v2imp.hpp"

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VideoPublisherNode>());
  rclcpp::shutdown();
  return 0;
}
