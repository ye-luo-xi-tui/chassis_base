//
// Created by yezi on 2021/7/27.
//
#include <cmd_transfer.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "cmd_transfer");
  ros::NodeHandle nh;
  chassis_base::cmd_transfer cmd_transfer(nh);
  ros::spin();
}