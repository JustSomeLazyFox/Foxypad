#include "fsutils.h"

#include "../debugging/Logger.h"

#include <filesystem>

void setupProjectConfigAtPath(std::string configFilePath) {
  std::filesystem::path configFile = std::filesystem::weakly_canonical(configFilePath);
  if (!std::filesystem::exists(configFile)) {
    Logger::warning("Config file does nyot exist");
    Logger::info("Creating config file at: " + configFilePath);
    if (configFile.has_parent_path()) {
      std::filesystem::create_directories(configFile.parent_path());
    } else {
      std::filesystem::create_directories(configFile);
    }
    Logger::success("Created file at: " + configFilePath);
  }
}