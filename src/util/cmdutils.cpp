#include "cmdutils.h"

#include "../debugging/Logger.h"

#include <cstdlib>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

void launchAndDisownChild(std::string command) {
  pid_t child = fork();

  if (child < 0) {
    Logger::error("Failed to fork");
    return;
  }

  if (child == 0) {
    setsid();

    pid_t grandchild = fork();

    if (grandchild == 0) {
      int theVoid = open("/dev/null", O_RDWR);
      if (theVoid != -1) {
        dup2(theVoid, STDIN_FILENO);
        dup2(theVoid, STDOUT_FILENO);
        dup2(theVoid, STDERR_FILENO);
      }

      const char *userShell = std::getenv("SHELL");

      if (userShell == nullptr || userShell[0] == '\0') {
        userShell = "/bin/sh";
      }

      execl(userShell, userShell, "-c", command.c_str(), nullptr);
      _exit(1);
    }

    _exit(0);
  }

  waitpid(child, nullptr, 0);
}