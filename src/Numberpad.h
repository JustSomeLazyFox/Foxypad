#pragma once

#include "util/shape.h"
#include <memory>

namespace Numberpad {
typedef struct {
  std::unique_ptr<Shape> region;
  int keyCode;
} KeyZone;
void startNumberpadDriver();
void handleProgramInterrupt(int signalNumber);
} // namespace Numberpad