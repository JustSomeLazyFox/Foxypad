#include "AsusTouchpadI2C.h"

#include "debugging/Logger.h"

#include <cstdint>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

AsusTouchpadI2C::AsusTouchpadI2C(const std::string &i2cDevice, int i2cAddress) {
  this->i2cAddress = i2cAddress;
  touchpadDeviceFileDescriptor = open(i2cDevice.c_str(), O_RDWR);
  if (touchpadDeviceFileDescriptor < 0) {
    Logger::error("Failed to open I2C bus");
  }

  if (ioctl(touchpadDeviceFileDescriptor, I2C_SLAVE_FORCE, i2cAddress) < 0) {
    close(touchpadDeviceFileDescriptor);
    Logger::error("Failed to acquire I2C access");
  }
}

AsusTouchpadI2C::~AsusTouchpadI2C() {
  if (touchpadDeviceFileDescriptor < 0)
    close(touchpadDeviceFileDescriptor);
}

void AsusTouchpadI2C::setNumpadState(bool shouldEnable, bool log) {
  // clang-format off
  std::vector<uint8_t> payload = {
    0x05, 0x00, 0x3d, 0x03,
    0x06, 0x00, 0x07, 0x00,
    0x0d, 0x14, 0x03,
    static_cast<uint8_t>(shouldEnable ? 0x01 : 0x00),
    0xad
  };
  // clang-format on

  if (write(touchpadDeviceFileDescriptor, payload.data(), payload.size()) != payload.size()) {
    Logger::error("Failed to write magic packet to I2C device");
  } else {
    if (log) {
      Logger::info("Numpad state set to: " + std::string(shouldEnable ? "ON" : "OFF"));
    }
    isNumpadEnabled = shouldEnable;
  }
}