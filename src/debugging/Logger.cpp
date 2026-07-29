#include "Logger.h"

#include <iostream>
#include <string>

void Logger::log(const std::string &message) {
  std::string out = AnsiConstructor().foreground(MAGENTA).bold().text("[LOG]: ").unbold().text(message).construct();
  std::cout << out << std::endl;
}

void Logger::info(const std::string &message) {
  std::string out = AnsiConstructor().foreground(BLUE).bold().text("[INFO]: ").unbold().text(message).construct();
  std::cout << out << std::endl;
}

void Logger::error(const std::string &message) {
  std::string out = AnsiConstructor().foreground(RED).bold().text("[ERROR]: ").unbold().text(message).construct();
  std::cout << out << std::endl;
}

void Logger::warning(const std::string &message) {
  std::string out = AnsiConstructor().foreground(YELLOW).bold().text("[WARN]: ").unbold().text(message).construct();
  std::cout << out << std::endl;
}

void Logger::success(const std::string &message) {
  std::string out = AnsiConstructor().foreground(GREEN).bold().text("[SUCCESS]: ").unbold().text(message).construct();
  std::cout << out << std::endl;
}

void Logger::check(const std::string &message) {
  std::string out = AnsiConstructor().foreground(128, 0, 200).bold().text("[CHECK]: ").unbold().text(message).construct();
  std::cout << out << std::endl;
}

void Logger::test(bool condition, const std::string &messageOnPass, const std::string &messageOnFail) {
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
  std::cout << out << std::endl;
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

AnsiConstructor &AnsiConstructor::foreground(int r, int g, int b) {
  this->out += "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
  return *this;
}

AnsiConstructor &AnsiConstructor::background(int r, int g, int b) {
  this->out += "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
  return *this;
}

std::string AnsiConstructor::construct() const { return this->out + "\033[0m"; }
