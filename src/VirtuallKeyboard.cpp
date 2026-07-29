#include "VirtualKeyboard.h"

#include "debugging/Logger.h"

#include <cstring>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <unistd.h>

void VirtualKeyboard::emit(int type, int code, int value) {
  struct input_event inputEvent;
  memset(&inputEvent, 0, sizeof(inputEvent));

  inputEvent.type = type;
  inputEvent.code = code;
  inputEvent.value = value;

  if (write(numpadDeviceFileDescriptor, &inputEvent, sizeof(inputEvent)) < 0) {
    Logger::error("Failed to write event to uinput device");
  }

  Logger::success("Sent " + std::to_string(type) + ", " + std::to_string(code) + ", " + std::to_string(value));
}

VirtualKeyboard::VirtualKeyboard() {
  numpadDeviceFileDescriptor = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  if (numpadDeviceFileDescriptor < 0) {
    Logger::error("Failed to open virtual keyboard. Is the uinput module loaded?");
  }

  ioctl(numpadDeviceFileDescriptor, UI_SET_EVBIT, EV_KEY);
  ioctl(numpadDeviceFileDescriptor, UI_SET_EVBIT, EV_SYN);

  for (int key = 0; key <= KEY_MAX; ++key) {
    ioctl(numpadDeviceFileDescriptor, UI_SET_KEYBIT, key);
  }

  struct uinput_setup inputSetup;
  memset(&inputSetup, 0, sizeof(inputSetup));

  inputSetup.id.bustype = BUS_USB;
  inputSetup.id.vendor = 0x1234;
  inputSetup.id.product = 0x5678;
  inputSetup.id.version = 1;
  strcpy(inputSetup.name, "Asus ROG Strix SCAR 16 Numpad");

  ioctl(numpadDeviceFileDescriptor, UI_DEV_SETUP, &inputSetup);

  if (ioctl(numpadDeviceFileDescriptor, UI_DEV_CREATE) < 0) {
    close(numpadDeviceFileDescriptor);
    Logger::error("Failed to create uinput device");
  }
}

VirtualKeyboard::~VirtualKeyboard() {
  if (numpadDeviceFileDescriptor >= 0) {
    ioctl(numpadDeviceFileDescriptor, UI_DEV_DESTROY);
    close(numpadDeviceFileDescriptor);
  }
}

void VirtualKeyboard::keyDown(int keycode) {
  emit(EV_KEY, keycode, 1);
  emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualKeyboard::keyUp(int keycode) {
  emit(EV_KEY, keycode, 0);
  emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualKeyboard::keyPress(int keycode) {
  keyDown(keycode);
  keyUp(keycode);
}