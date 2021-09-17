//
// Created by yezi on 2021/9/7.
//

#include <ros/ros.h>
#include <gpio_manager.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "Lamp_node");
  ros::NodeHandle nh;
  GpioMangager gpio_mangager;
  gpio_mangager.addOutIo(248);
  gpio_mangager.addOutIo(249);
  gpio_mangager.writeOutput(248, true);
  while (ros::ok()) {
  }
}