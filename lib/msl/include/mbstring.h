// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/mbstring.h
#ifndef _C_MBSTRING_H
#define _C_MBSTRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int mbtowc(wchar_t *pwc, const char *s, int n);
int __mbtowc_noconv(wchar_t *pwc, const char *str, int n);
int __wctomb_noconv(char *s, wchar_t wchar);
size_t mbstowcs(wchar_t *pDest, const char *pSrc, size_t num);
size_t wcstombs(char *s, const wchar_t *pwcs, size_t n);

#ifdef __cplusplus
}
#endif

#endif
