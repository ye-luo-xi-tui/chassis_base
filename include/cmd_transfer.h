//
// Created by yezi on 2021/7/27.
//

#ifndef SRC_CHASSIS_BASE_INCLUDE_CMD_TRANSFER_H_
#define SRC_CHASSIS_BASE_INCLUDE_CMD_TRANSFER_H_

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

namespace chassis_base {

class cmd_transfer {
 public:
  explicit cmd_transfer(ros::NodeHandle root_nh);
 private:
  bool IS_STOP_{false};
  geometry_msgs::Twist twist_{};
  int stop_button{};
  int start_button{};
  ros::Publisher cmd_pub_{};
  ros::Subscriber joy_sub_{};
  ros::Subscriber cmd_sub_{};
  void joy_callback(const sensor_msgs::Joy::ConstPtr &joy);
  void cmd_callback(const geometry_msgs::Twist::ConstPtr &cmd);
  void update();
};
}

#endif //SRC_CHASSIS_BASE_INCLUDE_CMD_TRANSFER_H_
