//
// Created by yezi on 2021/7/27.
//

#include <cmd_transfer.h>

namespace chassis_base {
cmd_transfer::cmd_transfer(ros::NodeHandle root_nh) {
  root_nh.param("start_button", start_button_, 0);
  root_nh.param("stop_button", stop_button_, 1);
  root_nh.param("auto_button", auto_button_, 2);
  root_nh.param("stick_button", stick_button_, 3);
  joy_sub_ = root_nh.subscribe<sensor_msgs::Joy>("joy", 10, &cmd_transfer::joy_callback, this);
  joy_cmd_sub_ =
      root_nh.subscribe<geometry_msgs::Twist>("joy_cmd_vel", 10, &cmd_transfer::joy_cmd_callback, this);
  computer_cmd_sub_ =
      root_nh.subscribe<geometry_msgs::Twist>("computer_cmd_vel",
                                              10,
                                              &cmd_transfer::computer_cmd_callback,
                                              this);
  cmd_pub_ = root_nh.advertise<geometry_msgs::Twist>("controllers/chassis_base_controller/cmd_vel", 1);
}
void cmd_transfer::joy_callback(const sensor_msgs::Joy::ConstPtr &joy) {
  if (joy->buttons[stop_button_])
    IS_STOP_ = true;
  if (joy->buttons[start_button_])
    IS_STOP_ = false;
  if (joy->buttons[auto_button_])
    IS_STICK_ = false;
  if (joy->buttons[stick_button_])
    IS_STICK_ = true;
  update();
}

void cmd_transfer::joy_cmd_callback(const geometry_msgs::Twist::ConstPtr &cmd) {
  if (IS_STICK_) {
    twist_.linear.x = cmd->linear.x;
    twist_.angular.z = cmd->angular.z;
    update();
  }
}

void cmd_transfer::computer_cmd_callback(const geometry_msgs::Twist::ConstPtr &cmd) {
  if (!IS_STICK_) {
    twist_.linear.x = cmd->linear.x;
    twist_.angular.z = cmd->angular.z;
    update();
  }
}

void cmd_transfer::update() {
  if (IS_STOP_) {
    twist_.linear.x = 0.0;
    twist_.angular.z = 0.0;
  }
  cmd_pub_.publish(twist_);
}
}