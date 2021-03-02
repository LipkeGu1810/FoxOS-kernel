#ifndef AHH_H
#define AHH_H

#include <stddef.h>
#include <float.h>

#ifdef __cplusplus
#define EXPOSEC extern "C"
#else
#define EXPOSEC
#endif

#define INFINITY (__builtin_inff())
#define NAN (__builtin_nanf (""))

EXPOSEC void* memset(void* buf, int c, int n);
EXPOSEC void* memcpy(void* dest, const void* src, size_t n);
EXPOSEC int strcmp(char *str1, char *str2);
EXPOSEC char *strcpy(char *dest, const char *src);
EXPOSEC int strlen(char *src);

#endif