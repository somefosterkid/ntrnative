// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/stdlib.h, with Black's
// additions
#ifndef _C_STRLIB_H
#define _C_STRLIB_H

#include <stddef.h>

int abs(int n);
// Registers a function to run at program exit
int atexit(void (*func)(void));

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *calloc(size_t count, size_t size);

double strtod(const char *str, char **end);
double atof(const char *str);
unsigned long strtoul(const char *str, char **end, int base);
long strtol(const char *str, char **end, int base);
long long strtoll(const char *str, char **end, int base);
long atol(const char *str);
long long atoll(const char *str);
// The readers of strtoul.c's functions and of scanf
unsigned long __strtoul(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg,
                        int *chars_scanned, int *negative, int *overflow);
unsigned long long __strtoull(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg,
                              int *chars_scanned, int *negative, int *overflow);
long double __strtold(int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, int *chars_scanned,
                      int *overflow);

void qsort(void *base, size_t count, size_t size, int (*compare)(const void *, const void *));
int rand(void);
void srand(unsigned int seed);

#endif
