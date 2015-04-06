/*
 * nicookie_osx.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#include <stddef.h>
#include <errno.h>
#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_osx.h"


char *nicookie_osx(char *buf, size_t size, int browser) {
  browser &= nicookie_browser_list();
  char *result = NULL;
  errno = ENOSYS;
  for (int i = 0; i < 8 && result == NULL; i++) {
    int browser_id = browser & (1<<i);
    switch (browser_id) {
      case NICOOKIE_BROWSER_FIREFOX:
        result = nicookie_osx_firefox(buf, size);
        break;
      case NICOOKIE_BROWSER_CHROME:
        result = nicookie_osx_chrome(buf, size);
        break;
      case NICOOKIE_BROWSER_SAFARI:
        result = nicookie_osx_safari(buf, size);
        break;
      case NICOOKIE_BROWSER_OPERA:
        errno = ENOSYS;
        break;
      default:
        break;
    }
  }
  return result;
}
