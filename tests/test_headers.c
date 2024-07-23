#include <stddef.h>
#include "utils/logging.h"
#include "network/headers.h"

int
main() {
  char *headers_block =
    "Authorization: Bearer caldncjkadnvjrbavbkGUSAUgJUVJVBCJDVBJHVDJB\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 69420\r\n";

  initialize_logger(NULL, LEVEL_TRACE);
  parse_headers(headers_block);
}
