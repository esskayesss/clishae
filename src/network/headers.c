#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "network/headers.h"
#include "utils/logging.h"


static inline void strtolower(char *p){
  for(; *p != '\0'; p++) *p = tolower(*p);
}

HTTPHeaders*
parse_headers(const char *headers_block){
  HTTPHeaders* headers = (HTTPHeaders *)malloc(sizeof(HTTPHeaders));
  if(!headers){
    LOG_ERROR("malloc failed for *headers: %s", strerror(errno));
    return NULL;
  }

  LOG_INFO("parsing headers from block\n%s", headers_block);
  size_t headers_len = 0;

  const char *header_end = headers_block;
  while((header_end = strstr(header_end, "\r\n"))){
    headers_len++;
    header_end += 2;
  }
  headers->headers_len = headers_len;
  headers->headers = (Header *)malloc(sizeof(Header) * headers_len);
  LOG_DEBUG("found %lu headers.", headers_len);

  const char *block_tmp = headers_block;
  char *crlf = strstr(block_tmp, "\r\n");
  char *colon = strstr(block_tmp, ":");
  while(headers_len){
    Header header = {0};
    // strndup key
    header.key = strndup(block_tmp, colon++ - block_tmp);
    strtolower(header.key);

    // strndup value
    header.value = strndup(colon, crlf - colon);
    strtolower(header.value);
    LOG_DEBUG("%s: %s", header.key, header.value);

    block_tmp = crlf + 2;
    crlf = strstr(block_tmp, "\r\n");
    colon = strstr(block_tmp, ":");
    headers->headers[headers_len-- - 1] = header;
  }
  LOG_INFO("all headers parsed.");

  return headers;
}

char *
get_header_value(const HTTPHeaders *headers, const char *key){
  char *lkey = strdup(key);
  strtolower(lkey);
  for(size_t i = 0; i < headers->headers_len; i++){
    if (strcmp(lkey, headers->headers[i].key) == 0){
      LOG_INFO("found header for key `%s`:\n{%s: %s}", key, headers->headers[i].key, headers->headers[i].value);
      return headers->headers[i].value;
    }
  }

  LOG_INFO("could not find header for key %s", key);
  return NULL;
}

int
free_headers(HTTPHeaders *headers){
  if (!headers) return 0;

  if (headers->headers){
    size_t i = 0;
    for(; i < headers->headers_len; i++){
      LOG_DEBUG("freeing header {%s: %s} ...", headers->headers[i].key, headers->headers[i].value);
      free(headers->headers[i].key);
      free(headers->headers[i].value);
    }
    free(headers->headers);
    LOG_DEBUG("freed headers->headers");
  }

  free(headers);
  LOG_DEBUG("freed headers struct");

  return 0;
}
