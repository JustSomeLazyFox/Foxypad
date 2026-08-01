#pragma once

#include "../debugging/Logger.h"
#include "mathutils.h"

#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

void launchAndDisownChild(std::string command);

enum CommandlineArgumentType { BOOLEANISH, BOOLEAN, STRING, NON_WHITESPACE_STRING, INTEGER };

class CommandlineArgument {
private:
  std::string fullName;
  char shortName;
  std::string description;
  CommandlineArgumentType type;
  std::function<void(std::variant<std::string, bool, int>)> callback;

public:
  CommandlineArgument(std::string fullName, char shortName, std::string description, CommandlineArgumentType type, std::function<void()> callback) {
    this->fullName = fullName;
    this->shortName = shortName;
    this->description = description;
    this->type = type;
    this->callback = [callback](std::variant<std::string, bool, int>) { callback(); };
  }
  CommandlineArgument(std::string fullName, char shortName, std::string description, CommandlineArgumentType type,
                      std::function<void(std::string)> callback) {
    this->fullName = fullName;
    this->shortName = shortName;
    this->description = description;
    this->type = type;
    this->callback = [callback, type, fullName](std::variant<std::string, bool, int> val) {
      if (type == NON_WHITESPACE_STRING) {
        if (std::get<std::string>(val).find(' ') != std::string::npos) {
          Logger::error("Argument " + fullName + " does not support whitespaces.");
          return;
        }
      }
      callback(std::get<std::string>(val));
    };
  }
  CommandlineArgument(std::string fullName, char shortName, std::string description, CommandlineArgumentType type,
                      std::function<void(bool)> callback) {
    this->fullName = fullName;
    this->shortName = shortName;
    this->description = description;
    this->type = type;
    this->callback = [callback](std::variant<std::string, bool, int> val) {
      if (auto pVal = std::get_if<bool>(&val)) {
        callback(*pVal);
      } else if (auto pStr = std::get_if<std::string>(&val)) {
        callback(*pStr == "true" || *pStr == "1");
      } else if (auto pInt = std::get_if<int>(&val)) {
        callback(*pInt != 0);
      } else {
        callback(false);
      }
    };
  }
  CommandlineArgument(std::string fullName, char shortName, std::string description, CommandlineArgumentType type,
                      std::function<void(int)> callback) {
    this->fullName = fullName;
    this->shortName = shortName;
    this->description = description;
    this->type = type;
    this->callback = [callback](std::variant<std::string, bool, int> val) {
      if (auto pInt = std::get_if<int>(&val)) {
        callback(*pInt);
      } else if (auto pStr = std::get_if<std::string>(&val)) {
        callback(std::stoi(*pStr));
      } else if (auto pVal = std::get_if<bool>(&val)) {
        callback(*pVal ? 1 : 0);
      }
    };
  }

  const std::string &getFullName() const { return fullName; }
  char getShortName() const { return shortName; }
  const std::string &getDescription() const { return description; }
  const CommandlineArgumentType &getType() const { return type; }
  void applyCallback(std::variant<std::string, bool, int> val) const { callback(val); }
};

class CommandlineParser {
private:
  std::vector<CommandlineArgument> arguments;
  std::string applicationDescription;
  std::string applicationUsage;
  std::string applicationVersion;

public:
  void setApplicationDescription(const std::string &applicationDescription) { this->applicationDescription = applicationDescription; }
  void setApplicationUsage(const std::string &applicationUsage) { this->applicationUsage = applicationUsage; }
  void setApplicationVersion(const std::string &applicationVersion) { this->applicationVersion = applicationVersion; }
  void addArgument(CommandlineArgument arg) { arguments.push_back(arg); }

  static std::string getArgumentsAsCsv(int argc, char **argv) {
    std::string result = "[";
    for (int i = 0; i < argc; i++) {
      result += argv[i];
      result += ", ";
    }
    result.pop_back();
    result.pop_back();
    result += "]";
    return result;
  }

