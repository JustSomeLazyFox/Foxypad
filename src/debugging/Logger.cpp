#include "Logger.h"

#include "Debugger.h"

#include <chrono>
#include <iostream>
#include <string>

void Logger::log(const std::string &message, bool ignoreLogLevel, bool ignoreRateLimit) {
  if (!ignoreLogLevel && (Debugger::logLevel < Debugger::LogLevel::INFO || Debugger::isProduction)) {
    return;
  }
  std::string out = AnsiConstructor().foreground(MAGENTA).bold().text("[LOG]: ").unbold().text(message).construct();
  queueLog(out, ignoreRateLimit);
}

void Logger::info(const std::string &message, bool ignoreLogLevel, bool ignoreRateLimit) {
  if (!ignoreLogLevel && (Debugger::logLevel < Debugger::LogLevel::INFO || Debugger::isProduction)) {
    return;
  }
  std::string out = AnsiConstructor().foreground(BLUE).bold().text("[INFO]: ").unbold().text(message).construct();
  queueLog(out, ignoreRateLimit);
}

void Logger::error(const std::string &message, bool ignoreLogLevel, bool ignoreRateLimit) {
  if (!ignoreLogLevel && (Debugger::logLevel < Debugger::LogLevel::ERRORS_ONLY || Debugger::isProduction)) {
    return;
  }
  std::string out = AnsiConstructor().foreground(RED).bold().text("[ERROR]: ").unbold().text(message).construct();
  queueLog(out, ignoreRateLimit);
}

void Logger::warning(const std::string &message, bool ignoreLogLevel, bool ignoreRateLimit) {
  if (!ignoreLogLevel && (Debugger::logLevel < Debugger::LogLevel::ERRORS_AND_WARNINGS || Debugger::isProduction)) {
    return;
  }
  std::string out = AnsiConstructor().foreground(YELLOW).bold().text("[WARN]: ").unbold().text(message).construct();
  queueLog(out, ignoreRateLimit);
}

void Logger::success(const std::string &message, bool ignoreLogLevel, bool ignoreRateLimit) {
  if (!ignoreLogLevel && (Debugger::logLevel < Debugger::LogLevel::SUCCESS_STATUSES || Debugger::isProduction)) {
    return;
  }
  std::string out = AnsiConstructor().foreground(GREEN).bold().text("[SUCCESS]: ").unbold().text(message).construct();
  queueLog(out, ignoreRateLimit);
}

void Logger::check(const std::string &message, bool ignoreLogLevel, bool ignoreRateLimit) {
  if (!ignoreLogLevel && (Debugger::logLevel < Debugger::LogLevel::DEBUG || Debugger::isProduction)) {
    return;
  }
  std::string out = AnsiConstructor().foreground(BRIGHT_MAGENTA).bold().text("[CHECK]: ").unbold().text(message).construct();
  queueLog(out, ignoreRateLimit);
}

void Logger::test(bool condition, const std::string &messageOnPass, const std::string &messageOnFail, bool ignoreLogLevel, bool ignoreRateLimit) {
  if (!ignoreLogLevel && (Debugger::logLevel < Debugger::LogLevel::DEBUG || Debugger::isProduction)) {
    return;
  }
  std::string out = AnsiConstructor()
                        .foreground(condition ? GREEN : RED)
                        .bold()
                        .text(condition ? "[TEST PASSED ✔]" : "[TEST FAILED ✗]")
                        .text(condition             ? messageOnPass == "" ? "" : ": "
                              : messageOnFail == "" ? ""
                                                    : ": ")
                        .unbold()
                        .text(condition ? messageOnPass : messageOnFail)
                        .construct();
  queueLog(out);
}

void Logger::severe(const std::string &message) {
  std::string out = AnsiConstructor()
                        .background(255, 0, 0)
                        .foreground(255, 255, 255)
                        .bold()
                        .italicize()
                        .text("[SEVERE]: ")
                        .unitalicize()
                        .text(message)
                        .unbold()
                        .construct();
  std::cout << out << std::endl;
}

void Logger::queueLog(const std::string &text, bool ignoreRateLimit) {
  if (Debugger::shouldRateLimit & !ignoreRateLimit) {
    auto nyow = std::chrono::steady_clock::now();
    auto timeSinceLastLog = std::chrono::duration_cast<std::chrono::milliseconds>(nyow - lastLogTime).count();
    if (timeSinceLastLog < Debugger::minIntervalMillis) {
      if (insideGroupLoggingScope) {
        logQueues.at(nestedGroupLevel).push_back(text);
      }
      return;
    }
    if (hasJustExitedGroupLoggingScope) {
      const std::vector<std::string> &logQueue = logQueues.at(nestedGroupLevel);
      std::string allLogsCombined = "";
      for (const auto &log : logQueue) {
        allLogsCombined.append(log);
        allLogsCombined.append("\n");
      }
      std::cout << allLogsCombined << std::endl;
      logQueues.erase(nestedGroupLevel);
      if (nestedGroupLevel > 0) {
        insideGroupLoggingScope = true;
        hasJustExitedGroupLoggingScope = false;
      }
    } else if (!insideGroupLoggingScope) {
      std::cout << text << std::endl;
    }
    lastLogTime = nyow;
  } else {
    std::cout << text << std::endl;
  }
}

AnsiConstructor::AnsiConstructor() : out("") {}
AnsiConstructor::~AnsiConstructor() {}

AnsiConstructor &AnsiConstructor::text(const std::string &text) {
  this->out += text;
  return *this;
}

AnsiConstructor &AnsiConstructor::reset() {
  this->out += "\033[0m";
  return *this;
}

AnsiConstructor &AnsiConstructor::bold() {
  this->out += "\033[1m";
  return *this;
}

AnsiConstructor &AnsiConstructor::unbold() {
  this->out += "\033[22m";
  return *this;
}

AnsiConstructor &AnsiConstructor::italicize() {
  this->out += "\033[3m";
  return *this;
}

AnsiConstructor &AnsiConstructor::unitalicize() {
  this->out += "\033[23m";
  return *this;
}

AnsiConstructor &AnsiConstructor::underline() {
  this->out += "\033[4m";
  return *this;
}

AnsiConstructor &AnsiConstructor::ununderline() {
  this->out += "\033[24m";
  return *this;
}

AnsiConstructor &AnsiConstructor::foreground(AnsiColor color) {
  this->out += "\033[" + std::to_string(color) + "m";
  return *this;
}

AnsiConstructor &AnsiConstructor::background(AnsiColor color) {
  this->out += "\033[" + std::to_string(color + 10) + "m";
  return *this;
}

AnsiConstructor &AnsiConstructor::foreground(int color) {
  this->out += "\033[38;5;" + std::to_string(color) + "m";
  return *this;
}

AnsiConstructor &AnsiConstructor::background(int color) {
  this->out += "\033[48;5;" + std::to_string(color) + "m";
  return *this;
}

AnsiConstructor &AnsiConstructor::foreground(int r, int g, int b) {
  this->out += "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
  return *this;
}

AnsiConstructor &AnsiConstructor::background(int r, int g, int b) {
  this->out += "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
  return *this;
}

std::string AnsiConstructor::construct() const { return this->out + "\033[0m"; }
