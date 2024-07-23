#include <stdarg.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "utils/error.h"
#include "utils/logging.h"

#define COLOR(color)  "\033[38;5;" #color "m"
#define RESET_COLOR   "\033[0m"


static FILE*    log_file = NULL;
static LogLevel current_level = LEVEL_INFO;


typedef struct {
  const char *level_str, *color;
} LevelStyle;

static const LevelStyle styles[] = {
  [LEVEL_TRACE]   = {"TRACE",   COLOR(4)},
  [LEVEL_DEBUG]   = {"DEBUG",   COLOR(7)},
  [LEVEL_INFO]    = {"INFO",    COLOR(3)},
  [LEVEL_WARN]    = {"WARN",    COLOR(11)},
  [LEVEL_ERROR]   = {"ERROR",   COLOR(1)},
  [LEVEL_FATAL]   = {"FATAL",   COLOR(9)},
  [LEVEL_SUCCESS] = {"SUCCESS", COLOR(10)},
};


int
initialize_logger(const char *filepath, LogLevel level){
  if(filepath){
    log_file = fopen(filepath, "w");
    if(!log_file){
      handle_fatal("fopen - could not open file at %s: %s", filepath, strerror(errno));
    }
  }

  current_level = level;
  return 0;
}

int
close_logger(void) {
  if (fclose(log_file) != 0)
    handle_err("fclose - could not close opened file at %s: %s", log_file, strerror(errno));

  return 0;
}

int
update_loglevel(LogLevel new_level) {
  current_level = new_level;
  return 0;
}

int
log_message(LogLevel msg_level, const char *fn_name, const int l_num, const char *msg_format, ...) {
  if (msg_level < current_level)
    return 0;

  if(!msg_format)
    LOG_ERROR("log message called by %s with empty message format!", fn_name);

  time_t now = time(NULL);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

  va_list args;
  va_start(args, msg_format);

  fprintf(stderr, "%s[%7s - %s] %s[%d] - ", styles[msg_level].color, styles[msg_level].level_str, timestamp, fn_name, l_num);
  vfprintf(stderr, msg_format, args);
  fprintf(stderr, RESET_COLOR "\n");

  if(log_file){
    fprintf(log_file, "[%7s - %s] %s[%d] - ", styles[msg_level].level_str, timestamp, fn_name, l_num);
    vfprintf(log_file, msg_format, args);
    fprintf(log_file, RESET_COLOR "\n");
  }

  va_end(args);
  return 0;
}
