#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "logging.h"

#define ERRMSG    strerror(errno)

#define handle_err(format, ...) do {\
  LOG_ERROR(format, ##__VA_ARGS__);\
  return 1;\
} while(0);

// TODO: kill thread instead of the entire process
#define handle_fatal(format, ...) do {\
  LOG_FATAL(format, ##__VA_ARGS__);\
  exit(EXIT_FAILURE);\
} while(0);

#endif
