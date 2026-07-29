#include "src/Numberpad.h"
#include "src/debugging/Logger.h"
#include "src/util/fsutils.h"

#include <cstdlib>
#include <linux/input-event-codes.h>
#include <string>
#include <unistd.h>

int main() {
  try {
    std::string userHome = std::getenv("HOME");
    std::string configFilePath = userHome + "/.config/foxypad/config.lua";
    setupProjectConfigAtPath(configFilePath);
    Numberpad::startNumberpadDriver(configFilePath);
  } catch (const std::exception &e) {
    Logger::error(e.what());
    return 1;
  }
  return 0;
}