#pragma once

class VirtualKeyboard {
private:
  int numpadDeviceFileDescriptor;
  void emit(int type, int code, int value);

public:
  VirtualKeyboard();
  ~VirtualKeyboard();
  void keyDown(int keycode);
  void keyUp(int keycode);
  void keyPress(int keycode);
};