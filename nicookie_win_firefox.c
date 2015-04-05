/*
 * nicookie_win_firefox.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */

#include <stdio.h>

#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_util.h"
#include "nicookie_win.h"
#include "nicookie_firefox.h"

// "${APPDATA}\\Mozilla\\Firefox\\profiles.ini"

char *nicookie_win_firefox(char *buf, size_t size) {
  char *profile_ini = nicookie_str_with_env(
      "%s\\Mozilla\\Firefox\\profiles.ini", "APPDATA");
  if (profile_ini == NULL) return NULL;
  nicookie_debug_str(profile_ini);

  char *result = nicookie_firefox_profiles(buf, size, profile_ini);
  free(profile_ini);

  return result;
}
