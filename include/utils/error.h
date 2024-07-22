#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <pthread.h>
#include "logging.h"

#define handle_err(format, ...) do {\
  LOG_ERROR(format, ...);\
  return 1;\
} while(0);

// TODO: kill thread instead of the entire process
#define handle_fatal(format, ...) do {\
  LOG_FATAL(format, ...);\
  exit(EXIT_FAILURE);\
} while(0);

#endif
