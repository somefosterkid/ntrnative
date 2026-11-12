// Black's code at ov114 0x021ba100-0x021ba7dc. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/nhttp_internal.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
#define TO_LOWER(c) ((((c) >= 'A') & ((c) <= 'Z')) ? (c) + ('a' - 'A') : (c))

#pragma thumb off
void func_ov114_021ba100(void *dest, const void *src, u32 size) {
    MI_CpuCopy8(src, dest, size);
}

int func_ov114_021ba118(const char *s) {
    return STD_GetStringLength(s);
}

int func_ov114_021ba124(const char *s, int max) {
    return func_0208ccfc(s, max);
}

int func_ov114_021ba130(const char *a, const char *b) {
    return STD_CompareString(a, b);
}

void func_ov114_021ba13c(void *dest, u32 size) {
    MI_CpuFill8(dest, 0, size);
}

// Compares two strings up to a size without caring for the case; 0 if they are equal, else the size that was left
int func_ov114_021ba150(const char *a, const char *b, int size) {
    int cb;
    int ca;

    while (size > 0) {
        ca = *a++;
        cb = *b++;
        if ((ca == 0 || cb == 0) && ca == 0 && cb == 0) {
            size = 0;
            break;
        }
        cb = TO_LOWER(cb);
        ca = TO_LOWER(ca);
        if (ca != cb) {
            break;
        }
        size--;
    }
    return size;
}

// The size of a string once it is URL encoded: a letter, a digit or a space is one byte and the rest three
int NHTTPi_getUrlEncodedSize(const char *s) {
    int size = 0;
    char c = *s++;

    while (c != 0) {
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ') {
            size += 1;
        } else {
            size += 3;
        }
        c = *s++;
    }
    return size;
}

int func_ov114_021ba25c(const char *s, int length) {
    int i = 0;
    int size = 0;
    char c = *s++;

    if (length > 0) {
        for (; i < length; i++) {
            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ') {
                size += 1;
            } else {
                size += 3;
            }
            c = *s++;
        }
    }
    return size;
}

int NHTTPi_encodeUrlChar(char *p, int c) {
    s32 hi;

    if (c == 0x20) {
        *p = 0x2b;
        return 1;
    }
    if ((c >= 0x30 && c <= 0x39) || (c >= 0x41 && c <= 0x5a) ||
        (c >= 0x61 && c <= 0x7a)) {
        *p = c;
        return 1;
    }
    hi = (c >> 4) & 0xf;
    c = c & 0xf;
    *p = 0x25;
    if (hi < 0xa) {
        p[1] = hi + 0x30;
    } else {
        p[1] = hi + 0x37;
    }
    if (c < 0xa) {
        p[2] = c + 0x30;
    } else {
        p[2] = c + 0x37;
    }
    return 3;
}

// Reads a number in hexadecimal, from at most 8 digits, with spaces before it and a space or the end after it, or -1
int func_ov114_021ba35c(const char *s, int size) {
    int i;
    int value;
    int started;

    if (size > 8) {
        return -1;
    }
    if (size == 8 && s[0] > '7') {
        return -1;
    }
    value = 0;
    started = 0;
    for (i = 0; i < size; i++) {
        char c = TO_LOWER(s[i]);

        if (c >= '0' && c <= '9') {
            value = (value << 4) + c - '0';
            started = 1;
        } else if (c >= 'a' && c <= 'f') {
            value = (value << 4) + c - 'a' + 10;
            started = 1;
        } else if (started && (c == ' ' || c == 0)) {
            break;
        } else if (started || c != ' ') {
            return -1;
        }
    }
    return value;
}

// Reads a number in decimal, from at most 10 digits, with spaces before it and a space or the end after it, or -1
int NHTTPi_strToInt(const char *s, int size) {
    int i;
    int value;
    int started;
    int before;

    if (size > 10) {
        return -1;
    }
    value = 0;
    started = 0;
    for (i = 0; i < size; i++) {
        char c = s[i];

        if (started && (c == ' ' || c == 0)) {
            break;
        }
        if (!started && c == ' ') {
            continue;
        }
        if (c < '0' || c > '9') {
            return -1;
        }
        before = value;
        started = 1;
        value = c + value * 10 - '0';
        if (before > value) {
            return -1;
        }
    }
    return value;
}

int func_ov114_021ba4e4(char *p, u32 n) {
    u32 rdx[9] = {0x3b9aca00, 0x5f5e100, 0x989680, 0xf4240, 0x186a0, 0x2710, 0x3e8, 0x64, 10};
    s32 i;
    s32 len = 0;
    s32 started = 0;
    u32 x;

    for (i = 0; i < 9; i++) {
        if (n >= rdx[i]) {
            x = n / rdx[i];
            p[len] = x + 0x30;
            started = 1;
            n -= x * rdx[i];
            len++;
        } else if (started) {
            p[len] = 0x30;
            len++;
        }
    }
    p[len] = n + 0x30;
    return len + 1;
}

static inline s32 NHTTPi_lower(s32 c) {
    if (c >= 0x41 && c <= 0x5a) {
        return c + 0x20;
    }
    return c;
}
// Compares a token, up to a space or the end of the first string, without caring for the case; 0 if they are equal
int func_ov114_021ba590(const char *a, const char *b) {
    while (TO_LOWER(*a) == TO_LOWER(*b)) {
        if (*a == 0 || *a == ' ') {
            return 0;
        }
        a++;
        b++;
    }
    return -1;
}

// Reads the digits of a string, leaving out what is not a digit, or -1 if there are none or more than 9
int NHTTPi_strtonum(const char *s, int size) {
    int count = 0;
    int value = 0;

    while (size-- != 0) {
        char c = *s;

        if (c != ' ' && c >= '0' && c <= '9') {
            value = c + value * 10 - '0';
            count++;
            if (count > 9) {
                return -1;
            }
        }
        s++;
    }
    return count == 0 ? -1 : value;
}

// Looks for a string in another one: 0 if it is there, -1 if not
int NHTTPi_memfind(const char *haystack, int haystackSize, const char *needle, int needleSize) {
    int i;
    int j;
    const char *p;

    if (haystackSize < needleSize) {
        return -1;
    }
    for (i = 0; i < haystackSize - needleSize + 1; i++) {
        if (haystack[i] == needle[0]) {
            p = haystack + i;
            for (j = 1; j < needleSize; j++) {
                if (p[j] != needle[j]) {
                    break;
                }
            }
            if (j == needleSize) {
                return 0;
            }
        }
    }
    return -1;
}

static char data_ov114_021ce3bc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int func_ov114_021ba714(char *param_1, const char *param_2) {
    s32 len;
    s32 i;
    const char *in = param_2;
    const char *tbl = data_ov114_021ce3bc;
    char *out = param_1;

    len = func_ov114_021ba118(param_2);
    i = 0;
    if (len > 0) {
        do {
            out[0] = tbl[in[0] >> 2];
            out[1] = tbl[((in[0] & 3) << 4) + (in[1] >> 4)];
            out[2] = tbl[((in[1] & 0xF) << 2) + (in[2] >> 6)];
            out[3] = tbl[in[2] & 0x3F];
            in += 3;
            out += 4;
            i += 3;
        } while (i < len);
    }
    if (i == len + 1) {
        out[-1] = 0x3D;
    } else if (i == len + 2) {
        out[-2] = 0x3D;
        out[-1] = 0x3D;
    }
    *out = 0;
    return func_ov114_021ba118(param_1);
}
