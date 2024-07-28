#ifndef HANDLERS_H
#define HANDLERS_H

#include "network/context.h"

int get_ping(int cfd, Context *ctx);
int get_paste(int cfd, Context *ctx);
int get_paste_raw(int cfd, Context *ctx);
int post_paste(int cfd, Context *ctx);
int delete_paste(int cfd, Context *ctx);

#endif
