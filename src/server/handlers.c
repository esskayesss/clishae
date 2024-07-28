#include "server/handlers.h"
#include "network/context.h"
#include "network/response.h"
#include "utils/fsutils.h"
#include "utils/logging.h"

int
get_ping(int cfd, Context *ctx){
  return send_response(cfd, 200, "pong");
}

int
post_paste(int cfd, Context *ctx){
  if (ctx->method != HTTP_POST){
    LOG_ERROR("wrong routing. Method should be POST");
    return -1;
  }

  if(!ctx->body.content_length){
    LOG_ERROR("empty body");
    send_response(cfd, RESPONSE_400, "empty body");
  } else{
    char *resp = new_file(ctx->body.content);
    LOG_INFO("new file created with filename: %s", resp);
    send_response(cfd, RESPONSE_201, resp);
    free(resp);
  }
  
  return 0;
}
