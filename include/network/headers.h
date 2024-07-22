#ifndef HEADERS_H
#define HEADERS_H

typedef struct {
  char *key, *value;
} Header;

typedef struct {
  Header        *headers;
  unsigned int  headers_len;
} HTTPHeaders;

HTTPHeaders*  parse_headers(const char *headers_block);
char*         get_header_value(const HTTPHeaders *headers, const char *key);
int           free_headers(HTTPHeaders *headers);
#endif
