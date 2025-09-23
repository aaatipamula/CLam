#include <stdio.h>
#include <stdarg.h>

#include "err.h"

const char *level_to_str(LogLevel l) {
  switch (l) {
    case LOG_WARN: return "WARN";
    case LOG_ERROR: return "ERROR";
  }
  return "";
}

void log_msg(LogLevel level, const char *fmt, ...) {
  fprintf(stderr, "CLam [%s]: ", level_to_str(level));

  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

