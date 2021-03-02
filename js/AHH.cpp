#include "AHH.h"


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