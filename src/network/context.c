#include <string.h>
#include <stdlib.h>
#include "network/headers.h"
#include "utils/error.h"
#include "utils/logging.h"
#include "network/context.h"


Context *
parse_context(const char *request){
  Context *ctx = (Context *)malloc(sizeof(Context));
  memset(ctx, 0, sizeof(Context));
  const char *crlf = "\r\n";

  char *checkpoint = strstr(request, " ");
  const char *method = strndup(request, checkpoint - request);
  LOG_DEBUG("Method: %s", method);
  if (!strcmp(method, "GET"))
    ctx->method = HTTP_GET;
  else if(!strcmp(method, "POST"))
    ctx->method = HTTP_POST;
  else if(!strcmp(method, "DELETE"))
    ctx->method = HTTP_DELETE;
  else{
    LOG_ERROR("unsupported method: %s", method);
    free(ctx);
    return NULL;
  }
  request = checkpoint + 1;
  free((void *)method);

  // next, we find the route
  checkpoint = strstr(request, " ");
  char *route = strndup(request, checkpoint - request);
  LOG_DEBUG("Route: %s", route);
  ctx->route = route;
  request = checkpoint + 1;

  // from the route to the crlf is the http version
  checkpoint = strstr(request, crlf);
  char *version = strndup(request, checkpoint - request);
  if(strcmp(version, "HTTP/1.1")){
    LOG_ERROR("unsupported http version: `%s`", version);
    free_context(ctx);
    return NULL;
  }
  LOG_DEBUG("Version: %s", version);
  ctx->version = version;
  request = checkpoint + 2;

  // from the first line to the "\r\n\r\n" is the headers block
  checkpoint = strstr(request, "\r\n\r\n");
  const char *headers_block = strndup(request, checkpoint + 2 - request);
  HTTPHeaders *headers = parse_headers(headers_block);
  free((void *)headers_block);
  ctx->headers = headers;
  request = checkpoint + 4;
  LOG_INFO("parsed headers");
  
  const size_t content_length = strtoul(get_header_value(headers, "content-length"), NULL, 10);
  char *content = strdup(request);
  const size_t content_strlen = strlen(content);
  LOG_DEBUG("Length of the read body: %lu", content_strlen);
  LOG_DEBUG("Content-Length in headers: %lu", content_length);
  if (content_length != content_strlen){
    LOG_ERROR("Content-Length and length of the read body is not the same");
    free_context(ctx);
    free(content);
    return NULL;
  }
  ctx->body.content = content;
  ctx->body.content_length = content_length;

  return ctx;
}


int
free_context(Context *ctx){
  if(!ctx)
    return 0;

  if(ctx->route)
    free(ctx->route);

  if(ctx->version)
    free(ctx->version);

  if(ctx->headers)
    free_headers(ctx->headers);

  if(ctx->body.content)
    free(ctx->body.content);

  return 0;
}
