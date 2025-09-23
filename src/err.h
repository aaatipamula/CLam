#ifndef CLAM_ERR_H_
#define CLAM_ERR_H_

typedef enum {
  LOG_WARN,
  LOG_ERROR,
} LogLevel;

void log_msg(LogLevel level, const char *fmt, ...);

#define ERR(fmt, ...) log_msg(LOG_ERROR, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...) log_msg(LOG_WARN, fmt, ##__VA_ARGS__)

#endif // CLAM_ERR_H_
