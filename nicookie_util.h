/*
 * nicookie_util.h
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#ifndef nicookie_util_H
#define nicookie_util_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "nicookie.h"
#include "nicookie_config.h"

// max path size 256
#define NICOOKIE_PATH_MAX 256

static inline uint32_t fget_uint32_be(FILE *stream) {
  uint32_t n = 0;
  for (int i = 0; i < 4; i++) {
    int c = getc(stream);
    if (c == EOF) return 0;
    n += ((uint32_t)c) << (8 * (3 - i));
  }
  return n;
}

static inline uint32_t fget_uint32_le(FILE *stream) {
  uint32_t n = 0;
  for (int i = 0; i < 4; i++) {
    int c = getc(stream);
    if (c == EOF) return 0;
    n += ((uint32_t)c) << (8 * i);
  }
  return n;
}

static inline int fmove_uint32(FILE *stream, uint32_t size) {
#if LONG_MAX >= 0xFFFFFFFF
  return fseek(stream, (long)size, SEEK_CUR);
#else
  int code;
  while (size > 0) {
    if (size > LONG_MAX) {
      code = fseek(stream, LONG_MAX, SEEK_CUR);
      if (code != 0) break;
      size -= LONG_MAX;
    } else {
      code = fseek(stream, (long)size, SEEK_CUR);
      size = 0;
    }
  }
  return code;
#endif
}

static inline bool fcheckstr(FILE *stream, const char *s) {
  for (;*s != '\0'; s++) {
    if (*s != getc(stream)) {
      return false;
    }
  }
  if (getc(stream) != '\0') {
    return false;
  }
  return true;
}

static inline size_t fsizetonull(FILE *stream) {
  fpos_t pos;
  fgetpos(stream, &pos);
  size_t count = 0;
  while (true) {
    int c = getc(stream);
    if (c == EOF) {
      count = 0;
      break;
    } else if (c == '\0') {
      count++;
      break;
    } else {
      count++;
    }
  }
  fsetpos(stream, &pos);
  return count;
}

static inline char *fgetstr(FILE *stream, size_t n, char *s) {
  for (size_t i = 0; i < n; i++) {
    int c = getc(stream);
    if (c == EOF) return NULL;
    s[i] = (char)c;
  }
  return s;
}

static inline char *nicookie_str_with_env(const char *str, const char *env) {
  nicookie_debug_func();
  nicookie_debug_str(str);
  nicookie_debug_str(env);

  char *env_value = getenv(env);
  if (env_value == NULL) return NULL;
  nicookie_debug_str(env_value);

  int buf_size = strlen(str) - 2 + strlen(env_value) + 1;
  nicookie_debug_int(buf_size);

  char *buf = malloc(buf_size);
  if (buf == NULL) return NULL;
  nicookie_debug_int(buf);

  int result = snprintf(buf, buf_size, str, env_value);
  nicookie_debug_int(result);

  if (result == buf_size - 1) {
    return buf;
  } else {
    free(buf);
    errno = EILSEQ;
    return NULL;
  }
}

#endif // nicookie_util_H
