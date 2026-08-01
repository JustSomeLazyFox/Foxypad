#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

class Logger {
private:
  inline static int nestedGroupLevel = 0;
  inline static bool insideGroupLoggingScope = false;
  inline static bool hasJustExitedGroupLoggingScope = false;
  inline static std::chrono::time_point<std::chrono::steady_clock> lastLogTime = std::chrono::steady_clock::time_point::min();
  inline static std::unordered_map<int, std::vector<std::string>> logQueues;
  static void queueLog(const std::string &text, bool ignoreRateLimit = false);

public:
  static void startGroupLogging() {
    insideGroupLoggingScope = true;
    hasJustExitedGroupLoggingScope = false;
    nestedGroupLevel++;
  }
  static void endGroupLogging() {
    insideGroupLoggingScope = false;
    hasJustExitedGroupLoggingScope = true;
    nestedGroupLevel--;
  }
  static bool isInsideGroupLoggingScope() { return insideGroupLoggingScope; }
  static void log(const std::string &message, bool ignoreLogLevel = false, bool ignoreRateLimit = false);
  static void info(const std::string &message, bool ignoreLogLevel = false, bool ignoreRateLimit = false);
  static void error(const std::string &message, bool ignoreLogLevel = false, bool ignoreRateLimit = false);
  static void warning(const std::string &message, bool ignoreLogLevel = false, bool ignoreRateLimit = false);
  static void success(const std::string &message, bool ignoreLogLevel = false, bool ignoreRateLimit = false);
  static void check(const std::string &message, bool ignoreLogLevel = false, bool ignoreRateLimit = false);
  static void test(bool condition, const std::string &messageOnPass = "", const std::string &messageOnFail = "", bool ignoreLogLevel = false,
                   bool ignoreRateLimit = false);
  static void severe(const std::string &message);
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
  DEFAULT = 39,
  BRIGHT_BLUE = 94,
  BRIGHT_MAGENTA = 95
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
  AnsiConstructor &foreground(int color);
  AnsiConstructor &background(int color);
  AnsiConstructor &foreground(int r, int g, int b);
  AnsiConstructor &background(int r, int g, int b);
  std::string construct() const;
};