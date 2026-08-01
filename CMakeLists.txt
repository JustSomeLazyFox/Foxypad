cmake_minimum_required(VERSION 3.10)
project(
  foxypad
  VERSION 1.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(EVDEV REQUIRED IMPORTED_TARGET libevdev)
pkg_check_modules(UDEV REQUIRED IMPORTED_TARGET libudev)
pkg_check_modules(LUA REQUIRED IMPORTED_TARGET lua5.4)
pkg_check_modules(SOL2 IMPORTED_TARGET sol2)

file(GLOB_RECURSE APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} main.cpp ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
  PkgConfig::EVDEV
  PkgConfig::UDEV
  PkgConfig::LUA
  PkgConfig::SOL2
)
