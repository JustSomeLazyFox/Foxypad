#include "config.h"

#include "../VirtualKeyboard.h"
#include "../debugging/Logger.h"
#include "cmdutils.h"
#include "fsutils.h"
#include "keycodes.h"
#include "mathutils.h"
#include "shape.h"
#include "vector.h"

#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <linux/input-event-codes.h>
#include <optional>
#include <poll.h>
#include <sol/error.hpp>
#include <sol/forward.hpp>
#include <sol/inheritance.hpp>
#include <sol/load_result.hpp>
#include <sol/object.hpp>
#include <sol/optional_implementation.hpp>
#include <sol/raii.hpp>
#include <sol/sol.hpp>
#include <sol/state.hpp>
#include <sol/table.hpp>
#include <sol/types.hpp>
#include <string>
#include <sys/inotify.h>
#include <thread>
#include <unistd.h>
#include <vector>

Config *currentConfig = nullptr;

sol::state luaContext;
std::atomic_bool ConfigWatcher::shouldWatchFile{false};

bool firstRun = true;
thread_local bool isLuaRuntimeExecuting = true;

void Config::useProfile(const std::string &profileName) {
  if (profileName == "touchpad") {
    currentProfileName = profileName;
    return;
  }
  for (Profile &profile : profiles) {
    if (profile.name == profileName) {
      currentProfileName = profileName;
      return;
    }
  }
  Logger::error("profile \"" + profileName + "\" nyot found");
}

std::optional<std::reference_wrapper<const Profile>> Config::getProfile(const std::string &profileName) const {
  for (const Profile &profile : profiles) {
    if (profile.name == profileName) {
      return std::ref(profile);
    }
  }
  Logger::error("profile \"" + profileName + "\" nyot found");
  return std::nullopt;
}

void Config::reset() {
  profiles.clear();
  currentProfileName = "";
  activationRegion = nullptr;
  activationTimeMilliseconds = 1000;
}

