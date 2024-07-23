#ifndef LOGGING_H
#define LOGGING_H

#define LOG_TRACE(format, ...) log_message(LEVEL_TRACE, __func__, __LINE__, format, ##__VA_ARGS__);
#define LOG_DEBUG(format, ...) log_message(LEVEL_DEBUG, __func__, __LINE__, format, ##__VA_ARGS__);
#define LOG_INFO(format, ...) log_message(LEVEL_INFO, __func__, __LINE__, format, ##__VA_ARGS__);
#define LOG_WARN(format, ...) log_message(LEVEL_WARN, __func__, __LINE__, format, ##__VA_ARGS__);
#define LOG_ERROR(format, ...) log_message(LEVEL_ERROR, __func__, __LINE__, format, ##__VA_ARGS__);
#define LOG_FATAL(format, ...) log_message(LEVEL_FATAL, __func__, __LINE__, format, ##__VA_ARGS__);
#define LOG_SUCCESS(format, ...) log_message(LEVEL_SUCCESS, __func__, __LINE__, format, ##__VA_ARGS__);

typedef enum {
  LEVEL_TRACE,
  LEVEL_DEBUG,
  LEVEL_INFO,
  LEVEL_WARN,
  LEVEL_ERROR,
  LEVEL_FATAL,
  LEVEL_SUCCESS,
} LogLevel;

int initialize_logger(const char* filepath, LogLevel level);
int close_logger(void);
int update_loglevel(LogLevel new_level);
int log_message(LogLevel msg_level, const char *fn_name, const int l_num, const char *msg_format, ...);

#endif
