// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/mem.h
#ifndef _C_MEM_H
#define _C_MEM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *memcpy(void *dst, const void *src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *dest, int val, size_t n);
void *memchr(const void *src, int val, size_t n);
int memcmp(const void *src1, const void *src2, size_t n);

#ifdef __cplusplus
}
#endif

#endif
