#pragma once

#include "shape.h"
#include <atomic>
#include <string>
#include <vector>

typedef struct {
  std::vector<std::string> profileNames;
  std::vector<std::vector<Shape>> touchRegionsPerProfile;
  std::vector<std::vector<void (*)()>> keycodeFunctionsPerProfile;
} Config;

typedef struct {
  int keyCode;
  int holdMilliseconds;
} KeyPressDescription;

void initializeLuaScriptingApi();

namespace ConfigWatcher {
extern std::atomic_bool shouldWatchFile;
void loadConfig(std::string configFilePath);
void watchConfigFile(const std::string configFilePath);
void stopWatching();
} // namespace ConfigWatcher