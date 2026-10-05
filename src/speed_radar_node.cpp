#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

class SpeedRadarNode : public rclcpp::Node
{
public:
  SpeedRadarNode()
  : Node("speed_radar_node")
  {
    // Feliratkozunk a 'car_velocity' topicra
    subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "car_velocity", 10, std::bind(&SpeedRadarNode::topic_callback, this, std::placeholders::_1));
  }

private:
  void topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const
  {
    // Ellenőrizzük a lineáris x sebességet
    if (msg->linear.x > 50.0) {
      RCLCPP_WARN(this->get_logger(), "!!! BUNTETES !!! Gyorshajtas: '%.2f' km/h", msg->linear.x);
    } else {
      RCLCPP_INFO(this->get_logger(), "Szabalyszeru kozlekedes: '%.2f' km/h", msg->linear.x);
    }
  }
  
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpeedRadarNode>());
  rclcpp::shutdown();
  return 0;
}