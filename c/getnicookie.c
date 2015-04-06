/*
 * getnicookie.c - Sample C Source
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "nicookie.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("NICOOKIE Get Niconico Cookie\n");
    printf("Usage: %s browser\n", argv[0]);
    printf("Nicookie Library Version: %s\n", nicookie_version());
    printf("Browser list:\n");
    for (int i = 0; i < 8; i++) {
      int browser_id = nicookie_browser_list() & (1<<i);
      if (browser_id != NICOOKIE_NO_BROWSER) {
        printf("  %s\n", nicookie_browser_name(browser_id));
      }
    }
    return 0;
  }

  char *name = argv[1];
  int browser = NICOOKIE_NO_BROWSER;
  for (int i = 0; i < 8; i++) {
    int browser_id = nicookie_browser_list() & (1<<i);
    if ((browser_id != NICOOKIE_NO_BROWSER) &&
        (strcasestr(nicookie_browser_name(browser_id), name))) {
      browser = browser_id;
      break;
    }
  }

  if (browser == NICOOKIE_NO_BROWSER) {
    printf("Unknown browser\n");
    return 1;
  }

  char *result = nicookie_get_cookie(NULL, 0, browser);
  if (result == NULL) {
    printf("NG: %s\n", strerror(errno));
  } else {
    printf("COOKIE: %s\n", result);
    free(result);
  }
  return 0;
}
