#include <string.h>
#include "network/headers.h"
#include "utils/netutils.h"
#include "utils/logging.h"

char
*get_header(const Context *ctx, const char *key){
  return get_header_value(ctx->headers, key);
}

char
*is_multipart_form(const Context *ctx){
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

// query a multipart form by a key
char  *get_field_value(const MultipartForm *form, const char *key);
