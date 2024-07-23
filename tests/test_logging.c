#include <stdio.h>
#include "utils/logging.h"

int
main() {
  initialize_logger(NULL, LEVEL_TRACE);
  LOG_TRACE("this is a trace log");
  LOG_DEBUG("this is a debug log");
  LOG_INFO("this is an info log");
  LOG_WARN("this is a warning log");
  LOG_ERROR("this is an error log");
  LOG_FATAL("this is a fatal log");
  LOG_SUCCESS("this is a success log");
}
