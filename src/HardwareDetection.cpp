#include "HardwareDetection.h"

#include "debugging/Logger.h"

#include <libudev.h>
#include <string>
#include <string_view>

std::string findTouchDeviceNode(struct udev *udevCtx) {
  std::string foundNode = "";
  struct udev_enumerate *enumerate = udev_enumerate_new(udevCtx);

  if (!enumerate) {
    Logger::error("Failed to create udev enumerate object");
    return foundNode;
  }

  udev_enumerate_add_match_subsystem(enumerate, "input");
  udev_enumerate_scan_devices(enumerate);

  struct udev_list_entry *inputDevices = udev_enumerate_get_list_entry(enumerate);
  struct udev_list_entry *entry;

  Logger::info("Scanning for touchpad devices...");

  udev_list_entry_foreach(entry, inputDevices) {
    const char *path = udev_list_entry_get_name(entry);
    struct udev_device *inputDevice = udev_device_new_from_syspath(udevCtx, path);
    if (!inputDevice) {
      Logger::error("Failed to create udev device from syspath: " + std::string(path));
      continue;
    }

    const char *deviceNode = udev_device_get_devnode(inputDevice);
    if (!deviceNode) {
      Logger::test(false, "", "Failed to get device node for syspath: " + std::string(path));
      udev_device_unref(inputDevice);
      continue;
    }

    std::string_view nodeString(deviceNode);

    if (nodeString.find("event") == std::string_view::npos) {
      Logger::test(false, "",
                   AnsiConstructor()
                       .foreground(AnsiColor::YELLOW)
                       .text("Skipping device node \"" + std::string(nodeString) + "\" as it does not expose events")
                       .construct());
      udev_device_unref(inputDevice);
      continue;
    }

    const char *isTouchpad = udev_device_get_property_value(inputDevice, "ID_INPUT_TOUCHPAD");
    if (!isTouchpad || std::string_view(isTouchpad) != "1") {
      Logger::test(false, "",
                   AnsiConstructor()
                       .foreground(AnsiColor::CYAN)
                       .text("Skipping device node " + std::string(nodeString) + " as it is not a touchpad")
                       .construct());
      udev_device_unref(inputDevice);
      continue;
    }

    const char *touchpadName = udev_device_get_sysattr_value(inputDevice, "name");
    Logger::test(
        true, "Found a touchpad device " + std::string(touchpadName ? touchpadName : "<NameNyotSpecified>") + " at " + std::string(deviceNode), "");
    foundNode = deviceNode;
    udev_device_unref(inputDevice);
    Logger::success("Found a touchpad device connected to this PC");
    break;
  }

  udev_enumerate_unref(enumerate);

  return foundNode;
}