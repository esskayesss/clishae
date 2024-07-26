#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include "network/response.h"
#include "utils/fsutils.h"
#include "utils/logging.h"


static const char* get_mime_type(const char* filename) {
    if (strstr(filename, ".html")) return "text/html";
    if (strstr(filename, ".css")) return "text/css";
    if (strstr(filename, ".js")) return "application/javascript";
    if (strstr(filename, ".jpg")) return "image/jpeg";
    if (strstr(filename, ".png")) return "image/png";
    if (strstr(filename, ".ico")) return "image/x-icon"; // MIME type for favicon
    return "application/octet-stream"; // Default MIME type
}

char *
get_response(const ResponseCodes code, const unsigned long content_length, const char *content){
  char headers[256];
  if(content){
    sprintf(headers, "Content-Type: text/plain\r\nContent-Length: %lu\r\n", content_length);
  } else {
    strcpy(headers, "");
    content = "";
  }
  char *response = (char *)malloc(strlen(content) + 256);
  sprintf(response, "HTTP/1.1 %s\r\n%s\r\n%s", status_str[code], headers, content);
  return response;
}


char *
get_file_response(const ResponseCodes code, const char *filepath) {
    char *file_contents = NULL;
    int file_size = read_file(filepath, &file_contents);
    if (file_size < 0) {
        LOG_ERROR("Failed to read file: %s", filepath);
        return get_response(RESPONSE_404, 0, NULL);
    }

    const char *mime_type = get_mime_type(filepath);
    char headers[256];
    sprintf(headers, "Content-Type: %s\r\nContent-Length: %d\r\n", mime_type, file_size);

    char *response = malloc(file_size + 256);
    sprintf(response, "HTTP/1.1 %s\r\n%s\r\n%s", status_str[code], headers, file_contents);

    free(file_contents);
    return response;
}

char *
get_file_raw_response(const ResponseCodes code, const char *filepath) {
    char *file_contents = NULL;
    int file_size = read_file(filepath, &file_contents);
    if (file_size < 0) {
        LOG_ERROR("Failed to read file: %s", filepath);
        return get_response(RESPONSE_404, 0, NULL);
    }

    const char *mime_type = get_mime_type(filepath);
    char headers[256];
    sprintf(headers, "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n",
            status_str[code], mime_type, file_size);

    char *response = malloc(strlen(headers) + file_size);
    memcpy(response, headers, strlen(headers));
    memcpy(response + strlen(headers), file_contents, file_size);

    free(file_contents);
    return response;
}
