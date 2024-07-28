#include <stdio.h>
#include <string.h>
#include "server/routes.h"
#include "network/context.h"
#include "network/response.h"
#include "server/handlers.h"
#include "utils/fsutils.h"
#include "utils/logging.h"

int
route(int cfd, Context *ctx){
  char *route = ctx->route;
  char path[256];

  if (ctx->method == HTTP_GET){
    if(strcmp("/favicon.ico", route) == 0){
      LOG_INFO("serving favicon.ico");
      serve_file(cfd, RESPONSE_200, "./public/favicon.ico");
    } else if(strstr(route, "/ping") == route){
      LOG_INFO("pong haha");
      get_ping(cfd, ctx);
    } else if (strstr(route, "/public/") == route) {
      LOG_INFO("serving public file");
      snprintf(path, 256, ".%s", route);
      serve_file(cfd, RESPONSE_200, path);
    } else if (strstr(route, "/raw/") == route){
      LOG_INFO("serving raw paste");
      snprintf(path, 256, "/tmp/clishae%s", route);
      serve_file(cfd, RESPONSE_200, path);
    }
  } else if (ctx->method == HTTP_POST) {
    if(strstr(route, "/paste") == route){
      LOG_INFO("creating new paste");
      post_paste(cfd, ctx);
    }
  } else{ // DELETE method
    if(strstr(route, "/paste/") == route){
      snprintf(path, 256, "/tmp/clishae/raw%s", (route+6));
      delete_file(path);
      send_response(cfd, RESPONSE_200, "paste was deleted successfully\n");
    }
  }

  return 0;
}