void initializeLuaScriptingApi(VirtualKeyboard &virtualKeyboard, Config &config, const std::string &configFilePath) {
  int numProfiles = 0;

  luaContext.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
  luaContext["VERSION"] = "1.0.0";

  for (int i = 0; i < numKeyDefinitions; ++i) {
    luaContext[keyDefinitions[i].name] = keyDefinitions[i].value;
  }
  // clang-format off
  luaContext.new_usertype<Vector2D>(
    "Vector2D",
    sol::factories(sol::overload(
      []() { return Vector2D(); },
      [](int val) { return Vector2D(val, val); },
      [](int x, int y) { return Vector2D(x, y); },
      [](sol::table args) { 
          return Vector2D(args.get_or("x", 0), args.get_or("y", 0)); 
      }
    )),
    "x", &Vector2D::x,
    "y", &Vector2D::y,
    "setX", &Vector2D::setX,
    "setY", &Vector2D::setY,
    "setBoth", &Vector2D::setBoth,
    "moveX", &Vector2D::moveX,
    "moveY", &Vector2D::moveY,
    "moveBoth", &Vector2D::moveBoth,
    "getX", &Vector2D::getX,
    "getY", &Vector2D::getY,
    "distanceTo", &Vector2D::distanceTo,
    "toString", &Vector2D::toString,
    sol::meta_function::addition, sol::overload(
      [](const Vector2D& lhs, const Vector2D& rhs) { return lhs + rhs; },
      [](const Vector2D& lhs, int rhs) { return lhs + rhs; }
    ),
    sol::meta_function::subtraction, sol::overload(
      [](const Vector2D& lhs, const Vector2D& rhs) { return lhs - rhs; },
      [](const Vector2D& lhs, int rhs) { return lhs - rhs; }
    ),
    sol::meta_function::multiplication, sol::overload(
      [](const Vector2D& lhs, const Vector2D& rhs) { return lhs * rhs; },
      [](const Vector2D& lhs, int rhs) { return lhs * rhs; }
    ),
    sol::meta_function::division, sol::overload(
      [](const Vector2D& lhs, const Vector2D& rhs) { return lhs / rhs; },
      [](const Vector2D& lhs, int rhs) { return lhs / rhs; }
    ),
    sol::meta_function::equal_to, &Vector2D::operator==,
    sol::meta_function::to_string, &Vector2D::toString
  );
  luaContext.new_usertype<Shape>(
    "Shape", 
    sol::no_constructor,
    "position", &Shape::position,
    "dimensions", &Shape::dimensions,
    "pivote", &Shape::pivote,
    "getPosition", &Shape::getPosition,
    "getDimensions", &Shape::getDimensions,
    "contains", &Shape::contains,
    sol::meta_function::to_string, &Shape::toString
  );
  luaContext.new_usertype<Rectangle>(
    "Rectangle",
    sol::factories(sol::overload(
      []() { return Rectangle(); },
      [](const Vector2D &position, const Vector2D &dimensions, Pivote pivote = CORNER_TOP_LEFT) { return Rectangle(position, dimensions, pivote); },
      [](sol::table args) { return Rectangle(args.get<Vector2D>("position"), args.get<Vector2D>("dimensions"), args.get_or("pivote", CORNER_TOP_LEFT)); }
    )),
    sol::base_classes, sol::bases<Shape>()
  );
  luaContext.new_usertype<Circle>(
    "Circle",
    sol::factories(sol::overload(
      []() { return Circle(); },
      [](const Vector2D &position, float radius) { return Circle(position, radius); },
      [](sol::table args) { return Circle(args.get<Vector2D>("position"), args.get<float>("radius")); }
    )),
    "radius", &Circle::radius,
    sol::base_classes, sol::bases<Shape>()
  );
  // clang-format on
  /* Helper Functions */
  luaContext.set_function("vec2", []() { return Vector2D(); });
  luaContext.set_function("vec2", [](float value) { return Vector2D(value); });
  luaContext.set_function("vec2", [](float x, float y) { return Vector2D(x, y); });

  luaContext.set_function("rect", [](sol::table args) -> Rectangle {
    return Rectangle(args.get<Vector2D>("position"), args.get<Vector2D>("dimensions"), args.get_or("pivote", CORNER_TOP_LEFT));
  });
  luaContext.set_function("circle", [](sol::table args) -> Circle { return Circle(args.get<Vector2D>("position"), args.get<float>("radius")); });

  luaContext.set_function("region", [](sol::table args) -> std::optional<Region> {
    int holdDuration = args.get_or("holdDuration", -1);
    bool performAfterRelease = args.get_or("performAfterRelease", false);

    if (!args["shape"].valid()) {
      Logger::error("Region shape nyot specified");
      return std::nullopt;
    }

    Shape *baseShape = args.get<Shape *>("shape");
    if (!baseShape) {
      Logger::error("Shape nyot specified in region");
      return std::nullopt;
    }

    if (!args["action"].valid()) {
      Logger::error("Action nyot specified in region");
      return std::nullopt;
    }

    if (args["action"].get_type() == sol::type::function) {
      std::shared_ptr<Shape> clonedShape = baseShape->clone();
      return Region(clonedShape, holdDuration, performAfterRelease, args.get<std::function<void()>>("action"));
    } else {
      Logger::error("Action in the region is nyot a function");
      return std::nullopt;
    }
  });

  luaContext.set_function("activation", [&config, &configFilePath](sol::table args) {
    if (!args["shape"].valid()) {
      Logger::error("Activation shape nyot specified");
    }

    Shape *baseShape = args.get<Shape *>("shape");
    if (!baseShape) {
      Logger::error("Shape nyot specified in activation");
    }
    config.activationRegion = baseShape ? baseShape->clone() : nullptr;
    config.activationTimeMilliseconds = args.get_or("holdDuration", 1000);
    config.idleTimeoutSeconds = args.get_or("idleTimeoutSeconds", 30);
    config.toggleOffOnIdle = args.get_or("toggleOffOnIdle", false);
    if (args["soundWhenEnabled"].valid()) {
      Logger::check("soundWhenEnabled: " + args.get<std::string>("soundWhenEnabled"));
      config.numpadEnabledBellSoundPath = getCanonicalPathRelativeTo(args.get<std::string>("soundWhenEnabled"), configFilePath);
    } else {
      config.numpadEnabledBellSoundPath = "";
    }
    if (args["soundWhenDisabled"].valid()) {
      config.numpadDisabledBellSoundPath = getCanonicalPathRelativeTo(args.get<std::string>("soundWhenDisabled"), configFilePath);
    } else {
      config.numpadDisabledBellSoundPath = "";
    }
  });

  luaContext.set_function("keyPress", [&virtualKeyboard](int keyCode) -> std::optional<std::function<void()>> {
    // Logger::check("Sending key press for key code: " + std::to_string(keyCode));
    if (isLuaRuntimeExecuting) {
      // Logger::check("sending code directly");
      if (isKeypadKey(keyCode)) {
        // Logger::log("Turning numlock on for a brief moment");
        virtualKeyboard.keyPress(KEY_NUMLOCK);
        virtualKeyboard.keyPress(keyCode);
        virtualKeyboard.keyPress(KEY_NUMLOCK);
      } else {
        virtualKeyboard.keyPress(keyCode);
      }
      return std::nullopt;
    } else {
      // Logger::check("Returning Lambda function for keyPress action");
      return [&virtualKeyboard, keyCode]() {
        if (isKeypadKey(keyCode)) {
          // Logger::log("Turning numlock on for a brief moment");
          virtualKeyboard.keyPress(KEY_NUMLOCK);
          virtualKeyboard.keyPress(keyCode);
          virtualKeyboard.keyPress(KEY_NUMLOCK);
        } else {
          virtualKeyboard.keyPress(keyCode);
        }
      };
    }
  });
  luaContext.set_function("execute", [&](const std::string &command) -> std::optional<std::function<void()>> {
    if (isLuaRuntimeExecuting) {
      launchAndDisownChild(command.c_str());
      return std::nullopt;
    } else {
      return [command]() { launchAndDisownChild(command.c_str()); };
    }
  });
  luaContext.set_function("useProfile", [&config](const std::string &profileName) -> std::optional<std::function<void()>> {
    if (isLuaRuntimeExecuting) {
      config.useProfile(profileName);
      return std::nullopt;
    } else {
      return [&config, profileName]() { config.useProfile(profileName); };
    }
  });

  luaContext.set_function("profile", [&config](sol::table args) {
    if (!args["name"].valid()) {
      Logger::warning("Profile name nyot specified");
      return;
    }

    std::string profileName = args["name"].get<std::string>();
    if (profileName == "touchpad") {
      Logger::error("Cannot use the name \"touchpad\" for a profile as it is a reserved profile name");
    }

    if (!args["regions"].valid()) {
      Logger::warning("Touch regions for profile \"" + profileName + "\" nyot specified");
      return;
    }

    Profile profile;
    profile.name = profileName;
    profile.regions = args.get<std::vector<Region>>("regions");

    config.profiles.push_back(profile);

    if (config.profiles.size() == 1) {
      config.useProfile(profileName);
    }
  });
}

