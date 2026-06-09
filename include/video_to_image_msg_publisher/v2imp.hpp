#pragma once

#ifdef __has_include
#if __has_include(<cv_bridge/cv_bridge.hpp>)
#include <cv_bridge/cv_bridge.hpp>
#else
#include <cv_bridge/cv_bridge.h>
#endif
#else
#include <cv_bridge/cv_bridge.h>
#endif
#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <string>

class VideoPublisherNode : public rclcpp::Node
{
public:
  VideoPublisherNode();
  ~VideoPublisherNode() = default;

private:
  void publish_image();

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string image_encoding_;
  std::string image_frame_;
  cv::VideoCapture cap_;
  bool loop_;
};
