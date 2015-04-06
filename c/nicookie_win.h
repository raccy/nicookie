/*
 * nicookie_win.h
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#ifndef nicookie_win_H
#define nicookie_win_H
#include <stddef.h>

char *nicookie_win(char *buf, size_t size, int browser);
// char *nicookie_win_ie(char *buf, size_t size);
// char *nicookie_win_chrome(char *buf, size_t size);
char *nicookie_win_firefox(char *buf, size_t size);
// char *nicookie_win_opera(char *str, size_t n);

#endif // nicookie_win_H
