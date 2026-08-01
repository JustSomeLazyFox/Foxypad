#include "Numberpad.h"

#include "AsusTouchpadI2C.h"
#include "HardwareDetection.h"
#include "VirtualKeyboard.h"
#include "debugging/Logger.h"
#include "util/cmdutils.h"
#include "util/config.h"
#include "util/shape.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <fcntl.h>
#include <functional>
#include <libevdev/libevdev.h>
#include <libudev.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <poll.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <thread>
#include <unistd.h>

namespace Numberpad {
AsusTouchpadI2C touchpadI2c;
Vector2D touchpadDimensions{};
Vector2D currentTouchPosition{};

std::chrono::time_point<std::chrono::steady_clock> lastInteractionTime;
std::atomic<bool> shouldRunEventLoop{true};

int touchpadDeviceFileDescriptor;
bool isNumberpadActive = false;
bool isTouchInNumlockZone = false;
bool isTouching = false;
bool isNumberpadCanonicallyEnabled = false;

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

void startNumberpadDriver(const std::string &configFilePath, std::string_view i2cDevice, int i2cAddress, bool isGenericTouchpad) {
  struct udev *udevContext = udev_new();

  VirtualKeyboard vk;
  Config config;

  touchpadI2c.initialize(std::string(i2cDevice), i2cAddress, isGenericTouchpad);

  initializeLuaScriptingApi(vk, config, configFilePath);
  ConfigWatcher::loadConfig(configFilePath, config);
  std::jthread autoUpdateConfig(ConfigWatcher::watchConfigFile, configFilePath, std::ref(config));

  std::string touchpadDevicePath = findTouchDeviceNode(udevContext);
  touchpadDeviceFileDescriptor = open(touchpadDevicePath.c_str(), O_RDONLY | O_NONBLOCK);

  if (touchpadDeviceFileDescriptor < 0) {
    Logger::error("Failed to open touchpad device file descriptor");
    return;
  }

  touchpadI2c.turnOffNumpad();
  if (isNumlockOn()) {
    vk.keyPress(KEY_NUMLOCK);
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

  Logger::info("Touchpad dimensions: " + touchpadDimensions.toString());

  currentTouchPosition = Vector2D();

  std::signal(SIGINT, handleProgramInterrupt);
  std::signal(SIGTERM, handleProgramInterrupt);

  // bool alreadyDetectedNumlockHold = false;
  // std::chrono::steady_clock::time_point numlockHoldTimeStart = std::chrono::steady_clock::now();
  // std::chrono::duration<double, std::milli> requiredNumlockHoldTime = std::chrono::milliseconds(config.activationTimeMilliseconds);

  const Shape *lastTouchedArea = nullptr;
  std::chrono::steady_clock::time_point currentRegionHoldTimeStart = std::chrono::steady_clock::now();
  bool alreadyPerformedCurrentRegionAction = false;

  struct pollfd pollFileDescriptor;
  pollFileDescriptor.fd = touchpadDeviceFileDescriptor;
  pollFileDescriptor.events = POLLIN;

  while (shouldRunEventLoop.load()) {
    if (!config.activationRegion) {
      Logger::severe("Nyo activation region specified in config. Stopping program...");
      shouldRunEventLoop.store(false);
    }
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

    // Logger::log("currentRegionHoldTime elaspeed: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - currentRegionHoldTimeStart).count()));
    // Logger::log("Time since last interaction: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastInteractionTime).count()));
    // Logger::log("Touch zone: " + (lastTouchedArea == nullptr ? "null" : lastTouchedArea->toString()));
    // Logger::check("Already performed current region action: " + std::to_string(alreadyPerformedCurrentRegionAction));

    std::chrono::duration timeSinceLastInteraction = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastInteractionTime);
    // Logger::check("Time since last interaction: " + std::to_string(timeSinceLastInteraction.count()) + "s");
    // Logger::check("Idle Timeout: " + std::to_string(config.idleTimeoutSeconds) + "s");
    

    if (isNumberpadActive && isNumberpadCanonicallyEnabled && timeSinceLastInteraction.count() >= 120) {
      touchpadI2c.turnOnNumpad();
      lastInteractionTime = std::chrono::steady_clock::now();
    }

    if (pollReturnCode > 0 && (pollFileDescriptor.revents & POLLIN)) {
      struct input_event inputEvent;
      while (libevdev_next_event(numberpadDevice, LIBEVDEV_READ_FLAG_NORMAL, &inputEvent) == LIBEVDEV_READ_STATUS_SUCCESS) {
        if (inputEvent.type == EV_ABS) {
          handleAbsoluteMovementChange(
            inputEvent.code,
            inputEvent.value,
            currentRegionHoldTimeStart,
            config
          );
        } else if (inputEvent.type == EV_KEY && inputEvent.code == BTN_TOUCH) {
          handleTouchEvents(
            inputEvent.value,
            currentRegionHoldTimeStart,
            alreadyPerformedCurrentRegionAction,
            config,
            lastTouchedArea,
            currentTouchPosition,
            numberpadDevice
          );
        }
      }
    }

    if (isTouching && config.activationRegion && config.activationRegion->contains(currentTouchPosition)) {
      Shape *numlockZone = config.activationRegion.get();
      if (lastTouchedArea != numlockZone) {
        alreadyPerformedCurrentRegionAction = false;
        currentRegionHoldTimeStart = std::chrono::steady_clock::now();
        lastTouchedArea = numlockZone;
      }
      auto nyow = std::chrono::steady_clock::now();
      auto elaspedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nyow - currentRegionHoldTimeStart).count();
      if (elaspedMilliseconds >= config.activationTimeMilliseconds && !alreadyPerformedCurrentRegionAction) {
        isNumberpadActive = !isNumberpadActive;
        touchpadI2c.toggleNumpadState();
        if (isNumberpadActive) {
          if (!config.numpadEnabledBellSoundPath.empty()) {
            launchAndDisownChild("paplay " + config.numpadEnabledBellSoundPath);
          }
          libevdev_grab(numberpadDevice, LIBEVDEV_GRAB);
          Logger::success("Numberpad on. Grabbed touchpad device, no events will be propagated to any programs.");
          isNumberpadCanonicallyEnabled = true;
          Logger::check("config.numpadEnabledBellSoundPath = " + config.numpadEnabledBellSoundPath);
        } else {
          if (!config.numpadDisabledBellSoundPath.empty()) {
            launchAndDisownChild("paplay " + config.numpadDisabledBellSoundPath);
          }
          libevdev_grab(numberpadDevice, LIBEVDEV_UNGRAB);
          Logger::success("Numberpad off. Ungrabbed touchpad device, events will be propagated to programs again");
          isNumberpadCanonicallyEnabled = false;
        }
        alreadyPerformedCurrentRegionAction = true;
      }
    }

    if (isNumberpadActive && config.idleTimeoutSeconds > 0) {
      if ( timeSinceLastInteraction.count() >= config.idleTimeoutSeconds) {
        touchpadI2c.turnOffNumpad();
        isNumberpadActive = false;
        Logger::info("Numberpad turned off due to inactivity");
        if (config.toggleOffOnIdle) {
          isNumberpadCanonicallyEnabled = false;
          libevdev_grab(numberpadDevice, LIBEVDEV_UNGRAB);
          Logger::success("Ungrabbed touchpad device, events will be propagated to programs again");
        }
      }
    }

    if (!isNumberpadActive) continue;

    if (config.profiles.empty()) {
      Logger::warning("No profiles found in config");
      continue;
    }

    if (config.currentProfileName == "touchpad") {
      isNumberpadActive = false;
      touchpadI2c.turnOffNumpad();
      isNumberpadCanonicallyEnabled = false;
      libevdev_grab(numberpadDevice, LIBEVDEV_UNGRAB);
      config.useProfile(config.profiles.begin()->name);
      continue;
    }

    auto currentProfile = config.getProfile(config.currentProfileName);
    if (!currentProfile.has_value()) {
      Logger::error("No profile found with name: " + config.currentProfileName);
      continue;
    }
    for (const Region &region : currentProfile->get().regions) {
      if (!region.shape) {
        Logger::warning("Region has no shape, skipping");
        continue;
      }
      Shape *shape = region.shape.get();
      if (isTouching && shape->contains(currentTouchPosition)) {
        if (lastTouchedArea != shape) {
          // Logger::check("last touched region is nyot the same, restarting timer");
          alreadyPerformedCurrentRegionAction = false;
          currentRegionHoldTimeStart = std::chrono::steady_clock::now();
          lastTouchedArea = shape;
          break;
        }
        auto nyow = std::chrono::steady_clock::now();
        auto elaspedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nyow - currentRegionHoldTimeStart).count();
        if (elaspedMilliseconds >= region.holdMilliseconds && !alreadyPerformedCurrentRegionAction && !region.performAfterRelease) {
          if (region.action.has_value()) {
            // Logger::check("performing action");
            region.action.value()();
            // Logger::check("Action performed");
          }
          currentRegionHoldTimeStart = std::chrono::steady_clock::now();
          alreadyPerformedCurrentRegionAction = true;
        }
        break;
      }
    }
  }
  touchpadI2c.turnOffNumpad();
  libevdev_free(numberpadDevice);
  close(touchpadDeviceFileDescriptor);
  udev_unref(udevContext);
  ConfigWatcher::stopWatching();
  Logger::log("Touchpad driver stopped");
}

