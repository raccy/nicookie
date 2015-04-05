/*
 * nicookie_chrome.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */

#include <stdlib.h>
#include <stddef.h>
#include <sqlite3.h>

#include "nicookie.h"
#include "nicookie_util.h"
#include "nicookie_util.h"
#include "nicookie_chrome.h"

char *nicookie_chrome_cookies(char *buf, size_t size, const char *cookies_path,
    char *(*decrypt_func)(const char *, int)) {
  char *value = NULL;
  void *encrypted_value = NULL;
  int encrypted_value_size = 0;
  int result = nicookie_sqlite3(cookies_path,
      "SELECT value,encrypted_value FROM cookies WHERE"
      " host_key = '"  NICOOKIE_COOKIE_HOST
      "' AND name = '" NICOOKIE_COOKIE_NAME
      "' AND path = '" NICOOKIE_COOKIE_PATH "';",
      &value, &encrypted_value, &encrypted_value_size);
  if (result != SQLITE_OK || value == NULL) {
    if (errno == 0) errno = ENOENT;
    free(value);
    free(encrypted_value);
    return NULL;
  }
  nicookie_debug_str(value);
  nicookie_debug_int(encrypted_value_size);

  char *result_buf = NULL;
  if (encrypted_value_size == 0) {
    result_buf = nicookie_setstr(buf, size, value);
  } else {
    char *plain_value = decrypt_func(encrypted_value,
        encrypted_value_size);
    if (plain_value == NULL) {
      free(value);
      free(encrypted_value);
      return NULL;
    }
    result_buf = nicookie_setstr(buf, size, plain_value);
    free(plain_value);
  }
  free(value);
  free(encrypted_value);
  return result_buf;
}
