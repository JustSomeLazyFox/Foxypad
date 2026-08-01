#include "fsutils.h"

#include "../debugging/Logger.h"

#include <cstdlib>
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

std::string getCanonicalPathRelativeTo(std::filesystem::path filePath, std::filesystem::path relativeTo) {
  if (filePath.string().starts_with("~/")) {
    std::filesystem::path canonicalPath = std::string(getenv("HOME")) + filePath.string().substr(1);
    return std::filesystem::canonical(canonicalPath).string();
  } else if (filePath.string().starts_with(".")) {
    std::filesystem::path baseDirectory = std::filesystem::is_directory(relativeTo) ? relativeTo : relativeTo.parent_path();
    return std::filesystem::weakly_canonical(baseDirectory / filePath).string();
  }
  return std::filesystem::weakly_canonical(filePath).string();
}