#include "network/context.h"
#include "utils/logging.h"

int req_post_success(){
  char* request =
    "POST /path/to/resource HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 12\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World!";
  initialize_logger(NULL, LEVEL_TRACE);

  Context *ctx = parse_context(request);
  if (!ctx) return 1;
  free_context(ctx);

  return 0;
}

int req_get_success(){
  char* request =
    "GET /path/to/resource HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 12\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World!";
  initialize_logger(NULL, LEVEL_TRACE);

  Context *ctx = parse_context(request);
  if (!ctx) return 1;
  free_context(ctx);

  return 0;
}

int req_get_failure_ctxlen(){
  char* request =
    "GET /path/to/resource HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 17\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World!";
  initialize_logger(NULL, LEVEL_TRACE);

  Context *ctx = parse_context(request);
  if (!ctx) return 1;
  free_context(ctx);

  return 0;
}

int req_get_failure_version(){
  char* request =
    "GET /path/to/resource HTTP/1.0\r\n"
    "Host: example.com\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 12\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World!";
  initialize_logger(NULL, LEVEL_TRACE);

  Context *ctx = parse_context(request);
  if (!ctx) return 1;
  free_context(ctx);

  return 0;
}

int 
main() {
  if(req_get_success()) {
    LOG_ERROR("test `req_get_success` failed");
  } else {
    LOG_SUCCESS("test `req_get_success` succeeded");
  }

  if(req_post_success()) {
    LOG_ERROR("test `req_post_success` failed");
  } else {
    LOG_SUCCESS("test `req_post_success` succeeded");
  }

  if(!req_get_failure_ctxlen()) {
    LOG_ERROR("test `req_get_failure_ctxlen` failed");
  } else {
    LOG_SUCCESS("test `req_get_failure_ctxlen` succeeded");
  }

  if(!req_get_failure_version()) {
    LOG_ERROR("test `req_get_failure_version` failed");
  } else {
    LOG_SUCCESS("test `req_get_failure_version` succeeded");
  }
}
