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
  size_t  content_length; // length of the request body
  char    *content;       // copy of the request body
} HTTPBody;

typedef struct {
  int         cfd;
  HTTPMethods method;     // HTTP method of the request
  char        *route;     // route of the request
  char        *version;   // HTTP version of the request
  HTTPHeaders *headers;   // headers in the HTTP request
  HTTPBody    body;       // body of the request
} Context;


// parses the entire context from a request
Context   *parse_context(int cfd, const char *request);

// frees a context struct
int       free_context(Context *ctx);
#endif
