/*
 * nicookie_firefox.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sqlite3.h>

#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_util.h"
#include "nicookie_firefox.h"

#define NICOOKIE_FIREFOX_COOKIES_SQLITE "/cookies.sqlite"

char *nicookie_firefox_cookies_path(const char *profile) {
  nicookie_debug_func();
  nicookie_debug_str(profile);

  FILE *profile_file = fopen(profile, "r");
  if (profile_file == NULL) return NULL;
  nicookie_debug_int(profile_file);

  char *profile_last_sep = strrchr(profile, NICOOKIE_PATH_SEPARATOR);
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
          strlen(buf) - 6 + strlen(NICOOKIE_FIREFOX_COOKIES_SQLITE) + 1);
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

      strcat(path, NICOOKIE_FIREFOX_COOKIES_SQLITE);
      nicookie_debug_str(path);
      // TODO: ちょっと最後が改行じゃ無いときとか、あやしい

      break;
    }
  }
  free(buf);
  fclose(profile_file);
  return path;
}

char *nicookie_firefox_cookies(char *buf, size_t size,
    const char *cookies_path) {
  nicookie_debug_str(cookies_path);

  char *value = NULL;
  int result = nicookie_sqlite3(cookies_path,
    "SELECT value FROM moz_cookies WHERE host = '" NICOOKIE_COOKIE_HOST
    "' and name = '" NICOOKIE_COOKIE_NAME
    "' and path = '" NICOOKIE_COOKIE_PATH
    "';",
    &value);
  if (result != SQLITE_OK || value == NULL) {
    if (errno == 0) errno = ENOENT;
    return NULL;
  }

  if (buf == NULL && size == 0) {
    buf = malloc(strlen(value) + 1);
    if (buf == NULL) return NULL;
  } else if (size < strlen(value) + 1) {
    errno = ERANGE;
    return NULL;
  } else if (buf == NULL) {
    buf = malloc(size);
    if (buf == NULL) return NULL;
  }
  strcpy(buf, value);

  return buf;
}
