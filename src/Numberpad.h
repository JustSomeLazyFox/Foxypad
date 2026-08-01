#pragma once

#include "util/config.h"
#include "util/shape.h"
#include "util/vector.h"

#include <libevdev/libevdev.h>

#include <chrono>
#include <memory>
#include <string>

namespace Numberpad {
typedef struct {
  std::unique_ptr<Shape> region;
  int keyCode;
} KeyZone;
void startNumberpadDriver(const std::string &configFilePath, std::string_view i2cDevice, int i2cAddress, bool isGenericTouchpad);
void handleProgramInterrupt(int signalNumber);
// clang-format off
void handleAbsoluteMovementChange(
  unsigned short eventCode,
  unsigned short eventValue,
  std::chrono::steady_clock::time_point &numlockHoldTimeStart,
  const Config &config
);
void handleTouchEvents(
  unsigned short eventValue,
  std::chrono::steady_clock::time_point &numlockHoldTimeStart,
  bool &alreadyDetectedNumlockHold,
  const Config &config,
  const Shape *&lastTouchedArea,
  const Vector2D &currentTouchPosition,
  struct libevdev *numberpadDevice
);
// clang-format on
bool isNumlockOn();
} // namespace Numberpad