void handleAbsoluteMovementChange(unsigned short eventCode, unsigned short eventValue, std::chrono::steady_clock::time_point &currentRegionHoldTimeStart, const Config &config) {
  if (eventCode == ABS_MT_POSITION_X || eventCode == ABS_X) {
    currentTouchPosition.setX(eventValue);
  } else if (eventCode == ABS_MT_POSITION_Y || eventCode == ABS_Y) {
    currentTouchPosition.setY(eventValue);
  }
  lastInteractionTime = std::chrono::steady_clock::now();
}

void handleTouchEvents(
  unsigned short eventValue, 
  std::chrono::steady_clock::time_point &currentRegionHoldTimeStart, 
  bool &alreadyPerformedCurrentRegionAction, 
  const Config &config, 
  const Shape *&lastTouchedArea, 
  const Vector2D &currentTouchPosition,
  struct libevdev *numberpadDevice
) {
  if (eventValue == 1) {
    isTouching = true;
    alreadyPerformedCurrentRegionAction = false;
    currentRegionHoldTimeStart = std::chrono::steady_clock::now();
    // Logger::check("Reseting hold timer as touch just started");
    Logger::log("Touch down");
    if (isNumberpadActive) {
      touchpadI2c.turnOnNumpad(false);
    }
    if (config.activationRegion && config.activationRegion->contains(currentTouchPosition)) {
      lastTouchedArea = config.activationRegion.get();
      return;
    } else {
      if (!isNumberpadActive) return;
      if (config.currentProfileName == "touchpad") goto endOfFunction;
      auto currentProfile = config.getProfile(config.currentProfileName);
      if (!currentProfile.has_value()) {
        Logger::error("Nyo profile found with name: " + config.currentProfileName);
        return;
      }
      const Profile &profile = currentProfile->get();
      for (const Region &region : profile.regions) {
        if (!region.shape) {
          Logger::warning("Region has no shape, skipping");
          continue;
        }
        Shape *shape = region.shape.get();
        if (shape->contains(currentTouchPosition)) {
          if (region.holdMilliseconds <= 0 && !region.performAfterRelease) {
            if (region.action.has_value()) {
              region.action.value()();
              alreadyPerformedCurrentRegionAction = true;
            }
          }
          lastTouchedArea = shape;
          return;
        }
      }
    }
  } else if (eventValue == 0) {
    Logger::log("Touch up");
    isTouching = false;
    isTouchInNumlockZone = false;
    if (isNumberpadActive && lastTouchedArea != nullptr && config.currentProfileName != "touchpad") {
      auto currentProfile = config.getProfile(config.currentProfileName);
      if (currentProfile.has_value()) {
        for (auto &region : currentProfile->get().regions) {
          if (region.shape.get() == lastTouchedArea && region.performAfterRelease) {
            if (!alreadyPerformedCurrentRegionAction) {
              auto nyow = std::chrono::steady_clock::now();
              auto elaspedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nyow - currentRegionHoldTimeStart).count();
              Logger::log("Elasped ms = " + std::to_string(elaspedMilliseconds));
              Logger::log("Region ms = " + std::to_string(region.holdMilliseconds));
              if (elaspedMilliseconds >= region.holdMilliseconds && region.action.has_value()) {
                region.action.value()();
                alreadyPerformedCurrentRegionAction = true;
              }
            }
            break;
          }
        }
      } else {
        Logger::error("Nyo profile found with name: " + config.currentProfileName);
      }
    }
    currentRegionHoldTimeStart = std::chrono::steady_clock::now();
    // Logger::check("Resetting hold timer as touch is lifted up");
    lastTouchedArea = nullptr;
  }
  endOfFunction:
  if (isNumberpadCanonicallyEnabled) {
    touchpadI2c.turnOnNumpad(false);
    isNumberpadActive = true;
    libevdev_grab(numberpadDevice, LIBEVDEV_GRAB);
  }
  lastInteractionTime = std::chrono::steady_clock::now();
}

bool isNumlockOn() {
  unsigned long leds = 0;

  if (ioctl(touchpadDeviceFileDescriptor, EVIOCGLED(sizeof(leds)), &leds) < 0) {
    Logger::error("Failed to get leds state");
    return false;
  }
  return (leds >> LED_NUML) & 1;
}
} // namespace Numberpad