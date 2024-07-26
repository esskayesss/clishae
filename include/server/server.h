#ifndef SERVER_H
#define SERVER_H

int   initialize_server(unsigned int port);
void  server_listen();
void  *handle_connection(void *pcfd);

#endif
