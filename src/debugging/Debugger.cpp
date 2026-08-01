#include "Debugger.h"

#include <cstdint>

namespace Debugger {
bool isProduction = false;
LogLevel logLevel = LogLevel::DEBUG;
uint64_t minIntervalMillis = 1000;
bool shouldRateLimit = false;
void rateLimit() { shouldRateLimit = true; }
void rateLimit(uint64_t minIntervalMillis) {
  Debugger::minIntervalMillis = minIntervalMillis;
  shouldRateLimit = true;
}
void disableRateLimit() { shouldRateLimit = false; }
void enableLogging() { isProduction = false; }
void disableLogging() { isProduction = true; }
void setLogLevel(LogLevel level) { logLevel = level; }
} // namespace Debugger