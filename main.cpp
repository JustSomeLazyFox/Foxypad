#include "src/Numberpad.h"
// #include "src/VirtualKeyboard.h"
#include "src/debugging/Logger.h"

#include <linux/input-event-codes.h>
#include <unistd.h>

int main() {
  // if (geteuid() != 0) {
  //   Logger::error((new AnsiConstructor())->text("Please run the program as root using ").foreground(AnsiColor::CYAN).text("`sudo`").construct());
  //   return 1;
  // }
  try {
    // VirtualKeyboard vk;
    // Logger::info("Virtual keyboard created. Sending '5' in 3 seconds...");

    // sleep(3);
    // vk.keyPress(KEY_NUMLOCK);
    // vk.keyPress(KEY_KPENTER);/
    // Logger::success("Keystroke sent!");
    Numberpad::startNumberpadDriver();
  } catch (const std::exception &e) {
    Logger::error(e.what());
    return 1;
  }

  return 0;
}