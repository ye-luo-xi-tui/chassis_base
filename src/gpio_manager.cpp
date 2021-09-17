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
  if (!mapOutputIo_.empty()) {
    for (auto i : mapOutputIo_) {
      close(i.second);
      ioUnExport(std::to_string(i.first));
    }
  }
  if (!mapInputIo_.empty()) {
    for (auto i : mapInputIo_) {
      int *ptr = i.second.begin()->second;
      delete ptr;
      close(i.second.begin()->first);
      ioUnExport(std::to_string(i.first));
    }
  }
  mapInputIo_.clear();
  mapOutputIo_.clear();
}

void GpioMangager::addInIo(int pin) {
  int fd;
  pin_ = std::to_string(pin);

  ioExport(pin_);
  ioDirectionSet(pin_, false);

  std::string file = "/sys/class/gpio/gpio" + pin_ + "/value";
  fd = open(file.data(), O_WRONLY);
  if (fd == -1) {
    ROS_ERROR("[gpio]Unable to open /sys/class/gpio/gpio%i/value", pin);
  }
  std::map<int, int *> mapfd2data;
  mapfd2data.insert(std::make_pair(fd, new int));
  mapInputIo_.insert(std::make_pair(pin, mapfd2data));
}

void GpioMangager::addOutIo(int pin) {
  int fd;
  pin_ = std::to_string(pin);

  ioExport(pin_);
  ioDirectionSet(pin_, true);

  std::string file = "/sys/class/gpio/gpio" + pin_ + "/value";
  fd = open(file.data(), O_WRONLY);
  if (fd == -1) {
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
  int j = 0;
  for (auto iter : mapInputIo_) {
    fds[j].fd = iter.second.begin()->first;
    fds[j].events = POLLPRI;
    j++;
  }
  char state;
  int ret = poll(fds, mapInputIo_.size(), 0);
  if (ret == -1) {
    ROS_ERROR("poll failed!\n");
  }
  for (int i = 0; i < mapInputIo_.size(); i++) {
    if (fds[i].revents & POLLPRI) {
      int gpio_fd = fds[i].fd;
      lseek(gpio_fd, 0, SEEK_SET);
      ret = read(gpio_fd, &state, 1);
      if (ret == -1) {
        ROS_ERROR("[gpio]:run :read failed");
        break;
      }
    }
    *mapInputIo_[i][fds[i].fd] = (state == 0x31);
  }
}

void GpioMangager::ioExport(std::string pin) {
  std::string file = "/sys/class/gpio/export";
  int fd;
  fd = open(file.data(), O_WRONLY);
  if (fd == -1) {
    ROS_ERROR("[gpio]Unable to open %s", file.data());
  }
  lseek(fd, 0, SEEK_SET);
  if (write(fd, pin.data(), pin.size()) != pin.size()) {
    ROS_ERROR("[gpio]failed to export gpio%s", pin.data());
  }
  close(fd);
}

void GpioMangager::ioUnExport(std::string pin) {
  std::string file = "/sys/class/gpio/unexport";
  int fd;
  fd = open(file.data(), O_WRONLY);
  if (fd == -1) {
    ROS_ERROR("[gpio]Unable to open %s", file.data());
  }
  if (write(fd, pin.data(), pin.size()) != pin.size()) {
    ROS_ERROR("[gpio]failed to export gpio%s", pin.data());
  }
  close(fd);
}

void GpioMangager::ioDirectionSet(std::string pin, bool IS_OUT) {
  std::string file = "/sys/class/gpio/gpio" + pin + "/direction";
  int fd;
  fd = open(file.data(), O_WRONLY);
  if (fd == -1) {
    ROS_ERROR("[gpio]Unable to open %s", file.data());
  }
  if (IS_OUT) {
    if (write(fd, "out", 3) != 3) {
      ROS_ERROR("[gpio]Failed to set direction of gpio%s", pin.data());
    }
  } else {
    if (write(fd, "in", 2) != 2) {
      ROS_ERROR("[gpio]Failed to set direction of gpio%s", pin.data());
    }
  }
  close(fd);
}