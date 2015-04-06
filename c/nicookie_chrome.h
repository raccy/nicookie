/*
 * nicookie_chrome.h
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#ifndef nicookie_chrome_H
#define nicookie_chrome_H

#include <stddef.h>

char *nicookie_chrome_cookies(char *buf, size_t size,
    const char *cookies_path, char *(*decrypt_func)(const char *, int));

#endif // nicookie_chrome_H
