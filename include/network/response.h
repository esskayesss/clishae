#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdlib.h>

typedef enum {
  RESPONSE_100,
  RESPONSE_200,
  RESPONSE_201,
  RESPONSE_202,
  RESPONSE_400,
  RESPONSE_404,
  RESPONSE_405,
  RESPONSE_500,
  RESPONSE_505
} ResponseCodes;

static const char *status_str[] = {
  [RESPONSE_100] = "100 Continue",
  [RESPONSE_200] = "200 OK",
  [RESPONSE_201] = "201 Created",
  [RESPONSE_202] = "202 Accepted",
  [RESPONSE_400] = "400 Bad Request",
  [RESPONSE_404] = "404 Not Found",
  [RESPONSE_405] = "405 Method Not Allowed",
  [RESPONSE_500] = "500 Internal Server Error",
  [RESPONSE_505] = "505 HTTP Version Not Supported"
};


size_t send_response(const int cfd, const ResponseCodes code, const char *message);
size_t serve_file(const int cfd, const ResponseCodes code, const char *filepath);
#endif
