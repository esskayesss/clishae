#ifndef RESPONSE_H
#define RESPONSE_H

typedef enum {
  RESPONSE_100,
  RESPONSE_200,
  RESPONSE_202,
  RESPONSE_404,
  RESPONSE_405,
  RESPONSE_500,
  RESPONSE_505
} ResponseCodes;

static const char *status_str[] = {
  [RESPONSE_100] = "100 Continue",
  [RESPONSE_200] = "200 OK",
  [RESPONSE_202] = "202 Accepted",
  [RESPONSE_404] = "404 Not Found",
  [RESPONSE_405] = "405 Method Not Allowed",
  [RESPONSE_500] = "500 Internal Server Error",
  [RESPONSE_505] = "505 HTTP Version Not Supported"
};


char *get_response(const ResponseCodes code, const unsigned long content_length, const char *content);
char *get_file_response(const ResponseCodes code, const char *filepath);
#endif
