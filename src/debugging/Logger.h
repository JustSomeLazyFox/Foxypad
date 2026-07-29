#pragma once

#include <string>

class Logger {
public:
  static void log(const std::string &message);
  static void info(const std::string &message);
  static void error(const std::string &message);
  static void warning(const std::string &message);
  static void success(const std::string &message);
  static void check(const std::string &message);
  static void test(bool condition, const std::string &messageOnPass = "", const std::string &messageOnFail = "");
};

// clang-format off
enum AnsiColor { 
  BLACK = 30, 
  RED = 31, 
  GREEN = 32, 
  YELLOW = 33, 
  BLUE = 34, 
  MAGENTA = 35, 
  CYAN = 36, 
  WHITE = 97, 
  DEFAULT = 39 
};
// clang-format on

class AnsiConstructor {
private:
  std::string out;

public:
  AnsiConstructor();
  ~AnsiConstructor();
  AnsiConstructor &text(const std::string &text);
  AnsiConstructor &reset();
  AnsiConstructor &bold();
  AnsiConstructor &unbold();
  AnsiConstructor &italicize();
  AnsiConstructor &unitalicize();
  AnsiConstructor &underline();
  AnsiConstructor &ununderline();
  AnsiConstructor &foreground(AnsiColor);
  AnsiConstructor &background(AnsiColor);
  AnsiConstructor &foreground(int r, int g, int b);
  AnsiConstructor &background(int r, int g, int b);
  std::string construct() const;
};