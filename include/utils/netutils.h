#ifndef NETUTILS_H
#define NETUTILS_H

#include "network/context.h"
#include "network/forms.h"


// query headers in a context by key
char  *get_header(const Context *ctx, const char *key);

// returns the boundary value if the body is a multipart form, NULL otherwise
char  *is_multipart_form(const Context *ctx);

// query a multipart form by a key
char  *get_field_value(const MultipartForm *form, const char *key);

#endif
