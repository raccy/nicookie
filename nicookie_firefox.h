/*
 * nicookie_firefox.h
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#ifndef nicookie_firefox_H
#define nicookie_firefox_H

#include <stddef.h>

char *nicookie_firefox_cookies_path(const char *profile);
char *nicookie_firefox_cookies(char *buf, size_t size,
    const char *cookies_path);

#endif // nicookie_firefox_H
