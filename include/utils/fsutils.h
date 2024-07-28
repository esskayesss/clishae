#ifndef FSUTILS_H
#define FSUTILS_H

#include <stddef.h>
#include <stdio.h>

size_t  read_file(const char *path, char **contents);
char    *new_file(const char *contents);
size_t  get_filesize(FILE *file);
int     delete_file(const char *path);

#endif
