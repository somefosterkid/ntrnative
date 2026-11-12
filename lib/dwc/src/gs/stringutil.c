// Black's code at ov009 0x021800cc-0x02180404. UTF-8 to UCS-2 conversions of the header stringutil.h
// (the UTF8 and UCS2 functions it declares).

#include "types.h"
#include "gs/nonport.h"
#include "string.h"

s32 _UTF8ToUCS2ConversionLengthOnly(s8 *str, s32 len);
int func_ov009_02180334(const char *utf8, int length, u16 *ucs2, int count);

u16 *func_ov009_021802ec(u8 *param_1, s32 param_2);

s32 _ReadUCS2CharFromUTF8String(s8 *str, s32 len, u16 *out) {
    if (len == 0) {
        *out = '?';
        return 0;
    }
    if ((u8)str[0] <= 0x7F) {
        *out = str[0];
        return 1;
    }
    if (((u8)str[0] & 0xE0) == 0xC0) {
        if (len < 2) {
            *out = '?';
            return 0;
        }
        if (((u8)str[1] & 0xC0) == 0x80) {
            *out = ((str[0] & 0x1F) << 6) + (str[1] & 0x3F);
            return 2;
        }
    } else if (((u8)str[0] & 0xF0) == 0xE0) {
        if (len < 3) {
            *out = '?';
            return 0;
        }
        if (((u8)str[1] & 0xC0) == 0x80 && ((u8)str[2] & 0xC0) == 0x80) {
            *out = (str[2] & 0x3F) + (((str[0] & 0x0F) << 12) + ((str[1] & 0x3F) << 6));
            return 3;
        }
    }
    *out = '?';
    return 1;
}

u32 _UCS2CharToUTF8String(u16 c, char *utf8) {
    if (c <= 0x7f) {
        utf8[0] = c;
        return 1;
    }
    if (c <= 0x7ff) {
        utf8[0] = 0xc0 | c >> 6;
        utf8[1] = 0x80 | (c & 0x3f);
        return 2;
    }
    utf8[0] = 0xe0 | c >> 12;
    utf8[1] = 0x80 | (c >> 6 & 0x3f);
    utf8[2] = 0x80 | (c & 0x3f);
    return 3;
}

s32 _UTF8ToUCS2ConversionLengthOnly(s8 *str, s32 len) {
    s32 count = 0;

    if (str == NULL) {
        return 0;
    }
    while (*str != 0 && len > 0) {
        if (((u8)*str & 0xE0) == 0xC0 && len >= 2 && ((u8)str[1] & 0xC0) == 0x80) {
            str += 2;
            len -= 2;
        } else if (((u8)*str & 0xF0) == 0xE0 && len >= 3 && ((u8)str[1] & 0xC0) == 0x80 && ((u8)str[2] & 0xC0) == 0x80) {
            str += 3;
            len -= 3;
        } else {
            str += 1;
            len -= 1;
        }
        count++;
    }
    return count;
}

u16 *UTF8ToUCS2StringAlloc(s8 *arg0) {
    if (arg0 == NULL) {
        return NULL;
    }
    return func_ov009_021802ec((u8 *) arg0, (s32) strlen(arg0));
}

#pragma thumb off
u16 *func_ov009_021802ec(u8 *param_1, s32 param_2) {
    s32 result;
    u16 *ptr;

    if (param_1 != (u8 *)0x0) {
        result = _UTF8ToUCS2ConversionLengthOnly(param_1, param_2);
        ptr = gsimalloc((result + 1) * 2);
        func_ov009_02180334(param_1, param_2, (s16 *)ptr, result);
        return ptr;
    }
    return (u16 *)0x0;
}

int func_ov009_02180334(const char *utf8, int length, u16 *ucs2, int count) {
    int converted = 0;
    int used = 0;
    u16 *write = ucs2;
    int size;

    if (utf8 == NULL) {
        *ucs2 = 0;
        return 1;
    }
    while (*(u8 *)utf8 != 0 && count + 1 > converted && length > used) {
        size = _ReadUCS2CharFromUTF8String(utf8, length - used, write);
        if (size == 0) {
            *ucs2 = 0;
            return 0;
        }
        used += size;
        utf8 += size;
        converted++;
        write++;
    }
    *write = 0;
    return converted + 1;
}

#pragma thumb on

#pragma thumb off
u32 func_ov009_021803bc(void *param_1, s8 *param_2, s32 param_3) {
    size_t n;

    n = strlen(param_2);
    if (n > param_3 - 1U) {
        n = param_3 - 1U;
    }
    memcpy(param_1, param_2, n);
    *(u8 *)((s32)(u8 *)param_1 + n) = 0;
    return n;
}
#pragma thumb on
