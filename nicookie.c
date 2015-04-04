/*
 * nicookie.c
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
#include "nicookie_config.h"

#ifdef _WIN32
#include "nicookie_win.h"
#endif // _WIN32
#ifdef __APPLE__
#include "nicookie_osx.h"
#endif // __APPLE__
#ifdef __linux__
#include "nicookie_linux.h"
#endif // __linux__

EXPORT const char *nicookie_version(void) {
  return NICOOKIE_VERSION;
}

EXPORT char *nicookie_get_cookie(char *buf, size_t size, int  browser) {
#ifdef _WIN32
  errno = ENOSYS;
  return NULL;
#endif // _WIN32
#ifdef __APPLE__
  return nicookie_osx(buf, size, browser);
#endif // __APPLE__
#ifdef __linux__
  errno = ENOSYS;
  return NULL;
#endif // __linux__
}

EXPORT const char *nicookie_browser_name(int browser) {
  switch(browser) {
  case NICOOKIE_BROWSER_INTERNET_EXPLORER:
    return "Internet Explorer";
  case NICOOKIE_BROWSER_FIREFOX:
    return "Mozilla Firefox";
  case NICOOKIE_BROWSER_CHROME:
    return "Google Chrome";
  case NICOOKIE_BROWSER_SAFARI:
    return "Safari";
  case NICOOKIE_BROWSER_OPERA:
    return "Opera";
  default:
    return "Unknown";
  }
}

EXPORT const int nicookie_browser_list(void) {
#ifdef _WIN32
  return NICOOKIE_NO_BROWSER;
#endif // _WIN32
#ifdef __APPLE__
  return NICOOKIE_BROWSER_SAFARI | NICOOKIE_BROWSER_FIREFOX | NICOOKIE_BROWSER_CHROME;
#endif // __APPLE__
#ifdef __linux__
  return NICOOKIE_NO_BROWSER;
#endif // __linux__
}
