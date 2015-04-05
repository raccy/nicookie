/*
 * nicookie_osx_firefox.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */

#include <stdio.h>

#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_util.h"
#include "nicookie_osx.h"
#include "nicookie_firefox.h"

// #define
// OSX_FIREFOX_PROFILE_COOKIE_FILE_SUFFIX
// "${HOME}/Library/Application Support/Firefox/profiles.ini"
// "${HOME}/.mozilla/firefox/profiles.ini"
// "${APPDATA}\\Mozilla\\Firefox\\profiles.ini"

char *nicookie_osx_firefox(char *buf, size_t size) {
  char *profile_ini = nicookie_str_with_env(
      "%s/Library/Application Support/Firefox/profiles.ini", "HOME");
  if (profile_ini == NULL) return NULL;
  nicookie_debug_str(profile_ini);

  char *result = nicookie_firefox_profiles(buf, size, profile_ini);
  free(profile_ini);

  return result;
}
