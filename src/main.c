#include <stdlib.h>
#include "server/server.h"
#include "utils/logging.h"

int
main(){
  initialize_logger(NULL, LEVEL_INFO);
  if(initialize_server(8080)){
    LOG_FATAL("could not start server.");
    exit(EXIT_FAILURE);
  }
  server_listen();
}
