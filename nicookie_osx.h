/*
 * nicookie_osx.h
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */
#ifndef nicookie_osx_H
#define nicookie_osx_H
#include <stddef.h>

// opera
// https://gist.github.com/gwarser/1324501

char *nicookie_osx(char *buf, size_t size, int browser);
char *nicookie_osx_chrome(char *buf, size_t size);
char *nicookie_osx_firefox(char *buf, size_t size);
char *nicookie_osx_safari(char *buf, size_t size);
// char *nicookie_osx_opera(char *str, size_t n);

#endif // nicookie_osx_H