  void parse(int argc, char **argv) {
    Logger::check("All Args: " + getArgumentsAsCsv(argc, argv));
    Logger::check(" argc: " + std::to_string(argc));
    for (int i = 1; i < argc; i++) {
      Logger::info("Parsing arg: " + std::string(argv[i]));
      std::string arg = argv[i];
      if (arg.starts_with("--")) {
        arg = arg.replace(0, 2, "");
        for (size_t j = 0; j < arguments.size(); j++) {
          CommandlineArgument argument = arguments[j];
          if (arg == argument.getFullName()) {
            if (argument.getType() == BOOLEANISH) {
              argument.applyCallback(true);
            } else if (argument.getType() == BOOLEAN) {
              if (i + 1 < argc) {
                argument.applyCallback(equalsIgnoreCase(std::string(argv[++i]), "true"));
              } else {
                argument.applyCallback(false);
              }
            } else if (argument.getType() == STRING || argument.getType() == NON_WHITESPACE_STRING || argument.getType() == INTEGER) {
              if (i + 1 < argc) {
                argument.applyCallback(std::string(argv[++i]));
              } else {
                argument.applyCallback(std::string(""));
              }
            }
            goto nextArg;
          }
        }
      } else if (arg.starts_with("-")) {
        arg = arg.replace(0, 1, "");
        Logger::check("arg: " + arg);
        for (size_t j = 0; j < arguments.size(); j++) {
          CommandlineArgument argument = arguments[j];
          if (arg[0] == argument.getShortName()) {
            Logger::check("Short arg is: " + std::string(1, argument.getShortName()));
            Logger::check("Arg value is: " + arg.substr(1));
            if (argument.getType() == BOOLEANISH) {
              argument.applyCallback(true);
            } else if (argument.getType() == BOOLEAN) {
              argument.applyCallback(equalsIgnoreCase(arg.substr(1), "true"));
            } else if (argument.getType() == STRING || argument.getType() == NON_WHITESPACE_STRING || argument.getType() == INTEGER) {
              argument.applyCallback(arg.substr(1));
            }
            goto nextArg;
          }
        }
      }
      if (arg == "help" || arg == "h") {
        Logger::info("Displaying help message");
        std::string helpMessage = "";
        helpMessage += AnsiConstructor().foreground(AnsiColor::MAGENTA).text(applicationDescription).construct() + "\n";
        helpMessage += AnsiConstructor().bold().foreground(AnsiColor::CYAN).text("Usage: ").unbold().text(applicationUsage).construct() + "\n";
        helpMessage += AnsiConstructor().bold().foreground(AnsiColor::WHITE).text("Version: ").unbold().text(applicationVersion).construct() + "\n";
        helpMessage += AnsiConstructor().bold().foreground(AnsiColor::GREEN).text("Arguments:\n").construct();

        for (CommandlineArgument argument : arguments) {
          helpMessage += AnsiConstructor()
                             .foreground(GREEN)
                             .text("\t-" + std::string(1, argument.getShortName()))
                             .foreground(YELLOW)
                             .text("\t--" + argument.getFullName())
                             .foreground(BLUE)
                             .text("\t" + argument.getDescription() + "\n")
                             .construct();
        }
        std::cout << helpMessage << std::endl;
        exit(0);
      }

      if (arg == "version" || arg == "v") {
        std::string versionMessage = AnsiConstructor()
                                         .bold()
                                         .foreground(WHITE)
                                         .text(std::string(std::filesystem::path(argv[0]).filename()) + ": ")
                                         .unbold()
                                         .foreground(GREEN)
                                         .text("v")
                                         .foreground(CYAN)
                                         .text(applicationVersion)
                                         .construct();
        std::cout << versionMessage << std::endl;
        exit(0);
      }
      Logger::warning("Unrecognized argument: " + arg);
    nextArg:
      continue;
    }
  }
};

// CommandlineArgument *parseCommandLine(int argc, char **argv);