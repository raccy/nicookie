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

#ifdef _WIN32
#define NICOOKIE_PATH_SEPARATOR '\\'
#else
#define NICOOKIE_PATH_SEPARATOR '/'
#endif

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

char *nicookie_str_with_env(const char *str, const char *env);

int nicookie_sqlite3(const char *filename, const char *sql, ...);

#endif // nicookie_util_H
