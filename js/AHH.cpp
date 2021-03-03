#include "AHH.h"

#include <renderer/font_renderer.h>

EXPOSEC void* memset(void* buf, int c, int n){
	unsigned char* p = (unsigned char*) buf;

	while (n--) {
		*p++ = c;
	}

	return buf;
}

EXPOSEC void* memcpy(void* dest, const void* src, size_t n){
	unsigned char* d = (unsigned char*) dest;
	const unsigned char* s = (unsigned char*) src;

	while (n--) {
		*d++ = *s++;
	}

	return dest;
}



EXPOSEC int strcmp(char *str1, char *str2){
	int i = 0;
	int failed = 0;
	while(str1[i] != '\0' && str2[i] != '\0'){
		if(str1[i] != str2[i]){
			failed = 1;
			break;
		}
		i++;
	}
	if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
		failed = 1;
	return failed;
}

EXPOSEC char *strcpy(char *dest, const char *src){
	do{
		*dest++ = *src++;
	} while (*src != 0);
	return 0;
}

EXPOSEC int strlen(char *src){
	int i = 0;
	while (*src++)
		i++;
	return i;
}

EXPOSEC char *strcat(char *dest, const char *src){
    int i;
    int j;

    for (i = 0; dest[i] != '\0'; i++);
    for (j = 0; src[j] != '\0'; j++) {
        dest[i+j] = src[j];
    }

    dest[i+j] = '\0';

    return dest;
}

EXPOSEC double fabs(double num) {
	if (num < 0) return -num;
	return num;
}

EXPOSEC double sqrt(double num) {
	double i = num / 2;
	if (num < 0) return 0;
	while (fabs(i - (num / i)) / i > 0.000000000000001) {
		i = (i + (num / i)) / 2;
	}
	return i;
}

EXPOSEC double ceil(double num) {
	if (num < 0) return (int) num;
	return ((int) num) + 1;
}

EXPOSEC double floor(double x) {
	if (x > 0) return (int) x;
	return (int)(x - 0.9999999999999999);
}

EXPOSEC double fmod(double a, double b) {
	return (int)((((a / b) - ((int)(a / b))) * b) + 0.5);
}

EXPOSEC double pow(double x,double y) {
    double result = 1;
    for (int i=0;i<y;i++)
        result *= x;
    return result;
}

EXPOSEC void __assert_fail(const char* __assertion, const char* __file, unsigned int __line) {
	renderer::global_font_renderer->printf("ASSERT: %s\nFAIL: %s line %d\n", __assertion);
	while(1);
}

EXPOSEC void exit(int num) {
	renderer::global_font_renderer->printf("EXIT: %d\n", num);
	while(1);
}