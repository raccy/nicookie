/*
 * nicookie_firefox.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nicookie.h"
#include "nicookie_config.h"

#define SEPARATOR '/'
#define COOKIES_SQLITE "/cookies.sqlite"

char *nicookie_firefox_cookies_path(const char *profile) {
  nicookie_debug_func();
  nicookie_debug_str(profile);

  FILE *profile_file = fopen(profile, "r");
  if (profile_file == NULL) return NULL;
  nicookie_debug_int(profile_file);

  char *profile_last_sep = strrchr(profile, SEPARATOR);
  if (profile_last_sep == NULL) return NULL;
  nicookie_debug_str(profile_last_sep);

  size_t buf_size = 256;
  char *buf = malloc(buf_size);
  if (buf == NULL) return NULL;
  nicookie_debug_int(buf);

  char *path = NULL;
  while (fgets(buf, buf_size, profile_file) != NULL) {
    if (strncmp(buf, "Path=", 5) == 0) {
      path = malloc(
          strlen(profile) - strlen(profile_last_sep) + 1 +
          strlen(buf) - 6 + strlen(COOKIES_SQLITE) + 1);
      if (path == NULL) {
        free(buf);
        return NULL;
      }
      nicookie_debug_int(path);

      path[0] = '\0';
      strncat(path, profile, strlen(profile) - strlen(profile_last_sep) + 1);
      nicookie_debug_str(path);

      strncat(path, buf + 5, strlen(buf) - 6);
      nicookie_debug_str(path);

      strcat(path, COOKIES_SQLITE);
      nicookie_debug_str(path);
      // TODO: ちょっと最後が改行じゃ無いときとか、あやしい

      break;
    }
  }
  free(buf);
  fclose(profile_file);
  return path;
}
