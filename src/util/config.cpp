#include "config.h"

#include "../debugging/Logger.h"
#include "shape.h"
#include "vector.h"

#include <atomic>
#include <fstream>
#include <shared_mutex>
#include <sol/inheritance.hpp>
#include <sol/raii.hpp>
#include <sol/sol.hpp>
#include <sol/state.hpp>
#include <sol/table.hpp>
#include <sol/types.hpp>
#include <string>
#include <sys/inotify.h>
#include <thread>
#include <unistd.h>

Config *currentConfig = nullptr;

sol::state luaContext;
std::shared_mutex numpadLayoutMutex;
std::atomic_bool ConfigWatcher::shouldWatchFile(false);

bool firstRun = true;

void initializeLuaScriptingApi() {
  luaContext.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
  luaContext["VERSION"] = "1.0.0";
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

  luaContext.set_function("rect", [](sol::table args) {
    return Rectangle(args.get<Vector2D>("position"), args.get<Vector2D>("dimensions"), args.get_or("pivote", CORNER_TOP_LEFT));
  });
  luaContext.set_function("circle", [](sol::table args) { return Circle(args.get<Vector2D>("position"), args.get<float>("radius")); });

  luaContext.set_function("keyPress", [](sol::table args) {
    if (args["keyCode"].valid()) {
      return KeyPressDescription(args.get<int>("keyCode"), args.get_or("hold", -1));
    }
    return KeyPressDescription(-1, -1);
  });
}

void ConfigWatcher::loadConfig(std::string configFilePath) {
  std::ifstream inputFileStream(configFilePath);
  if (!inputFileStream.is_open()) {
    Logger::error("Failed to open config file");
    return;
  }

  Config *newConfig = new Config();

  numpadLayoutMutex.lock();
  numpadLayoutMutex.unlock();
  firstRun = false;
}

void ConfigWatcher::stopWatching() { shouldWatchFile.store(false); }

void ConfigWatcher::watchConfigFile(const std::string configFilePath) {
  shouldWatchFile.store(true);
  int watcherFileDescriptor = inotify_init();

  if (watcherFileDescriptor < 0) {
    Logger::error("Failed to initialize index node notifier instance");
    return;
  }

  int watchDescriptor = inotify_add_watch(watcherFileDescriptor, configFilePath.c_str(), IN_MODIFY);
  if (watchDescriptor < 0) {
    Logger::error("Failed to add watch on config file");
    return;
  }

  const size_t bufferSize = sizeof(struct inotify_event) + 256;
  char buffer[bufferSize];

  while (shouldWatchFile.load()) {
    int eventDataLength = read(watcherFileDescriptor, buffer, bufferSize);
    if (eventDataLength > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      loadConfig(configFilePath);
    }
  }
}