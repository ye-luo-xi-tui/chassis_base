//
// Created by yezi on 2021/9/9.
//

#ifndef SRC_CHASSIS_BASE_INCLUDE_GPIO_MANAGER_H_
#define SRC_CHASSIS_BASE_INCLUDE_GPIO_MANAGER_H_

#include <XmlRpcValue.h>
#include <fcntl.h>
#include <map>
#include <poll.h>
#include <ros/ros.h>
#include <string>

struct gpio_data {
  int pin;
  int value;
};

struct GpioDataStamp {
  gpio_data data{};
  ros::Time stamp;
};

class GpioMangager {
public:
  explicit GpioMangager();
  ~GpioMangager();
  bool init(ros::NodeHandle module_nh);
  void writeOutput(int pin, bool IS_HIGH);
  void readInput(std::vector<GpioDataStamp> &gpio_data_stamp_vector);

private:
  void addInIo(int pin);
  void addOutIo(int pin);
  void ioDirectionSet(std::string pin, bool IS_OUT);
  std::map<int, int> mapOutputIo_;
  std::map<int, int> mapInputIo_;
  std::string pin_;
  struct pollfd fds[20]{};
};


#endif // SRC_CHASSIS_BASE_INCLUDE_GPIO_MANAGER_H_
