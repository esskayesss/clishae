#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils/fsutils.h"
#include "utils/error.h"
#include "utils/logging.h"

int read_file(const char *path, char **contents){
  FILE *file = fopen(path, "r");
  if(!file){
    return 1;
  }

  struct stat file_stat;
  stat(path, &file_stat);
  unsigned long file_size = file_stat.st_size;

  char *body = malloc(file_size + 1);
  fread(body, 1, file_size, file);
  body[file_size] = '\0';
  *contents = body;

  fclose(file);
  return file_size;
}

int
new_file(const char *path, const char *contents){
  FILE *file = fopen(path, "w");
  if(!file){
    LOG_ERROR("could not open file for writing: %s", ERRMSG);
    return 1;
  }
  size_t nitems = strlen(contents);
  size_t wbytes = fwrite(contents, 1, nitems, file);
  if (nitems > wbytes){
    LOG_ERROR("fwrite failed after writing %lu items: %s", wbytes, ERRMSG);
    return 1;
  }
  fclose(file);

  return 0;
}

int
delete_file(const char *path){
  if(remove(path)){
    LOG_ERROR("file deletion failed: %s", ERRMSG);
    return 1;
  }
  return 0;
}
