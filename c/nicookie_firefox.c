/*
 * nicookie_firefox.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sqlite3.h>

#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_util.h"
#include "nicookie_firefox.h"

#define NICOOKIE_FIREFOX_COOKIES_SQLITE "cookies.sqlite"
#define NICOOKIE_FIREFOX_PROFILES_MAX_LINE_LENGTH 1024

char *nicookie_firefox_profiles(char *buf, size_t size, char *profiles_ini) {
  char *cookies_path = nicookie_firefox_cookies_path(profiles_ini);
  if (cookies_path == NULL) return NULL;
  nicookie_debug_str(cookies_path);

  char *result = nicookie_firefox_cookies(buf, size, cookies_path);

  free(cookies_path);
  return result;
}

char *nicookie_firefox_cookies_path(const char *profiles_ini) {
  nicookie_debug_str(profiles_ini);

  char *profiles_ini_last_sep = strrchr(profiles_ini, NICOOKIE_PATH_SEPARATOR);

  FILE *profiles_ini_file = fopen(profiles_ini, "r");
  if (profiles_ini_file == NULL) return NULL;

  char read_buf[NICOOKIE_FIREFOX_PROFILES_MAX_LINE_LENGTH];
  char path[NICOOKIE_FIREFOX_PROFILES_MAX_LINE_LENGTH];
  path[0] = '\0';
  int is_relative = -1;
  while (fgets(read_buf, NICOOKIE_FIREFOX_PROFILES_MAX_LINE_LENGTH,
      profiles_ini_file) != NULL) {
    // TODO: if over line length, remain is as next line.
    nicookie_str_chomp(read_buf);
    if (path[0] == '\0' && strncmp(read_buf, "Path=", 5) == 0) {
      strcpy(path, read_buf + 5);
      if (is_relative != -1) break;
    } else if (is_relative == -1 && strncmp(read_buf, "IsRelative=", 11) == 0) {
      if (read_buf[11] == '0') {
        is_relative = 0;
      } else {
        is_relative = 1;
      }
      if (path[0] != '\0') break;
    }
  }
  fclose(profiles_ini_file);

  nicookie_debug_str(path);
  if (strlen(path) == 0) {
    errno = ENOENT;
    return NULL;
  }

  char *cookies_path = NULL;
  if (is_relative && profiles_ini_last_sep != NULL) {
    cookies_path = malloc(
        strlen(profiles_ini) - strlen(profiles_ini_last_sep) + 1 +
        strlen(path) + 1 + strlen(NICOOKIE_FIREFOX_COOKIES_SQLITE) + 1);
    if (cookies_path == NULL) return NULL;
    cookies_path[0] = '\0';
    // inculde separtor
    strncat(cookies_path, profiles_ini,
        strlen(profiles_ini) - strlen(profiles_ini_last_sep) + 1);
    nicookie_debug_str(cookies_path);

    strcat(cookies_path, path);
    nicookie_debug_str(cookies_path);

    cookies_path[strlen(cookies_path)] = NICOOKIE_PATH_SEPARATOR;
    cookies_path[strlen(cookies_path)] = '\0';
    strcat(cookies_path, NICOOKIE_FIREFOX_COOKIES_SQLITE);
    nicookie_debug_str(cookies_path);
  } else {
    cookies_path = malloc(strlen(path) + 1);
    if (cookies_path == NULL) return NULL;
    strcpy(cookies_path, path);
  }

  return cookies_path;
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
    free(value);
    return NULL;
  }

  char *result_buf = nicookie_setstr(buf, size, value);
  free(value);

  return result_buf;
}
