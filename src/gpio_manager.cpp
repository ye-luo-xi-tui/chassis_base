//
// Created by yezi on 2021/9/9.
//

#include <gpio_manager.h>
#include <ros/ros.h>

GpioMangager::GpioMangager() {
  mapOutputIo_.clear();
  mapInputIo_.clear();
}

GpioMangager::~GpioMangager() {
  if(!mapOutputIo_.empty()){
    for (auto i:mapOutputIo_){
      ioUnExport(i.first);
      close(i.second);
    }
  }
  if(!mapInputIo_.empty()){
    for(auto i:mapInputIo_)
    {
      ioUnExport(i.first);
      close(i.second);
    }
  }
  mapInputIo_.clear();
  mapOutputIo_.clear();
}

void GpioMangager::addInIo(int pin) {
  int fd;
  pin_ = std::to_string(pin);

  ioExport(pin);
  ioDirectionSet(pin, false);

  std::string file = "/sys/class/gpio/gpio" + pin_ + "/value";
  fd = open(file.data(), O_WRONLY);
  if(fd == -1){
    ROS_ERROR("[gpio]Unable to open /sys/class/gpio/gpio%i/value", pin);
  }
  mapInputIo_.insert(std::make_pair(pin, fd));
}

void GpioMangager::addOutIo(int pin) {
  int fd;
  pin_ = std::to_string(pin);

  ioExport(pin);
  ioDirectionSet(pin, true);

  std::string file = "/sys/class/gpio/gpio" + pin_ + "value";
  fd = open(file.data(), O_WRONLY);
  if(fd == -1){
    ROS_ERROR("[gpio]Unable to open /sys/class/gpio/gpio%i/value", pin);
  }
  mapOutputIo_.insert(std::make_pair(pin, fd));
}

void GpioMangager::writeOutput(int pin, bool IS_HIGH) {
  lseek(mapOutputIo_[pin],0,SEEK_SET);
  if(IS_HIGH)
  {
    int ref = write(mapOutputIo_[pin],"1",1);
    if(ref == -1)
      ROS_ERROR("[GPIO]Failed to write to GPIO%d.", pin);
  }
  else {
    int ref = write(mapOutputIo_[pin], "0", 1);
    if(ref == -1)
      ROS_ERROR("[GPIO]Failed to write to GPIO%d.", pin);
  }
}

void GpioMangager::readInput() {

}