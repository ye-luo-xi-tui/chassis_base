//
// Created by yezi on 2021/9/9.
//

#ifndef SRC_CHASSIS_BASE_INCLUDE_GPIO_MANAGER_H_
#define SRC_CHASSIS_BASE_INCLUDE_GPIO_MANAGER_H_

#include <fcntl.h>
#include <string>
#include <map>
#include <poll.h>

class GpioMangager{
  public:
    explicit  GpioMangager();
    ~GpioMangager();
    void addInIo(int pin);
    void addOutIo(int pin);
    void writeOutput(int pin, bool IS_HIGH);
    void readInput();

  private:
  void ioExport(int pin);
  void ioUnExport(int pin);
  void ioDirectionSet(int pin, bool IS_OUT);
  std::map<int, int> mapOutputIo_;
  std::map<int, int> mapInputIo_;
  std::string pin_;
  struct pollfd fds[20]{};
};


#endif // SRC_CHASSIS_BASE_INCLUDE_GPIO_MANAGER_H_
