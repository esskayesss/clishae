#ifndef FSUTILS_H
#define FSUTILS_H

#include <stddef.h>

int     file_exists(const char *path);
int     read_file(const char *path, char **contents, size_t *bytes_read);
int     new_file(const char *path, const char *contents);
int     delete_file(const char *path);
size_t  get_file_size(const char *path);

#endif
