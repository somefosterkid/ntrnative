// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/limits.h, as it is (tools/port_lib.py)
#ifndef _C_LIMITS_H
#define _C_LIMITS_H

#ifdef __cplusplus
extern "C" {
#endif

#define SCHAR_MIN (-0x7F - 1)
#define SCHAR_MAX 0x7F
#define UCHAR_MAX 0xFFU

#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX

#define SHRT_MIN (-0x7FFF - 1)
#define SHRT_MAX 0x7FFF
#define USHRT_MAX 0xFFFF

#define INT_MIN (-0x7FFFFFFF - 1)
#define INT_MAX 0x7FFFFFFF
#define UINT_MAX 0xFFFFFFFF

#define LONG_MIN (-0x7FFFFFFFL - 1)
#define LONG_MAX 0x7FFFFFFFL
#define ULONG_MAX 0xFFFFFFFFUL

#define LLONG_MIN (-0x7FFFFFFFFFFFFFFFLL - 1)
#define LLONG_MAX 0x7FFFFFFFFFFFFFFFLL
#define ULLONG_MAX 0xFFFFFFFFFFFFFFFFULL

#define CHAR_BIT 8

#ifdef __cplusplus
}
#endif

#endif
