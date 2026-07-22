#pragma once

#include <string>

class AsusTouchpadI2C {
private:
  int touchpadDeviceFileDescriptor;
  int i2cAddress;
  bool isNumpadEnabled = false;
  void setNumpadState(bool shouldEnable);

public:
  AsusTouchpadI2C(const std::string &i2cDevice, int i2cAddress = 0x38);
  ~AsusTouchpadI2C();

  void turnOnNumpad() { setNumpadState(true); }
  void turnOffNumpad() { setNumpadState(false); }
  void toggleNumpadState() {
    if (isNumpadEnabled)
      turnOffNumpad();
    else
      turnOnNumpad();
  }
};