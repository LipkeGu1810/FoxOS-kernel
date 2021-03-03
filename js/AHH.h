#ifndef AHH_H
#define AHH_H

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#ifdef __cplusplus
#define EXPOSEC extern "C"
#else
#define EXPOSEC
#endif

#define ZEROPAD	1
#define SIGN	2
#define PLUS	4
#define SPACE	8
#define LEFT	16
#define SMALL	32
#define SPECIAL	64

#define INFINITY (__builtin_inff())
#define NAN (__builtin_nanf (""))

#define isdigit(c) (c >= '0' && c <= '9')

#define __do_div(n, base) ({					\
int __res;										\
__res = ((unsigned long) n) % (unsigned) base;	\
n = ((unsigned long) n) / (unsigned) base;		\
__res; })

typedef struct {
	uint64_t	rbx;
	uint64_t	rsp;
	uint64_t	rbp;

	uint64_t	rdi;
	uint64_t	rsi;
	uint64_t	r12;
	uint64_t	r13;
	uint64_t	r14;
	uint64_t	r15;
	uint64_t	rip;
	uint64_t	rxCsr;
	uint8_t	xmm_buffer[160];
} __attribute__((aligned(8))) jmp_buf[1];

extern unsigned int setjmp(jmp_buf env) __attribute__((returns_twice));
extern void longjmp(jmp_buf env, unsigned int value) __attribute__((noreturn));

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
EXPOSEC void exit(int num);
EXPOSEC void exit(int num);
EXPOSEC void puts(char* str);
EXPOSEC int sprintf(char *buf, const char *fmt, ...);
EXPOSEC int printf(const char *fmt, ...);
EXPOSEC const char* strstr(const char* X, const char* Y);
EXPOSEC char* strchr(const char* s, int c);
EXPOSEC char* strrchr (const char* s, int c);
EXPOSEC void not_implemented(const char* __file, unsigned int __line);
EXPOSEC int atoi(char* str);
EXPOSEC int strncmp( const char * s1, const char * s2, size_t n );
EXPOSEC void putchar(char c);
EXPOSEC bool isfinite(double __x);
EXPOSEC bool signbit(double __x);

#define assert(expr)														\
	((void) sizeof ((expr) ? 1 : 0), __extension__ ({						\
		if (expr)															\
			;																\
		else																\
			__assert_fail (#expr, __FILE__, __LINE__);						\
	}))

#define uh_oh() not_implemented(__FILE__, __LINE__);

#endif