void ConfigWatcher::loadConfig(const std::string &configFilePath, Config &config) {
  MemoryMappedFile configFile(configFilePath.c_str());
  if (!configFile.isValid()) {
    Logger::error("Failed to memory map config file");
    return;
  }

  config.reset();
  isLuaRuntimeExecuting = false;

  sol::load_result script = luaContext.load(configFile.peekIn());
  if (!script.valid()) {
    Logger::error("Failed to load config file");
    return;
  }

  sol::protected_function_result result = script();
  if (!result.valid()) {
    sol::error error = result;
    Logger::error("Failed to execute config file: " + std::string(error.what()));
    return;
  }

  Logger::success("Successfully " + std::string(firstRun ? "loaded" : "reloaded") + " config");

  isLuaRuntimeExecuting = true;

  firstRun = false;
}

void ConfigWatcher::stopWatching() { shouldWatchFile.store(false); }

void ConfigWatcher::watchConfigFile(const std::string &configFilePath, Config &config) {
  shouldWatchFile.store(true);
  int watcherFileDescriptor = inotify_init();

  if (watcherFileDescriptor < 0) {
    Logger::error("Failed to initialize index node notifier instance");
    return;
  }

  std::filesystem::path configPath(configFilePath);
  std::filesystem::path parentDir = configPath.parent_path();
  std::string filename = configPath.filename().string();

  int watchDescriptor = inotify_add_watch(watcherFileDescriptor, parentDir.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO);
  if (watchDescriptor < 0) {
    Logger::error("Failed to add watch on config directory");
    close(watcherFileDescriptor);
    return;
  }

  const size_t bufferSize = sizeof(struct inotify_event) + 256;
  char buffer[bufferSize];

  struct pollfd pollFileDescriptor;
  pollFileDescriptor.fd = watcherFileDescriptor;
  pollFileDescriptor.events = POLLIN;

  while (shouldWatchFile.load()) {
    int pollReturnCode = poll(&pollFileDescriptor, 1, 500);
    if (pollReturnCode == -1) {
      if (shouldWatchFile.load()) {
        Logger::error("Config watcher polling failed");
      } else {
        Logger::info("Config watcher polling stopped");
        break;
      }
    }
    if (pollReturnCode > 0 && (pollFileDescriptor.revents & POLLIN)) {
      int eventDataLength = read(watcherFileDescriptor, buffer, bufferSize);
      if (eventDataLength > 0) {
        bool matched = false;
        int i = 0;
        while (i < eventDataLength) {
          struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buffer[i]);
          if (event->len > 0) {
            if (std::string(event->name) == filename) {
              matched = true;
              break;
            }
          }
          i += sizeof(struct inotify_event) + event->len;
        }

        if (matched) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          Logger::info("Config file changed, reloading...");
          loadConfig(configFilePath, config);
          if (config.activationRegion) {
            Logger::check("Activation Region -> " + config.activationRegion->toString());
          }
          Logger::check("Activation Time -> " + std::to_string(config.activationTimeMilliseconds));
        }
      }
    }
  }

  inotify_rm_watch(watcherFileDescriptor, watchDescriptor);
  close(watcherFileDescriptor);
  Logger::info("Stopped watching config file");
}