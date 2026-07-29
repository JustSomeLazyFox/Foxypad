#pragma once

#include "../VirtualKeyboard.h"
#include "shape.h"

#include <atomic>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct Region {
  std::shared_ptr<Shape> shape;
  int holdMilliseconds;
  bool performAfterRelease;
  std::optional<std::function<void()>> action;
};

struct Profile {
  std::string name;
  std::vector<Region> regions;
};

struct Config {
  std::vector<Profile> profiles;
  std::string currentProfileName;
  std::shared_ptr<Shape> activationRegion;
  int activationTimeMilliseconds;
  int idleTimeoutSeconds;
  bool toggleOffOnIdle;
  void useProfile(const std::string &profileName);
  std::optional<std::reference_wrapper<const Profile>> getProfile(const std::string &profileName) const;
  void reset();
};

void initializeLuaScriptingApi(VirtualKeyboard &virtualKeyboard, Config &config);

namespace ConfigWatcher {
extern std::atomic_bool shouldWatchFile;
void loadConfig(const std::string &configFilePath, Config &config);
void watchConfigFile(const std::string &configFilePath, Config &config);
void stopWatching();
} // namespace ConfigWatcher