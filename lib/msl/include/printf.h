// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/printf.h
#ifndef _C_PRINTF_H
#define _C_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>

int printf(const char *format, ...);
int fprintf(FILE *, const char *format, ...);
int vprintf(const char *, va_list);
int vsnprintf(char *, size_t, const char *, va_list);
int vsprintf(char *, const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int sprintf(char *, const char *, ...);

#ifdef __cplusplus
}
#endif

#endif
