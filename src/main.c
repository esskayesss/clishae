#include <stdlib.h>
#include "server/server.h"
#include "utils/logging.h"
#include <signal.h>

void handle_sigpipe(int sig) {
  LOG_INFO("SIGPIPE");
}

int
main(){
  signal(SIGPIPE, handle_sigpipe);
  initialize_logger(NULL, LEVEL_TRACE);
  if(initialize_server(8080)){
    LOG_FATAL("could not start server.");
    exit(EXIT_FAILURE);
  }
  server_listen();
}
