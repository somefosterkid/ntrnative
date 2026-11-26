#include "string_util.h"

#include "global.h"

#include "constants/charcode.h"

u16 *CopyU16StringArray(u16 *dest, const u16 *src) {
    while (*src != EOS) {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = EOS;
    return dest;
}

u16 *CopyU16StringArrayN(u16 *dest, const u16 *src, u32 n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = src[i];
    }
    dest += n;
    return dest;
}

int StringLength(const u16 *str) {
    int i;
    for (i = 0; str[i] != EOS; i++) {}
    return i;
}

BOOL StringNotEqual(const u16 *a, const u16 *b) {
    while (*a == *b) {
        if (*a == EOS) {
            return FALSE;
        }
        a++;
        b++;
    }
    return TRUE;
}

BOOL StringNotEqualN(const u16 *a, const u16 *b, u32 n) {
    while (*a == *b) {
        if (n == 0) {
            return FALSE;
        }
        if (*a == EOS && *b == EOS) {
            return FALSE;
        }
        a++;
        b++;
        n--;
    }
    return TRUE;
}

u16 *StringFill(u16 *str, u16 val, u32 n) {
    int i;
    for (i = 0; i < n; i++) {
        str[i] = val;
    }
    return str + i;
}

u16 *StringFillEOS(u16 *str, u32 n) {
    return StringFill(str, EOS, n);
}

static u32 hgasset_ConvertUIntToDecimalString__power_of_10[] __attribute__((section(BWPORT_DATA_SECTION))) = {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };

u16 *ConvertUIntToDecimalString(u16 *dest, u32 num, PrintingMode strconvmode, u32 ndigits) {
    static const u16 _digit_strcode[] = {
        CHAR_JP_0, CHAR_JP_1, CHAR_JP_2, CHAR_JP_3, CHAR_JP_4, CHAR_JP_5, CHAR_JP_6, CHAR_JP_7, CHAR_JP_8, CHAR_JP_9, CHAR_JP_A, CHAR_JP_B, CHAR_JP_C, CHAR_JP_D, CHAR_JP_E, CHAR_JP_F

    };
    
    u32 denom;
    u16 digit;

    for (denom = hgasset_ConvertUIntToDecimalString__power_of_10[ndigits - 1]; denom != 0; denom /= 10) {
        digit = num / denom;
        num -= denom * digit;
        if (strconvmode == PRINTING_MODE_LEADING_ZEROS) {
            *dest = (digit >= 10) ? CHAR_JP_QUESTION_MARK : _digit_strcode[digit];
            dest++;
        } else if (digit != 0 || denom == 1) {
            strconvmode = PRINTING_MODE_LEADING_ZEROS;
            *dest = (digit >= 10) ? CHAR_JP_QUESTION_MARK : _digit_strcode[digit];
            dest++;
        } else if (strconvmode == PRINTING_MODE_RIGHT_ALIGN) {
            *dest = CHAR_JP_SPACE;
            dest++;
        }
    }
    *dest = EOS;
    return dest;
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_string_util_c_0(void) {
    PORT_AssetAdd("IPKE", "src/string_util.c:hgasset_ConvertUIntToDecimalString__power_of_10", (void *)&hgasset_ConvertUIntToDecimalString__power_of_10, sizeof(hgasset_ConvertUIntToDecimalString__power_of_10), "u8");
    PORT_AssetAdd("IPGE", "src/string_util.c:hgasset_ConvertUIntToDecimalString__power_of_10", (void *)&hgasset_ConvertUIntToDecimalString__power_of_10, sizeof(hgasset_ConvertUIntToDecimalString__power_of_10), "u8");
}
