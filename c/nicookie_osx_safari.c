/*
 * nicookie_osx_safari.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

#include "nicookie.h"
#include "nicookie_osx.h"
#include "nicookie_util.h"

#define SAFARI_COOKIE_FILE_SUFFIX "/Library/Cookies/Cookies.binarycookies"

/*
 * OS X Safari Cookie data format
 * http://www.securitylearn.net/2012/10/27/cookies-binarycookies-reader/
*/

static char *nicookie_osx_safari_file(char *buf, size_t size, FILE *stream);
static char *nicookie_osx_safari_search_page(char *buf, size_t size,
    FILE *stream);
static char *nicookie_osx_safari_search_cookie(char *buf, size_t size,
    FILE *stream);

char *nicookie_osx_safari(char *buf, size_t size) {
  const char *restrict suffix_cookies_path = SAFARI_COOKIE_FILE_SUFFIX;
  const size_t suffix_cookies_path_len = strlen(suffix_cookies_path);
  const char *restrict home_path = getenv("HOME");
  if (home_path == NULL) return NULL;
  const size_t home_path_len = strlen(home_path);
  size_t cookies_path_list_len = suffix_cookies_path_len + home_path_len + 1;
  char cookies_path[cookies_path_list_len];
  strcpy(cookies_path, home_path);
  strcpy(cookies_path + home_path_len, suffix_cookies_path);

  FILE *cookies_file = fopen(cookies_path, "rb");
  if (cookies_file == NULL) return NULL;
  char *result = nicookie_osx_safari_file(buf, size, cookies_file);
  fclose(cookies_file);
  return result;
}

static char *nicookie_osx_safari_file(char *buf, size_t size, FILE *stream) {
  const char *restrict headr_signature = "cook";
  const size_t headr_signature_len = 4;

  // Signature
  for (int i = 0; i < headr_signature_len; i++) {
    if (getc(stream) != headr_signature[i]) {
      errno = EILSEQ;
      return NULL;
    }
  }
  if (ferror(stream) || feof(stream)) {
    errno = EILSEQ;
    return NULL;
  }

  // No. of pages
  uint32_t page_num = fget_uint32_be(stream);
  if (page_num == 0) {
    if (ferror(stream) || feof(stream)) {
      errno = EILSEQ;
    } else {
      errno = ENOENT;
    }
    return NULL;
  }

  // Page N Size
  uint32_t page_size[page_num];
  for (int i = 0; i < page_num; i++) {
    page_size[i] = fget_uint32_be(stream);
  }
  if (ferror(stream) || feof(stream)) {
    errno = EILSEQ;
    return NULL;
  }

  // Page N
  char *result = NULL;
  for (int i = 0; i < page_num; i++) {
    fpos_t pos;
    fgetpos(stream, &pos);
    result = nicookie_osx_safari_search_page(buf, size, stream);
    if (result != NULL) return result;
    fsetpos(stream, &pos);
    fmove_uint32(stream, page_size[i]);
  }

  return NULL;
}

static char *nicookie_osx_safari_search_page(char *buf, size_t size,
    FILE *stream) {
  fpos_t begin_pos;
  if (fgetpos(stream, &begin_pos) != 0) {
    errno = EILSEQ;
    return NULL;
  }

  // Page Header
  uint32_t page_header = fget_uint32_be(stream);
  if (page_header != 0x00000100) {
    errno = EILSEQ;
    return NULL;
  }

  // No. of cookies
  uint32_t cookie_num = fget_uint32_le(stream);

  // Cookie N offset
  uint32_t cookie_offset[cookie_num];
  for (int i = 0; i < cookie_num; i++) {
    cookie_offset[i] = fget_uint32_le(stream);
  }

  // Cookie N
  char *result = NULL;
  for (int i = 0; i < cookie_num; i++) {
    fsetpos(stream, &begin_pos);
    fmove_uint32(stream, cookie_offset[i]);
    result = nicookie_osx_safari_search_cookie(buf, size, stream);
    if (result != NULL) return result;
  }

  return NULL;
}

static char *nicookie_osx_safari_search_cookie(char *buf, size_t size,
    FILE *stream) {
  fpos_t begin_pos;
  if (fgetpos(stream, &begin_pos) != 0) {
    errno = EILSEQ;
    return NULL;
  }

  uint32_t cookie_size = fget_uint32_le(stream);
  fget_uint32_le(stream);
  uint32_t flags = fget_uint32_le(stream);
  fget_uint32_le(stream);
  uint32_t url_offset = fget_uint32_le(stream);
  uint32_t name_offset = fget_uint32_le(stream);
  uint32_t path_offset = fget_uint32_le(stream);
  uint32_t value_offset = fget_uint32_le(stream);

  // check url
  const char *restrict url_str = ".nicovideo.jp";
  fsetpos(stream, &begin_pos);
  fmove_uint32(stream, url_offset);
  if (!fcheckstr(stream, url_str)) return NULL;

  // check name
  const char *restrict name_str = "user_session";
  fsetpos(stream, &begin_pos);
  fmove_uint32(stream, name_offset);
  if (!fcheckstr(stream, name_str)) return NULL;

  // check path
  const char *restrict path_str = "/";
  fsetpos(stream, &begin_pos);
  fmove_uint32(stream, path_offset);
  if (!fcheckstr(stream, path_str)) return NULL;

  // copy value
  fsetpos(stream, &begin_pos);
  fmove_uint32(stream, value_offset);
  size_t value_size = fsizetonull(stream);
  if (size == 0 && buf == NULL) {
    buf = malloc(value_size);
    if (buf == NULL) return NULL;
    size = value_size;
  }

  if (value_size > size) {
    errno = ERANGE;
    return NULL;
  }

  if (buf == NULL) {
    buf = malloc(size);
    if (buf == NULL) return NULL;
  }

  return fgetstr(stream, value_size, buf);
}
