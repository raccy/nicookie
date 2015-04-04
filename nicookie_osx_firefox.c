/*
 * nicookie_osx_firefox.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */

#include <stdio.h>
#include <string.h>

#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_util.h"
#include "nicookie_osx.h"
#include "nicookie_firefox.h"
#include "sqlite3.h"

int nicookie_firefox_callback(void *data, int argc, char **argv, char **argvName) {
  nicookie_debug_func();
  nicookie_debug_int(argc);
  char **param = data;
  free(*param);
  char *value = malloc(strlen(argv[0]) + 1);
  if (value == NULL) return -1;
  strcpy(value, argv[0]);
  *param = value;
  return 0;
}

// #define
// OSX_FIREFOX_PROFILE_COOKIE_FILE_SUFFIX
// "${HOME}/Library/Application Support/Firefox/profiles.ini"
// "${HOME}/.mozilla/firefox/profiles.ini"
// "${APPDATA}\\Mozilla\\Firefox\\profiles.ini"

char *nicookie_osx_firefox(char *buf, size_t size) {
  char *firefox_profile = nicookie_str_with_env(
      "%s/Library/Application Support/Firefox/profiles.ini", "HOME");
  if (firefox_profile == NULL) return NULL;
  nicookie_debug_str(firefox_profile);

  const char *cookies_path = nicookie_firefox_cookies_path(firefox_profile);
  if (cookies_path == NULL) return NULL;
  nicookie_debug_str(cookies_path);

  sqlite3 *db = NULL;
  if (sqlite3_open_v2(cookies_path, &db, SQLITE_OPEN_READONLY, NULL)
      != SQLITE_OK) {
    sqlite3_close(db);
    return NULL;
  }
  nicookie_debug_int(db);

  char *errmsg = NULL;
  char *value = NULL;
  int result = sqlite3_exec(db, "select value from moz_cookies where host = '.nicovideo.jp' and name = 'user_session' and path = '/';", nicookie_firefox_callback, &value, &errmsg);
  nicookie_debug_int(result);
  nicookie_debug_int(value);

  sqlite3_close(db);
  free(firefox_profile);

  return value;

}
