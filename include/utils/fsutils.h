#ifndef FSUTILS_H
#define FSUTILS_H

#include <stddef.h>

int     read_file(const char *path, char **contents);
int     new_file(const char *path, const char *contents);
int     delete_file(const char *path);

#endif
