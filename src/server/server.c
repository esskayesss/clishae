#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "network/context.h"
#include "utils/error.h"
#include "utils/logging.h"
#include "utils/netutils.h"
#include "server/server.h"
#include "network/response.h"

#define BUF_SIZE  8192
#define L_BACKLOG 100

static int sfd;

int initialize_server(unsigned int port) {
  if (port >= 65535) {
    handle_err("Invalid port number. Won't open socket.");
  }
  struct sockaddr_in serv_addr;

  sfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_err("Could not open socket: %s", ERRMSG);
  }
  LOG_INFO("Socket opened");

  memset(&serv_addr, 0, sizeof(struct sockaddr_in));
  serv_addr.sin_family = PF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (bind(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
    handle_err("Failed to bind: %s", ERRMSG);
  }
  LOG_INFO("socket bound.");

  if (listen(sfd, L_BACKLOG) == -1) {
    handle_err("Failed to listen: %s", ERRMSG);
  }
  LOG_INFO("listening on socket.");

  int opt = 1;
  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    handle_err("setsockopt failed: %s", ERRMSG);
  }

  return 0;
}

int close_socket() {
  return close(sfd);
}

void server_listen() {
  int cfd;
  socklen_t peer_addrlen;
  struct sockaddr_in peer_addr;

  while (1) {
    cfd = accept(sfd, (struct sockaddr*)&peer_addr, &peer_addrlen);
    if (cfd == -1) {
      LOG_ERROR("Failed to accept connection: %s", ERRMSG);
      continue;
    }
    LOG_INFO("client connected.");

    pthread_t new_process;
    int *pcfd = malloc(sizeof(int));
    if (!pcfd) {
      LOG_ERROR("Failed to allocate memory: %s", ERRMSG);
      close(cfd);
      continue;
    }
    *pcfd = cfd;

    if (pthread_create(&new_process, NULL, handle_connection, pcfd)) {
      LOG_ERROR("Failed to create thread: %s", ERRMSG);
      free(pcfd);
      close(cfd);
      continue;
    }
    pthread_detach(new_process);
  }
}

void *handle_connection(void *pcfd) {
  int cfd = *(int *)pcfd;
  free(pcfd);
  char *response = NULL;

  char BUFFER[BUF_SIZE];
  memset(BUFFER, 0, BUF_SIZE);

  // Read the request into BUFFER
  ssize_t bytes_read = read(cfd, BUFFER, BUF_SIZE);
  if (bytes_read <= 0) {
    LOG_ERROR("Failed to read from socket");
    close(cfd);
    return NULL;
  }

  // Parse the context
  Context *ctx = parse_context(BUFFER);
  if (!ctx) {
    LOG_ERROR("Failed to parse context");
    close(cfd);
    return NULL;
  }

  // Handle "Expect: 100-continue" header
  char *expect = get_header_value(ctx->headers, "expect");
  if (expect && strcmp("100-continue", expect) == 0) {
    LOG_INFO("Returning a 100 Continue");
    response = get_response(RESPONSE_100, 0, NULL);
    if (response) {
      write(cfd, response, strlen(response));
      free(response);

      // Allocate memory for the body and read it from the socket
      char *body = (char *)malloc(ctx->body.content_length + 1);
      if (body) {
        memset(body, 0, ctx->body.content_length + 1);
        bytes_read = read(cfd, body, ctx->body.content_length);
        if ((size_t)bytes_read < ctx->body.content_length) {
          LOG_ERROR("Failed to read the entire body from socket");
          free(body);
          free_context(ctx);
          close(cfd);
          return NULL;
        }
        ctx->body.content = body;
      } else {
        LOG_ERROR("Failed to allocate memory for body");
        free_context(ctx);
        close(cfd);
        return NULL;
      }
    }
  }

  // Create and send the final response
  response = get_response(RESPONSE_200, 4, "pong");
  if (response) {
    write(cfd, response, strlen(response));
    free(response);
  }

  // Clean up and close the connection
  free_context(ctx);
  close(cfd);
  return NULL;
}

