#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include "cv_bridge/cv_bridge.h"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/path.hpp"

#include "../../../../include/System.h"

using namespace std;

class VideoStreamGrabber : public rclcpp::Node
{
public:
    VideoStreamGrabber(ORB_SLAM3::System* pSLAM, const std::string& stream_url)
        : Node("video_stream_grabber"), mpSLAM(pSLAM), stream_url_(stream_url)
    {
        // Set up OpenCV to capture from the video stream URL
        cap_.open(stream_url_);
        if (!cap_.isOpened())
        {
            RCLCPP_ERROR(this->get_logger(), "Error: Could not open video stream from URL: %s", stream_url_.c_str());
            rclcpp::shutdown();
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "Successfully opened video stream from URL: %s", stream_url_.c_str());
        }

        // Start the timer to grab frames
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(30),
            std::bind(&VideoStreamGrabber::GrabFrame, this));

        // Initialize the pose publisher
        pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("pose", 10);

        // Initialize the path publisher
        path_publisher_ = this->create_publisher<nav_msgs::msg::Path>("path", 10);

        // Set the frame ID for the path
        path_.header.frame_id = "map";
    }

private:
    void GrabFrame()
    {
        cv::Mat frame;
        if (!cap_.read(frame))
        {
            RCLCPP_ERROR(this->get_logger(), "Error: Failed to capture frame from video stream");
            return;
        }

        // Convert cv::Mat to ROS2 sensor_msgs::msg::Image
        auto now = this->now();
        std_msgs::msg::Header header;
        header.stamp = now;
        header.frame_id = "camera";

        cv_bridge::CvImage cv_image(header, "bgr8", frame);
        sensor_msgs::msg::Image::SharedPtr msg = cv_image.toImageMsg();

        // Pass the frame to ORB-SLAM3
        Sophus::SE3f pose = mpSLAM->TrackMonocular(cv_image.image, now.seconds());

        // Publish the pose information
        geometry_msgs::msg::PoseStamped pose_msg;
        pose_msg.header.stamp = now;
        pose_msg.header.frame_id = "map";
        pose_msg.pose.position.x = pose.translation().x();
        pose_msg.pose.position.y = pose.translation().y();
        pose_msg.pose.position.z = pose.translation().z();
        pose_msg.pose.orientation.x = pose.unit_quaternion().x();
        pose_msg.pose.orientation.y = pose.unit_quaternion().y();
        pose_msg.pose.orientation.z = pose.unit_quaternion().z();
        pose_msg.pose.orientation.w = pose.unit_quaternion().w();

        pose_publisher_->publish(pose_msg);

        // Append the pose to the path
        path_.header.stamp = now;
        path_.poses.push_back(pose_msg);

        // Publish the path
        path_publisher_->publish(path_);
    }

    ORB_SLAM3::System* mpSLAM;
    std::string stream_url_;
    cv::VideoCapture cap_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_;

    // Path publisher and path message
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_publisher_;
    nav_msgs::msg::Path path_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    if (argc != 4)
    {
        std::cerr << std::endl << "Usage: ros2 run orb_slam3_ros2 MonoNode path_to_vocabulary path_to_settings stream_url" << std::endl;
        return 1;
    }

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);

    std::string stream_url = argv[3];

    auto node = std::make_shared<VideoStreamGrabber>(&SLAM, stream_url);

    rclcpp::spin(node);

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    rclcpp::shutdown();

    return 0;
}