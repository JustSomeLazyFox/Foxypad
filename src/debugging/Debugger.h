#pragma once

#include <cstdint>

namespace Debugger {
enum LogLevel { ERRORS_ONLY, ERRORS_AND_WARNINGS, SUCCESS_STATUSES, INFO, DEBUG };
extern bool isProduction;
extern LogLevel logLevel;
extern bool shouldRateLimit;
extern uint64_t minIntervalMillis;
void rateLimit();
void rateLimit(uint64_t minIntervalMillis);
void disableRateLimit();
void enableLogging();
void disableLogging();
void setLogLevel(LogLevel level);
} // namespace Debugger