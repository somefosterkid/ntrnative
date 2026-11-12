// NitroSDK's STD library (std_string.c): the prototypes of Black's functions.
#ifndef BWPORT_NITRO_STD_H
#define BWPORT_NITRO_STD_H

#include "types.h"
#include "stdarg.h"

// Copies the string with its terminator, returning dest
char *STD_CopyString(char *dest, const char *src);
// Compares two strings, returning the difference of their first unequal bytes: pokebw2's STD_CompareString
int STD_CompareString(const char *a, const char *b);
// NitroSDK's STD_GetStringNLength: the length of a string, at most maxlen
int func_0208ccfc(const char *str, int maxlen);

int STD_TSScanf(const char *src, const char *fmt, ...);

// TwlSDK's STD_IsSjisLeadByte: whether a byte begins a two-byte Shift_JIS character. STD_IsSjisTrailByte: whether a
// byte can be the second byte of one. STD_IsSjisCharacter: whether a string starts with one
#define STD_IsSjisLeadByte(c) ((u32)(((u8)(c) ^ 0x20) - 0xa1) < 0x3c)

#define STD_IsSjisTrailByte(c) ((c) != 0x7f && (u8)((c) - 0x40) <= 0xbc)

// Where sub is in str, or NULL
char *STD_SearchString(const char *str, const char *sub);
// The same for the first len characters
int STD_CompareNString(const char *a, const char *b, int len);
// The same with the pointers in a list
int STD_TVSScanf(const char *src, const char *fmt, va_list args);

#endif // BWPORT_NITRO_STD_H
