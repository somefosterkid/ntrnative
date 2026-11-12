// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/wchar_io.h
#ifndef _C_WCHAR_H
#define _C_WCHAR_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int fwide(FILE *stream, int mode);

#ifdef __cplusplus
}
#endif

#endif
