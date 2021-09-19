//
// Created by yezi on 2021/9/7.
//

#include <gpio_manager.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

// void joy_callback(const sensor_msgs::Joy::ConstPtr &joy) { if
// (joy->buttons[]) }

int main(int argc, char **argv) {
  ros::init(argc, argv, "Lamp_node");
  ros::NodeHandle nh("/lamp_node");
  GpioMangager gpio_mangager;
  gpio_mangager.init(nh);
  //  ros::Subscriber joy_sub =
  //      nh.subscribe<sensor_msgs::Joy>("joy", 10, &joy_callback);

  ros::spin();
}
