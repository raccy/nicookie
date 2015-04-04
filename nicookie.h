/*
 * nicookie.h
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#ifndef nicookie_H
#define nicookie_H

#include <stddef.h>

// TODO: 後から考える
#define EXPORT

#define NICOOKIE_NO_BROWSER                 0
#define NICOOKIE_BROWSER_INTERNET_EXPLORER  1
#define NICOOKIE_BROWSER_FIREFOX            2
#define NICOOKIE_BROWSER_CHROME             4
#define NICOOKIE_BROWSER_SAFARI             8
#define NICOOKIE_BROWSER_OPERA             16
#define NICOOKIE_ALL_BROWSER              255

#ifdef __cplusplus
extern "C" {
#endif

EXPORT const char *nicookie_version(void);
EXPORT char *nicookie_get_cookie(char *buf, size_t size, int browser);
EXPORT const int nicookie_browser_list(void);
EXPORT const char *nicookie_browser_name(int browser);

#ifdef __cplusplus
}
#endif
#endif // nicookie_H
