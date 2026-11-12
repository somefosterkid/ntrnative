// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/scanf.h, as it is (tools/port_lib.py)
#ifndef _C_SCANF_H
#define _C_SCANF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

int vsscanf(const char *, const char *, va_list);
int sscanf(const char *, const char *, ...);

#ifdef __cplusplus
}
#endif

#endif
