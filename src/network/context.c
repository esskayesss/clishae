#include <string.h>
#include <stdlib.h>
#include "network/headers.h"
#include "utils/error.h"
#include "utils/logging.h"
#include "network/context.h"


Context *
parse_context(int cfd, const char *request){
  Context *ctx = (Context *)malloc(sizeof(Context));
  if (!ctx) {
    LOG_ERROR("failed to allocate memory for context: %s", ERRMSG);
    return NULL;
  }
  memset(ctx, 0, sizeof(Context));
  ctx->cfd = cfd;
  const char *crlf = "\r\n";

  // Parse HTTP method
  char *checkpoint = strstr(request, " ");
  if (!checkpoint) {
    LOG_ERROR("Invalid HTTP request format: missing space after method");
    free(ctx);
    return NULL;
  }
  const char *method = strndup(request, checkpoint - request);
  LOG_DEBUG("Method: %s", method);
  if (!strcmp(method, "GET"))
    ctx->method = HTTP_GET;
  else if (!strcmp(method, "POST"))
    ctx->method = HTTP_POST;
  else if (!strcmp(method, "DELETE"))
    ctx->method = HTTP_DELETE;
  else {
    LOG_ERROR("Unsupported method: %s", method);
    free((void *)method);
    free(ctx);
    return NULL;
  }
  request = checkpoint + 1;
  free((void *)method);

  // Parse route
  checkpoint = strstr(request, " ");
  if (!checkpoint) {
    LOG_ERROR("Invalid HTTP request format: missing space after route");
    free(ctx);
    return NULL;
  }
  char *route = strndup(request, checkpoint - request);
  LOG_DEBUG("Route: %s", route);
  ctx->route = route;
  request = checkpoint + 1;

  // Parse HTTP version
  checkpoint = strstr(request, crlf);
  if (!checkpoint) {
    LOG_ERROR("Invalid HTTP request format: missing CRLF after version");
    free_context(ctx);
    return NULL;
  }
  char *version = strndup(request, checkpoint - request);
  if (strcmp(version, "HTTP/1.1")) {
    LOG_ERROR("Unsupported HTTP version: `%s`", version);
    free_context(ctx);
    return NULL;
  }
  LOG_DEBUG("Version: %s", version);
  ctx->version = version;
  request = checkpoint + 2;

  // Parse headers
  checkpoint = strstr(request, "\r\n\r\n");
  if (!checkpoint) {
    LOG_ERROR("Invalid HTTP request format: missing CRLFCRLF after headers");
    free_context(ctx);
    return NULL;
  }
  const char *headers_block = strndup(request, checkpoint + 2 - request);
  HTTPHeaders *headers = parse_headers(headers_block);
  if (!headers) {
    LOG_ERROR("Failed to parse headers");
    free((void *)headers_block);
    free_context(ctx);
    return NULL;
  }
  free((void *)headers_block);
  ctx->headers = headers;
  request = checkpoint + 4;
  LOG_INFO("Parsed headers");

  // Parse content length and body
  ctx->body.content_length = 0;
  ctx->body.content = NULL;
  char *cl_header = get_header_value(headers, "content-length");
  if (cl_header == NULL) {
    LOG_DEBUG("No content-length header, not reading body.");
  } else {
    const size_t content_length = strtoul(cl_header, NULL, 10);
    if (content_length > 8192) {
      LOG_ERROR("Content length exceeds maximum allowed size");
      free_context(ctx);
      return NULL;
    }
    ctx->body.content_length = content_length;
    char *content = strdup(request);
    if (!content) {
      LOG_ERROR("Failed to allocate memory for content");
      free_context(ctx);
      return NULL;
    }
    const size_t content_strlen = strlen(content);
    LOG_DEBUG("Length of the read body: %lu", content_strlen);
    LOG_DEBUG("Content-Length in headers: %lu", content_length);
    if (content_length != content_strlen && !get_header_value(headers, "expect")) {
      LOG_ERROR("Content-Length and length of the read body are not the same");
      free(content);
      free_context(ctx);
      return NULL;
    }
    ctx->body.content = content;
  }

  LOG_INFO("Parsed request into a context struct successfully.");
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
