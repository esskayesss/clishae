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
  HTTPHeaders *headers = parse_headers(headers_block);
  const char *value = get_header_value(headers, "authorization");
  LOG_INFO("found value %s", value);
  free_headers(headers);
}
