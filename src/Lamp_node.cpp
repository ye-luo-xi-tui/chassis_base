//
// Created by yezi on 2021/9/7.
//

#include <gpio_manager.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

GpioMangager gpio_manager;
bool Front_lamp_is_light = false;
bool Back_lamp_is_light = false;

void joy_callback(const sensor_msgs::Joy::ConstPtr &joy) {
  if (joy->buttons[4]) {
    if (Front_lamp_is_light) {
      gpio_manager.writeOutput(248, false);
      Front_lamp_is_light = false;
    } else {
      gpio_manager.writeOutput(248, true);
      Front_lamp_is_light = true;
    }
  }
  if (joy->buttons[5]) {
    if (Back_lamp_is_light) {
      gpio_manager.writeOutput(249, false);
      Back_lamp_is_light = false;
    } else {
      gpio_manager.writeOutput(249, true);
      Back_lamp_is_light = true;
    }
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "Lamp_node");
  ros::NodeHandle nh("/lamp_node");
  ros::NodeHandle root_nh;
  gpio_manager.init(nh);
  ros::Subscriber joy_sub = root_nh.subscribe("joy", 10, joy_callback);
  ros::spin();
}
