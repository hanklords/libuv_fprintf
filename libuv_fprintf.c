#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "libuv_fprintf.h"
#define LIBUV_FPRINTF_MAXLEN 2048

void uv_fprintf_write(uv_write_t* req, int status) {
  uv_buf_t *buf = req->data;

  free(buf->base);
  free(buf);
  free(req);
}

int uv_fprintf(uv_stream_t* stream, const char* format, ...) {
  va_list ap;
  uv_write_t *req;
  uv_buf_t buf;
  int buf_len;
  char* buf_data;

  buf_data = malloc(LIBUV_FPRINTF_MAXLEN);

  va_start(ap, format);
  buf_len = vsnprintf(buf_data, LIBUV_FPRINTF_MAXLEN, format, ap);
  va_end(ap);
  if(buf_len < 0 || buf_len > LIBUV_FPRINTF_MAXLEN) {
    free(buf_data);
    return -1;
  }
  
  req = malloc(sizeof(*req));
  req->data = malloc(sizeof(uv_buf_t));
  buf = uv_buf_init(buf_data, buf_len);
  memcpy(req->data, &buf, sizeof(buf));
  uv_write(req, stream, req->data, 1, uv_fprintf_write);
  
  return 0;
}

