// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/string.h, with Black's
// additions
#ifndef _C_STRING_H
#define _C_STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// MSL's memory functions, which its string.h declares too: MWCC builds memcpy and memset of a known size in place
// only when their first declaration is this one, from a header named string.h, with size_t sizes (mem.h's are the
// same prototypes, and leave a call)
void *memcpy(void *dst, const void *src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *dest, int val, size_t n);
void *memchr(const void *src, int val, size_t n);
int memcmp(const void *src1, const void *src2, size_t n);

size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t num);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t num);
char *strcat(char *dest, const char *src);
char *strchr(const char *str, int chr);
size_t strcspn(const char *str, const char *set);
char *strstr(const char *str, const char *pat);

// MSL's extras (string_extras.c)
int stricmp(const char *s1, const char *s2);
int strnicmp(const char *s1, const char *s2, int n);
int strcmpi(const char *s1, const char *s2);

#ifdef __cplusplus
}
#endif

#endif
