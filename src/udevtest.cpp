#include <libudev.h>

#include "HardwareDetection.h"
#include "VirtualKeyboard.h"
#include "debugging/Logger.h"

void runUdevTest() {
  struct udev *udevCtx = udev_new();
  if (!udevCtx) {
    Logger::error("Failed to create udev object.");
    return;
  }

  Logger::success("udev initialized successfully");

  std::string devicePath = findTouchDeviceNode(udevCtx);
  udev_unref(udevCtx);

  if (devicePath.empty()) {
    Logger::error("No touchpad event node found. Are you running as root?");
    return;
  }

  Logger::success("Found target touchpad device node at " + devicePath);

  new VirtualKeyboard();

  Logger::info("udev uninitialized");
}