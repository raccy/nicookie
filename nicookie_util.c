/*
 * nicookie_util.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "nicookie.h"
#include "nicookie_config.h"

char *nicookie_str_with_env(const char *str, const char *env) {
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
