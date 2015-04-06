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
#include <stdarg.h>

#include <sqlite3.h>

#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_util.h"

char *nicookie_setstr(char *buf, size_t size, const char *str) {
  if (buf == NULL && size == 0) {
    buf = malloc(strlen(str) + 1);
    if (buf == NULL) return NULL;
  } else if (size < strlen(str) + 1) {
    errno = ERANGE;
    return NULL;
  } else if (buf == NULL) {
    buf = malloc(size);
    if (buf == NULL) return NULL;
  }
  strcpy(buf, str);
  return buf;
}

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

// @return sqlite result
// get first reocrd noly.
int nicookie_sqlite3(const char *filename, const char *sql, ...) {
  nicookie_debug_func();

  va_list list;
  va_start(list, sql);

  sqlite3 *db = NULL;
  int result = SQLITE_OK;
  result = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL);
  nicookie_debug_int(result);
  if (result != SQLITE_OK) {
    sqlite3_close(db);
    va_end(list);
    return result;
  }

  sqlite3_stmt *stmt = NULL;
  result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  nicookie_debug_int(result);
  if (result != SQLITE_OK) {
    sqlite3_close(db);
    sqlite3_finalize(stmt);
    va_end(list);
    return result;
  }

  result = sqlite3_step(stmt);
  nicookie_debug_int(result);
  if (result != SQLITE_ROW) {
    sqlite3_close(db);
    sqlite3_finalize(stmt);
    va_end(list);
    errno = ENOENT;
    return result;
  }

  int query_size = 0;
  const char *query_text = NULL;
  const void *query_blob = NULL;
  char *buf_text = NULL;
  void *buf_blob = NULL;
  // only one record
  int count = sqlite3_data_count(stmt);
  for (int i = 0; i < count; i++) {
    nicookie_debug_int(i);
    nicookie_debug_int(sqlite3_column_type(stmt, i));
    switch(sqlite3_column_type(stmt, i)) {
    case SQLITE_INTEGER:
      *(va_arg(list, int *)) = sqlite3_column_int(stmt, i);
      break;
    case SQLITE_FLOAT:
      *(va_arg(list, double *)) = sqlite3_column_double(stmt, i);
      break;
    case SQLITE3_TEXT:
      query_text = (const char *)sqlite3_column_text(stmt, i);
      if (query_text == NULL) {
        *(va_arg(list, char **)) = NULL;
      } else {
        buf_text = (char *)malloc(strlen(query_text) + 1);
        if (buf_text == NULL) {
          sqlite3_close(db);
          sqlite3_finalize(stmt);
          va_end(list);
          return SQLITE_NOMEM;
        }
        strcpy(buf_text, query_text);
        *(va_arg(list, char **)) = buf_text;
      }
      break;
    case SQLITE_BLOB:
      query_size = sqlite3_column_bytes(stmt, i);
      if (query_size == 0) {
        *(va_arg(list, void **)) = NULL;
        *(va_arg(list, int *)) = 0;
      } else {
        query_blob = sqlite3_column_blob(stmt, i);
        buf_blob = (void *)malloc(query_size);
        if (buf_blob == NULL) {
          sqlite3_close(db);
          sqlite3_finalize(stmt);
          va_end(list);
          return SQLITE_NOMEM;
        }
        memcpy(buf_blob, query_blob, query_size);
        *(va_arg(list, void **)) = buf_blob;
        *(va_arg(list, int *)) = query_size;
      }
      break;
    case SQLITE_NULL:
      *(va_arg(list, void **)) = NULL;
      break;
    default:
      // bug ? nuknow type
      sqlite3_close(db);
      sqlite3_finalize(stmt);
      va_end(list);
      return SQLITE_ERROR;
      break;
    }
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  va_end(list);

  return SQLITE_OK; // = 0
}

// similar Ruby's String#chomp!
char *nicookie_str_chomp(char *str) {
  if (str == NULL) return NULL;

  size_t length = strlen(str);
  if (length == 0) return str;

  if (str[length - 1] == '\n') {
    str[length - 1] = '\0';
    if (length > 1 && str[length - 2] == '\r') {
      str[length - 2] = '\0';
    }
  } else if (str[length - 1] == '\r') {
    str[length - 1] = '\0';
  }

  return str;
}