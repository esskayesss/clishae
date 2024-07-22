#ifndef CTX_H
#define CTX_H

#include <stddef.h>
#include "headers.h"

// HTTP methods supported by this server
typedef enum {
  HTTP_GET,
  HTTP_POST,
  HTTP_DELETE
} HTTPMethods;

typedef struct {
  size_t  content_length;
  char    *content;
} HTTPBody;

typedef struct {
  HTTPMethods method;
  char        *route;
  char        *version;
  HTTPHeaders *headers;
  HTTPBody    body;
} Context;


Context   *parse_context(const char *request);
int       free_context(Context *ctx);
#endif
