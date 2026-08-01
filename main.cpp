#include "src/HardwareDetection.h"
#include "src/Numberpad.h"
#include "src/debugging/Debugger.h"
#include "src/debugging/Logger.h"
#include "src/util/cmdutils.h"
#include "src/util/fsutils.h"
#include "src/util/mathutils.h"
#include "src/util/vector.h"

#include <cstdlib>
#include <fcntl.h>
#include <filesystem>
#include <libevdev/libevdev.h>
#include <libudev.h>
#include <linux/input-event-codes.h>
#include <string>
#include <unistd.h>

int main(int argc, char **argv) {
  Debugger::setLogLevel(Debugger::LogLevel::SUCCESS_STATUSES);
  Debugger::enableLogging();
  Debugger::disableRateLimit();
  try {
    std::string userHome = std::getenv("HOME");
    std::string configFilePath = userHome + "/.config/foxypad/config.lua";
    std::string i2cDevice = "/dev/i2c-0";
    bool isGenericTouchpad = false;
    bool isOldModelTouchpad = false;
    CommandlineParser parser;
    // clang-format off
    parser.setApplicationDescription("A device driver for Asus laptops with Numberpad support with experimental support for generic touchpads on any laptop");
    parser.setApplicationUsage("foxypad [options...] \u001B[37m# arguments are parsed sequentially\u001B[0m");
    parser.setApplicationVersion("1.1.0");
    parser.addArgument(CommandlineArgument("config-file", 'c', "Read from a different config file", CommandlineArgumentType::STRING, [&](std::string filePath) { configFilePath = getCanonicalPathRelativeTo(filePath, std::filesystem::current_path()); }));
    parser.addArgument(CommandlineArgument("generic", 'g', "For use with non-Asus laptops or devices without Numberpad support", CommandlineArgumentType::BOOLEAN, [&]() { isGenericTouchpad = true; }));
    parser.addArgument(CommandlineArgument("old-model", 'o', "Compatibility with older models of Asus Numberpad", CommandlineArgumentType::BOOLEAN, [&]() { isOldModelTouchpad = true; }));
    parser.addArgument(CommandlineArgument("i2c-device", 'i', "Use a different i2c device path. Default is " + i2cDevice + ".", CommandlineArgumentType::STRING, [&](std::string path) { i2cDevice = path; }));
    parser.addArgument(CommandlineArgument("log-level", 'l', "Set the log level. Possible values: ERRORS_ONLY, ERRORS_AND_WARNINGS, SUCCESS_STATUSES, INFO, DEBUG (case-insensitive)", CommandlineArgumentType::STRING, 
      [&](std::string level) {
          if (level.empty()) {
            Logger::warning("Log level nyot specified. Defaulting to SUCCESS_STATUSES");
            Debugger::setLogLevel(Debugger::SUCCESS_STATUSES);
            return;
          }
          Logger::info("level = " + level);
          if (equalsIgnoreCase(level, "ERRORS_ONLY")) {
            Debugger::setLogLevel(Debugger::LogLevel::ERRORS_ONLY);
          } else if (equalsIgnoreCase(level, "ERRORS_AND_WARNINGS")) {
            Debugger::setLogLevel(Debugger::LogLevel::ERRORS_AND_WARNINGS);
          } else if (equalsIgnoreCase(level, "SUCCESS_STATUSES")) {
            Debugger::setLogLevel(Debugger::LogLevel::SUCCESS_STATUSES);
          } else if (equalsIgnoreCase(level, "INFO")) {
            Debugger::setLogLevel(Debugger::LogLevel::INFO);
          } else if (equalsIgnoreCase(level, "DEBUG")) {
            Debugger::setLogLevel(Debugger::LogLevel::DEBUG);
          } else {
            Logger::warning("Unknown log level: \"" + level + "\". Defaulting to ERRORS_AND_WARNINGS.");
          }
        }));
    parser.addArgument(CommandlineArgument("dimensions", 'd', "Detect and print the dimensions of the touchpad device", CommandlineArgumentType::BOOLEANISH, 
      [&]() {
          struct udev *udevContext = udev_new();
          int touchpadDeviceFileDescriptor = open(findTouchDeviceNode(udevContext).c_str(), O_RDONLY | O_NONBLOCK);
          if (touchpadDeviceFileDescriptor < 0) {
            Logger::error("Failed to open touchpad device file descriptor");
            return;
          }
          Vector2D touchpadDimensions;
          struct libevdev *numberpadDevice = nullptr;
          if (libevdev_new_from_fd(touchpadDeviceFileDescriptor, &numberpadDevice) != 0) {
            Logger::error("Failed to create new libevdev device");
            close(touchpadDeviceFileDescriptor);
            return;
          }
          touchpadDimensions.setX(libevdev_get_abs_maximum(numberpadDevice, ABS_MT_POSITION_X));
          touchpadDimensions.setY(libevdev_get_abs_maximum(numberpadDevice, ABS_MT_POSITION_Y));
          std::string message = AnsiConstructor()
                                    .foreground(BLUE)
                                    .bold()
                                    .text("Touchpad Dimensions: ")
                                    .foreground(WHITE)
                                    .text("<")
                                    .foreground(RED)
                                    .text(std::to_string(touchpadDimensions.x))
                                    .foreground(WHITE)
                                    .text(", ")
                                    .foreground(GREEN)
                                    .text(std::to_string(touchpadDimensions.y))
                                    .foreground(WHITE)
                                    .text(">")
                                    .construct();
          std::cout << message << std::endl;
          libevdev_free(numberpadDevice);
          close(touchpadDeviceFileDescriptor);
          exit(0);
        }));
    // clang-format on
    Logger::info("Logging level: " + std::to_string(Debugger::logLevel));
    parser.parse(argc, argv);
    setupProjectConfigAtPath(configFilePath);
    Numberpad::startNumberpadDriver(configFilePath, i2cDevice, isOldModelTouchpad ? 0x15 : 0x38, isGenericTouchpad);
  } catch (const std::exception &e) {
    Logger::error(e.what());
    return 1;
  }
  return 0;
}