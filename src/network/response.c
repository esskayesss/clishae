#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdio.h>
#include "network/response.h"
#include "network/context.h"
#include "utils/fsutils.h"
#include "utils/logging.h"
#include "utils/error.h"

#define BUF_SIZE 8192


static char* get_mime_type(const char* filename) {
    if (strstr(filename, ".html")) return "text/html";
    if (strstr(filename, ".mp4")) return "video/mp4";
    if (strstr(filename, ".css")) return "text/css";
    if (strstr(filename, ".js")) return "application/javascript";
    if (strstr(filename, ".jpeg")) return "image/jpeg";
    if (strstr(filename, ".png")) return "image/png";
    if (strstr(filename, ".ico")) return "image/x-icon"; // MIME type for favicon
    return "text/plain"; // Default MIME type
}

ssize_t send_all(int socket, const char *buffer, size_t length) {
  size_t total_sent = 0;
  ssize_t sent;

  while (total_sent < length) {
    sent = send(socket, buffer + total_sent, length - total_sent, 0);
    if (sent == -1) {
      // Handle recoverable errors
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_WARN("EAGAIN | EWOULDBLOCK");
        usleep(1000);
        continue;
      } else {
        LOG_ERROR("send failed: %s", strerror(errno));
        return -1;
      }
    }
    // Handle case where no data was sent
    if (sent == 0) {
      break;
    }
    total_sent += sent;
  }
  return total_sent;
}

size_t
send_response(const int cfd, const ResponseCodes code, const char *message){
  size_t bytes_sent = 0;
  size_t content_length = strlen(message);

  char meta[256];
  sprintf(meta, "HTTP/1.1 %s\r\nContent-Type: text/plain\r\nContent-Length: %lu\r\nConnection: close\r\n\r\n", status_str[code], content_length);
  bytes_sent += send_all(cfd, meta, strlen(meta));
  bytes_sent += send_all(cfd, message, content_length);

  return bytes_sent;
}

size_t serve_file(const int cfd, const ResponseCodes code, const char *filepath) {
  size_t bytes_sent = 0;
  char *mime = get_mime_type(filepath);
  FILE *file = NULL;

  if(strcmp(mime, "text/plain") == 0)
    file = fopen(filepath, "r");
  else
    file = fopen(filepath, "rb");
  if (!file) {
    LOG_ERROR("could not open file `%s`: %s", filepath, strerror(errno));
    return -1;
  }

  size_t content_length = get_filesize(file);
  if (content_length == 0) {
    fclose(file);
    LOG_ERROR("could not determine file size or file is empty");
    return -1;
  }

  char meta[256];
  snprintf(meta, sizeof(meta),
      "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %lu\r\n\r\n",
      status_str[code], get_mime_type(filepath), content_length);

  ssize_t header_sent = send_all(cfd, meta, strlen(meta));
  if (header_sent < 0) {
    fclose(file);
    return -1;
  }
  bytes_sent += header_sent;
  LOG_DEBUG("headers:\n%s", meta);

  char buffer[1024];
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
    ssize_t sent = send_all(cfd, buffer, bytes_read);
    if (sent < 0) {
      fclose(file);
      return -1;
    }
    bytes_sent += sent;
  }
  printf("\n");

  if (ferror(file)) {
    LOG_ERROR("error reading file: %s", strerror(errno));
    fclose(file);
    return -1;
  }

  fclose(file);
  return bytes_sent;
}
