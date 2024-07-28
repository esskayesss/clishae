#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils/fsutils.h"
#include "utils/error.h"
#include "utils/logging.h"


char *
get_random_name(){
  char *filename = (char *)malloc(25);
  sprintf(filename, "%lu", (unsigned long)time(NULL));

  return filename;
}


size_t read_file(const char *path, char **contents) {
  FILE *file = fopen(path, "r"); // Use binary mode
  if (!file){
    LOG_ERROR("could not open file: %s", ERRMSG);
    return -1;
  }

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *body = malloc(file_size);
  if (!body) {
    LOG_ERROR("failed to allocate memory for body: %s", ERRMSG);
    fclose(file);
    return -1;
  }
  size_t read_size = fread(body, 1, file_size, file);
  if (read_size != file_size) {
    LOG_ERROR("failed to read from file: %s", ERRMSG);
    free(body);
    fclose(file);
    return -1;
  }
  *contents = body;
  fclose(file);
  return file_size;
}

size_t
get_filesize(FILE *file){
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

char *
new_file(const char *contents) {
  char *rand_name = get_random_name();
  if (!rand_name) {
    LOG_ERROR("could not generate random name");
    return NULL;
  }

  char path[256];
  snprintf(path, sizeof(path), "/tmp/clishae/raw/%s", rand_name);

  FILE *file = fopen(path, "w");
  if (!file) {
    LOG_ERROR("could not open file for writing: %s", strerror(errno));
    free(rand_name);
    return NULL;
  }

  size_t nitems = strlen(contents);
  size_t wbytes = fwrite(contents, 1, nitems, file);
  if (nitems > wbytes) {
    LOG_ERROR("fwrite failed after writing %lu items: %s", wbytes, strerror(errno));
    fclose(file);
    free(rand_name);
    return NULL;
  }

  fclose(file);

  char *get_url = malloc(256);
  snprintf(get_url, 256, "http://localhost:8080/raw/%s\n", rand_name);

  free(rand_name);
  return get_url;
}

int
delete_file(const char *path){
  if(remove(path)){
    LOG_ERROR("file deletion failed: %s", ERRMSG);
    return 1;
  }
  return 0;
}
