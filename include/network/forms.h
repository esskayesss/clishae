#ifndef FORMS_H
#define FORMS_H

#include <stddef.h>

typedef struct {
  char    *contents;
  char    *filename;
  char    *content_type;
  size_t  filesize;
} FormFile;

typedef union FormValue {
  char      *body;
  FormFile  *file;
} FormValue;

typedef struct {
  char      *key;
  int       is_file;
  FormValue *value;
} FormField;

typedef struct {
  char      *boundary;
  FormField *fields;
} MultipartForm;

MultipartForm   *parse_multipart_form(const char *body, const char *boundary);
int             free_multipart(MultipartForm *form);

// TODO more methods to save files, etc
#endif
