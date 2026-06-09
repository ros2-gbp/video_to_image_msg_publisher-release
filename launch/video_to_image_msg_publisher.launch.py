#!/usr/bin/env python3
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node

from launch import LaunchDescription


def generate_launch_description():
    package_share_directory = get_package_share_directory(
        "video_to_image_msg_publisher"
    )
    config_file_path = (
        f"{package_share_directory}/config/video_to_image_msg_publisher.yaml"
    )
    return LaunchDescription(
        [
            Node(
                package="video_to_image_msg_publisher",
                executable="video_to_image_msg_publisher",
                name="video_to_image_msg_publisher",
                parameters=[config_file_path],
            ),
        ]
    )
