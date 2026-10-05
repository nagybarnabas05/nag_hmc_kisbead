#include <chrono>
#include <memory>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class CarSimulatorNode : public rclcpp::Node
{
public:
  CarSimulatorNode()
  : Node("car_simulator_node")
  {
    // Létrehozunk egy publishert a 'car_velocity' topicra
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("car_velocity", 10);
    
    // Beállítunk egy timert, ami másodpercenként meghívja a timer_callback függvényt
    timer_ = this->create_wall_timer(
      1000ms, std::bind(&CarSimulatorNode::timer_callback, this));
    
    // Véletlenszám-generátor inicializálása (sebesség 30 és 80 km/h között)
    gen_.seed(rd_());
  }

private:
  void timer_callback()
  {
    auto message = geometry_msgs::msg::Twist();
    
    // Véletlen sebesség generálása 30.0 és 80.0 között
    std::uniform_real_distribution<double> dis(30.0, 80.0);
    
    // A Twist üzenet lineáris x mezőjébe írjuk a sebességet
    message.linear.x = dis(gen_);

    RCLCPP_INFO(this->get_logger(), "Auto sebessege: '%.2f' km/h", message.linear.x);
    publisher_->publish(message);
  }
  
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  std::random_device rd_;
  std::mt19937 gen_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CarSimulatorNode>());
  rclcpp::shutdown();
  return 0;
}