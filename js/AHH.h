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
EXPOSEC char *strcat(char *dest, const char *src);
EXPOSEC double fabs(double num);
EXPOSEC double sqrt(double num);
EXPOSEC double ceil(double num);
EXPOSEC double floor(double x);
EXPOSEC double fmod(double a, double b);
EXPOSEC double pow(double x,double y);
EXPOSEC void __assert_fail(const char* __assertion, const char* __file, unsigned int __line);

#define assert(expr)														\
	((void) sizeof ((expr) ? 1 : 0), __extension__ ({						\
		if (expr)															\
			;																\
		else																\
			__assert_fail (#expr, __FILE__, __LINE__);	\
	}))

#endif