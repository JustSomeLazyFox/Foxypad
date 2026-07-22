#pragma once

#include <string>

class Logger {
public:
  static void log(std::string message);
  static void info(std::string message);
  static void error(std::string message);
  static void warning(std::string message);
  static void success(std::string message);
  static void check(std::string message);
  static void test(bool condition, std::string messageOnPass = "",
                   std::string messageOnFail = "");
};

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

class AnsiConstructor {
private:
  std::string out;

public:
  AnsiConstructor();
  ~AnsiConstructor();
  AnsiConstructor &text(std::string);
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