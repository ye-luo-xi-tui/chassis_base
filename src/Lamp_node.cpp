//
// Created by yezi on 2021/9/7.
//

#include <ros/ros.h>
#include <gpio_manager.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "Lamp_node");
  ros::NodeHandle nh;

  while (ros::ok())
    return 0;
}