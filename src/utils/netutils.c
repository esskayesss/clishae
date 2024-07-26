#include <string.h>
#include "network/headers.h"
#include "utils/netutils.h"
#include "utils/logging.h"

char *
get_header(const Context *ctx, const char *key){
  char *val = get_header_value(ctx->headers, key);
  if(val){
    return val; 
  }
  LOG_INFO("returning NULL");
  return NULL;
}

char *
is_multipart_form(const Context *ctx){
  const char *content_type = get_header(ctx, "content-type");
  char *ptr = strstr(content_type, "multipart/form-data;");
  if (!ptr){
    LOG_INFO("the body does not have multipart form data.");
    return NULL;
  }
  ptr = strstr(content_type, "=");
  char *boundary = strndup(ptr + 1, strlen(ptr + 1));
  return boundary;
}
