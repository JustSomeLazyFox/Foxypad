#include "Numberpad.h"

#include "AsusTouchpadI2C.h"
#include "HardwareDetection.h"
#include "VirtualKeyboard.h"
#include "debugging/Logger.h"
#include "util/shape.h"

#include <atomic>
#include <chrono>
#include <csignal>
// #include <cstdlib>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <libudev.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <poll.h>
#include <ratio>
#include <string>
#include <unistd.h>

namespace Numberpad {
AsusTouchpadI2C touchpadI2c{"/dev/i2c-0", 0x38};
Rectangle numlockZone{(Vector2D(3270, 250)), (Vector2D(600, 400)), Pivote::CORNER_TOP_LEFT};
Vector2D touchpadDimensions{};
Vector2D currentTouchPosition{};

std::atomic<bool> shouldRunEventLoop{true};

int touchpadDeviceFileDescriptor;
bool isNumberpadActive = false;
bool isTouchInNumlockZone = false;

void handleProgramInterrupt(int signalNumber) {
  std::string signal;
  switch (signalNumber) {
  case SIGINT:
    signal = "INTERRUPT";
    break;
  case SIGTERM:
    signal = "TERMINATE";
    break;
  default:
    signal = std::to_string(signalNumber);
    break;
  }
  Logger::warning("Event loop interrupted by signal " + signal);
  shouldRunEventLoop.store(false);
}

void startNumberpadDriver() {
  struct udev *udevContext = udev_new();

  VirtualKeyboard vk;
  touchpadI2c.turnOffNumpad();

  std::string touchpadDevicePath = findTouchDeviceNode(udevContext);
  touchpadDeviceFileDescriptor = open(touchpadDevicePath.c_str(), O_RDONLY | O_NONBLOCK);

  if (touchpadDeviceFileDescriptor < 0) {
    Logger::error("Failed to open touchpad device file descriptor");
    return;
  }

  struct libevdev *numberpadDevice = nullptr;
  if (libevdev_new_from_fd(touchpadDeviceFileDescriptor, &numberpadDevice) != 0) {
    Logger::error("Failed to create new libevdev device");
    close(touchpadDeviceFileDescriptor);
    return;
  }

  touchpadDimensions = Vector2D();
  touchpadDimensions.setX(libevdev_get_abs_maximum(numberpadDevice, ABS_MT_POSITION_X));
  touchpadDimensions.setY(libevdev_get_abs_maximum(numberpadDevice, ABS_MT_POSITION_Y));

  currentTouchPosition = Vector2D();

  std::signal(SIGINT, handleProgramInterrupt);
  std::signal(SIGTERM, handleProgramInterrupt);

  bool alreadyDetectedNumlockHold = false;
  auto numlockHoldTimeStart = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::milli> requiredNumlockHoldTime = std::chrono::milliseconds(1000);

  struct pollfd pollFileDescriptor;
  pollFileDescriptor.fd = touchpadDeviceFileDescriptor;
  pollFileDescriptor.events = POLLIN;

  while (shouldRunEventLoop.load()) {
    int pollTimeout = isTouchInNumlockZone ? 50 : 1000;
    int pollReturnCode = poll(&pollFileDescriptor, 1, pollTimeout);
    if (pollReturnCode == -1) {
      if (shouldRunEventLoop.load()) {
        Logger::error("Poll failed");
      } else {
        // clang-format off
        Logger::info(
          AnsiConstructor()
          .foreground(AnsiColor::CYAN)
          .text("Termination requested. ")
          .foreground(AnsiColor::GREEN)
          .text("Gracefully shutting down...")
          .construct()
        );
        //clang-format on
      }
      break;
    }
    if (pollReturnCode > 0 && ((pollFileDescriptor.revents & POLLIN))) {
      struct input_event inputEvent;
      while (libevdev_next_event(numberpadDevice, LIBEVDEV_READ_FLAG_NORMAL, &inputEvent) == LIBEVDEV_READ_STATUS_SUCCESS) {
        if (inputEvent.type == EV_ABS) {
          if (inputEvent.code == ABS_MT_POSITION_X || inputEvent.code == ABS_X) {
            currentTouchPosition.setX(inputEvent.value);
          } else if (inputEvent.code == ABS_MT_POSITION_Y || inputEvent.code == ABS_Y) {
            currentTouchPosition.setY(inputEvent.value);
          }
          // Logger::check("Position: " + currentTouchPosition.toString());
          if (numlockZone.contains(currentTouchPosition)) {
            isTouchInNumlockZone = true;
          } else {
            isTouchInNumlockZone = false;
            numlockHoldTimeStart = std::chrono::steady_clock::now();
          }
        } else if (inputEvent.type == EV_KEY && inputEvent.code == BTN_TOUCH) {
          // numlockHoldTimeStart = std::chrono::steady_clock::now();
          if (inputEvent.value == 1) {
            numlockHoldTimeStart = std::chrono::steady_clock::now();
            Logger::log("Touch down");
            if (numlockZone.contains(currentTouchPosition)) {
              isTouchInNumlockZone = true;
              alreadyDetectedNumlockHold = false;
              Logger::info("Touch in numlock zone");
            } else {
              numlockHoldTimeStart = std::chrono::steady_clock::now();
              Logger::info("Touch not in numlock zone");
            }
          } else if (inputEvent.value == 0) {
            Logger::log("Touch up");
            isTouchInNumlockZone = false;
            numlockHoldTimeStart = std::chrono::steady_clock::now();
          }
        }
      }
    }

    if (isTouchInNumlockZone && !alreadyDetectedNumlockHold) {
      if (numlockZone.contains(currentTouchPosition)) {
        auto nyow = std::chrono::steady_clock::now();
        auto elaspedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nyow - numlockHoldTimeStart).count();
        // Logger::check("Elasped Milliseconds: " + std::to_string(elaspedMilliseconds));
        if (elaspedMilliseconds >= 1000) {
          elaspedMilliseconds = -1;
          numlockHoldTimeStart = std::chrono::steady_clock::now();
          isNumberpadActive = !isNumberpadActive;
          touchpadI2c.toggleNumpadState();
          Logger::success("Toggled Numlock State!");

          if (isNumberpadActive) {
            libevdev_grab(numberpadDevice, LIBEVDEV_GRAB);
            Logger::log("Numberpad on. Grabbed numpad device");
          } else {
            libevdev_grab(numberpadDevice, LIBEVDEV_UNGRAB);
            Logger::log("Numberpad off. Ungrabbed numpad device");
          }

          alreadyDetectedNumlockHold = true;
        }
      } else {
        isTouchInNumlockZone = false;
        numlockHoldTimeStart = std::chrono::steady_clock::now();
      }
    }
  }
  libevdev_free(numberpadDevice);
  close(touchpadDeviceFileDescriptor);
  udev_unref(udevContext);
  Logger::log("Touchpad driver stopped");
}
} // namespace Numberpad