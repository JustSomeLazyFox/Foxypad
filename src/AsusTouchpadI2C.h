#pragma once

#include <string>

class AsusTouchpadI2C {
private:
  int touchpadDeviceFileDescriptor;
  int i2cAddress;
  bool isNumpadEnabled = false;
  bool isGenericTouchpad;
  void setNumpadState(bool shouldEnable, bool log = true);

public:
  AsusTouchpadI2C();
  ~AsusTouchpadI2C();

  void initialize(const std::string &i2cDevice, int i2cAddress, bool isGenericTouchpad);
  void turnOnNumpad(bool log = true) { setNumpadState(true, log); }
  void turnOffNumpad(bool log = true) { setNumpadState(false, log); }
  void toggleNumpadState() {
    if (isNumpadEnabled)
      turnOffNumpad();
    else
      turnOnNumpad();
  }
};