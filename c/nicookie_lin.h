/*
 * nicookie_lin.h
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#ifndef nicookie_lin_H
#define nicookie_lin_H
#include <stddef.h>

char *nicookie_lin(char *buf, size_t size, int browser);
char *nicookie_lin_chrome(char *buf, size_t size);
char *nicookie_lin_firefox(char *buf, size_t size);
char *nicookie_lin_safari(char *buf, size_t size);
// char *nicookie_lin_opera(char *str, size_t n);

#endif // nicookie_lin_H
