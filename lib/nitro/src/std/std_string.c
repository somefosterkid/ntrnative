// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/std/src/std_string.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro.h>

char * STD_CopyString (char * destp, const char * srcp)
{
    char * retval = destp;

    SDK_ASSERT(destp && srcp);

    while (*srcp) {
        *destp++ = (char)*srcp++;
    }

    *destp = '\0';

    return retval;
}

int STD_CopyLString (char * dst, const char * src, int size)
{
    u32 align1;
    u32 align2;
    int i;
    const char * p;
    u32 word;
    u32 combined;

    align1 = (u32)dst & 3;
    align2 = (u32)src & 3;
    p = src;
    i = 0;
    if ((align1 ^ align2) == 0) {
        if (align1 != 0) {
            for (; i + align1 < 4 && i < size - 1; i++) {
                dst[i] = *p;
                if (*p == 0) {
                    goto tail;
                }
                p++;
            }
        }
        if (i < size - 4) {
            do {
                word = *(const u32 *)(src + i);
                combined = (word & 0x7f7f7f7f) + 0x7f7f7f7f | word | 0x7f7f7f7f;
                combined = ~combined;
                if (combined != 0) {
                    break;
                }
                *(u32 *)(dst + i) = word;
                i += 4;
            } while (i < size - 4);
        }
        p = src + i;
    }

    for (; i < size - 1; p++) {
        dst[i] = *p;
        if (*p == 0) {
            break;
        }
        i++;
    }

tail:
    if (i >= size - 1 && 0 < size) {
        dst[i] = 0;
    }
    if (*p != 0) {
        i += STD_GetStringLength(p);
    }
    return i;
}


char * STD_SearchString (const char * srcp, const char * str)
{
    int i;
    int n;

    SDK_ASSERT(srcp && str);

    for (i = 0; srcp[i]; i++) {
        n = 0;
        while (str[n] && srcp[i + n] == str[n]) {
            n++;
        }

        if (str[n] == '\0') {
            return (char *)&srcp[i];
        }
    }

    return NULL;
}

int STD_GetStringLength (const char * str)
{
    int n;
    const u32 * word;

    n = 0;
    while (((u32)str + n) & 3) {
        if (str[n] == 0) {
            return n;
        }
        n++;
    }

    word = (const u32 *)(str + n);
    while (TRUE) {
        u32 combined;
        combined = ((*word & 0x7f7f7f7f) + 0x7f7f7f7f) | *word | 0x7f7f7f7f;
        combined = ~combined;
        if (combined != 0) {
            break;
        }
        word++;
        n += 4;
    }

    while (str[n] != 0) {
        n++;
    }

    return n;
}

int func_0208ccfc (const char * str, int maxlen)
{
    int n = 0;
    const u32 *p;
    u32 word;
    u32 combined;

    while (((u32)(str + n) & 3) && n < maxlen) {
        if (str[n] == 0) {
            return n;
        }
        n++;
    }
    p = (const u32 *)(str + n);
    if (n < maxlen - 4) {
        do {
            word = *p;
            combined = (word & 0x7f7f7f7f) + 0x7f7f7f7f | word | 0x7f7f7f7f;
            combined = ~combined;
            if (combined != 0) {
                break;
            }
            p++;
            n += 4;
        } while (n < maxlen - 4);
    }
    while (str[n] != 0 && n < maxlen) {
        n++;
    }
    return n;
}

// From libntr (MIT, https://github.com/cybervisi0n/libntr), libraries/std/src/std_string.c: ported as it is, with credit (tools/ref_port.py)
int STD_CompareString (const char * str1, const char * str2)
{
    u32 align1;
    u32 align2;
    u32 word1;
    u32 b1;
    u32 b2;
    u32 combined;
    int i;

    align1 = (u32)str1 & 3;
    align2 = (u32)str2 & 3;
    i = 0;
    if ((align1 ^ align2) == 0) {
        if (align1 != 0) {
            for (; i + align1 < 4; i++) {
                b1 = *(u8 *)(str1 + i);
                b2 = *(u8 *)(str2 + i);
                if (b1 != b2 || b1 == 0) {
                    return b1 - b2;
                }
            }
        }
        while (TRUE) {
            word1 = *(u32 *)(str1 + i);
            if (word1 != *(u32 *)(str2 + i)) {
                break;
            }
            combined = (word1 & 0x7f7f7f7f) + 0x7f7f7f7f | word1 | 0x7f7f7f7f;
            combined = ~combined;
            if (combined != 0) {
                break;
            }
            i += 4;
        }
    }

    for (;; i++) {
        b1 = *(u8 *)(str1 + i);
        b2 = *(u8 *)(str2 + i);
        if (b1 != b2 || b1 == 0) {
            return b1 - b2;
        }
    }
}



int STD_CompareNString (const char * str1, const char * str2, int len)
{
    u32 align1;
    u32 align2;
    int i;
    u32 word1;
    u32 b1;
    u32 b2;
    u32 combined;

    if (len != 0) {
        align1 = (u32)str1 & 3;
        align2 = (u32)str2 & 3;
        i = 0;
        if ((align1 ^ align2) == 0) {
            if (align1 != 0) {
                for (; i + align1 < 4 && i < len; i++) {
                    b1 = *(u8 *)(str1 + i);
                    b2 = *(u8 *)(str2 + i);
                    if (b1 != b2 || b1 == 0) {
                        return b1 - b2;
                    }
                }
            }
            if (i <= len - 4) {
                do {
                    word1 = *(u32 *)(str1 + i);
                    if (word1 != *(u32 *)(str2 + i)) {
                        break;
                    }
                    combined = (word1 & 0x7f7f7f7f) + 0x7f7f7f7f | word1 | 0x7f7f7f7f;
                    combined = ~combined;
                    if (combined != 0) {
                        break;
                    }
                    i += 4;
                } while (i <= len - 4);
            }
        }

        for (; i < len; i++) {
            b1 = *(u8 *)(str1 + i);
            b2 = *(u8 *)(str2 + i);
            if (b1 != b2 || b1 == 0) {
                return b1 - b2;
            }
        }
    }

    return 0;
}

int STD_TVSScanf(const char *src, const char *fmt, va_list args);

int STD_TSScanf(const char *src, const char *fmt, ...)
{
    int ret;
    va_list vlist;
    va_start(vlist, fmt);
    ret = STD_TVSScanf(src, fmt, vlist);
    va_end(vlist);
    return ret;
}

BOOL STDi_IsSpace (int c)
{
    BOOL ret = FALSE;
    u32 bit = c - 9;
    if (bit > 0x17) {
        return ret;
    }
    if (0x80001f & (1 << bit)) {
        ret = TRUE;
    }
    return ret;
}

inline static u32 STDi_GetBitset (const u32 * bitset, u32 i)
{
    const u32 pos = (u32)(i >> 5UL);
    const u32 bit = (u32)(i & 31UL);

    return ((bitset[pos] >> bit) & 1UL);
}

inline static void STDi_SetBitset (u32 * bitset, u32 i)
{
    const u32 pos = (i >> 5UL);
    const u32 bit = (i & 31UL);
    bitset[pos] |= (1UL << bit);
}

void STDi_FillBitset (u32 *bitset, u32 a, u32 b)
{
    while (a < b) {
        const u32 pos = (u32)(a >> 5UL);
        const u32 bit = (u32)(a & 31UL);
        u32 mask = (~0UL << bit);
        a = (pos + 1UL) * 32UL;
        if (a > b) {
            mask &= (u32)((1UL << (b & 31UL)) - 1UL);
        }
        bitset[pos] |= mask;
    }
}

#define flag_ignored 0x2000

#define flag_h1 0x0040

#define flag_h2 0x0100

#define flag_l1 0x0020

#define flag_l2 0x0080

#define flag_unsigned 0x1000

#define flag_plus 0x0002

#define flag_minus 0x0008

#define STD_SET_BIT(set, c) ((set)[(u32)(c) >> 5] |= 1 << ((u32)(c) & 31))
int STD_TVSScanf(const char *src, const char *fmt, va_list args) {
    // Declared in the order of the original's stack slots, the highest first
    const char *textStart = src;
    BOOL matched = FALSE;
    u64 value;
    u32 flags;
    int digits;
    u32 include;
    BOOL range;
    char *setDst;
    int radix;
    int count = 0;
    int width;
    char *dst;
    u8 prev;
    u32 set[8];
    char ch;
    char conv;
    char *dstStart;

    while (*fmt != '\0') {
        int c = *fmt;

        if (STDi_IsSpace(c)) {
            while (STDi_IsSpace(*fmt)) {
                fmt++;
            }
            while (STDi_IsSpace(*src)) {
                src++;
            }
            continue;
        }
        if (c != '%') {
            if (c != *src) {
                break;
            }
            if (STD_IsSjisLeadByte(c)) {
                src++;
                // BUG: compares the second byte of the text with the first byte of the format character, and moves
                // on in the format by one byte only
#ifdef BUGFIX
                fmt++;
                if (*fmt != *src) {
                    break;
                }
#else
                if (c != *src) {
                    break;
                }
#endif
            }
            src++;
            fmt++;
            continue;
        }
        if (fmt[1] == '%') {
            if (c != *src) {
                break;
            }
            fmt += 2;
            continue;
        }

        value = 0;
        flags = 0;
        fmt++;
        width = 0;
        conv = *fmt;
        if (conv == '*') {
            flags |= flag_ignored;
            fmt++;
            conv = *fmt;
        }
        while (conv >= '0' && conv <= '9') {
            width = width * 10 + conv - '0';
            fmt++;
            conv = *fmt;
        }
        switch (conv) {
        case 'h':
            fmt++;
            conv = *fmt;
            if (conv != 'h') {
                flags |= flag_h1;
            } else {
                flags |= flag_h2;
                fmt++;
                conv = *fmt;
            }
            break;
        case 'l':
            fmt++;
            conv = *fmt;
            if (conv != 'l') {
                flags |= flag_l1;
            } else {
                flags |= flag_l2;
                fmt++;
                conv = *fmt;
            }
            break;
        }

        switch (conv) {
        case 'd':
            radix = 10;
            break;
        case 'i':
            radix = -1;
            break;
        case 'o':
            radix = 8;
            break;
        case 'u':
            radix = 10;
            flags |= flag_unsigned;
            break;
        case 'p':
        case 'x':
        case 'X':
            radix = 16;
            flags |= flag_unsigned;
            break;
        case 'n':
            if (!(flags & flag_ignored)) {
                count--;
            }
            fmt++;
            value = src - textStart;
            goto storeValue;
        case 'c':
        case 's':
            goto scanString;
        case '[':
            goto scanSet;
        default:
            // style-ok: goto-return a break here would only leave the switch, not the loop
            goto finish;
        }

        fmt++;
        c = *src;
        while (STDi_IsSpace(c)) {
            src++;
            c = *src;
        }
        for (;;) {
            if (c == '+') {
                flags |= flag_plus;
            } else if (c == '-') {
                flags |= flag_minus;
            } else {
                break;
            }
            src++;
            c = *src;
        }
        if (radix == -1) {
            if (c != '0') {
                radix = 10;
            } else if (src[1] == 'x' || src[1] == 'X') {
                radix = 16;
            } else {
                radix = 8;
            }
        }
        if (radix == 16 && c == '0' && (src[1] == 'x' || src[1] == 'X')) {
            src += 2;
            c = *src;
        }
        if (width == 0) {
            width = 0x7fffffff;
        }
        digits = 0;
        while (digits < width) {
            u32 digit = c - '0';

            if (digit >= 10) {
                if (digit - ('a' - '0') < 6) {
                    digit -= 'a' - '0' - 10;
                } else {
                    digit -= 'A' - '0';
                    if (digit < 6) {
                        digit += 10;
                    }
                    // BUG: any other character gets the digit c - 'A', which makes 'G' to 'P' digits of 6 to 15
#ifdef BUGFIX
                    else {
                        break;
                    }
#endif
                }
            }
            if (digit >= radix) {
                break;
            }
            src++;
            c = *src;
            value = value * radix + digit;
            digits++;
        }
        if (digits == 0) {
            break;
        }
        if (flags & flag_minus) {
            value = -value;
        }
        matched = TRUE;
    storeValue:
        if (!(flags & flag_ignored)) {
            count++;
            if (flags & flag_h2) {
                *va_arg(args, u8 *) = value;
            } else if (flags & flag_h1) {
                *va_arg(args, u16 *) = value;
            } else if (flags & flag_l2) {
                *va_arg(args, u64 *) = value;
            } else {
                *va_arg(args, u32 *) = value;
            }
        }
        continue;
    scanString:
        dst = NULL;
        dstStart = NULL;
        fmt++;
        if (!(flags & flag_ignored)) {
            dst = va_arg(args, char *);
            dstStart = dst;
        }
        if (conv == 's') {
            if (width == 0) {
                width = 0x7fffffff;
            }
            c = *src;
            while (STDi_IsSpace(c)) {
                src++;
                c = *src;
            }
            while (c != '\0' && !STDi_IsSpace(c) && width > 0) {
                if (dst != NULL) {
                    *dst++ = c;
                }
                src++;
                c = *src;
                width--;
            }
            if (dst != dstStart) {
                *dst = '\0';
                count++;
                matched = TRUE;
            }
        } else {
            if (width == 0) {
                width = 1;
            }
            c = *src;
            while (c != '\0' && width > 0) {
                if (dst != NULL) {
                    *dst++ = c;
                }
                width--;
                src++;
                c = *src;
            }
            if (dst != dstStart) {
                count++;
                matched = TRUE;
                if (width > 0) {
                    *dst = '\0';
                }
            }
        }
        continue;
    scanSet:
        setDst = NULL;
        include = TRUE;
        fmt++;
        prev = 0;
        range = FALSE;
        MI_CpuClear32(set, sizeof(set));
        if (*fmt == '^') {
            fmt++;
            include = FALSE;
        }
        if (*fmt == ']') {
            STD_SET_BIT(set, (u8)*fmt);
            fmt++;
        }
        for (;;) {
            if (*fmt == '\0' || *fmt == ']') {
                if (range) {
                    STD_SET_BIT(set, prev);
                    STD_SET_BIT(set, '-');
                }
                if (*fmt == ']') {
                    fmt++;
                }
                break;
            }
            if (!range) {
                if (prev != 0 && *fmt == '-') {
                    range = TRUE;
                } else {
                    STD_SET_BIT(set, (u8)*fmt);
                    prev = *fmt;
                }
            } else {
                u32 cur = (u8)*fmt;

                if (prev > cur) {
                    STD_SET_BIT(set, prev);
                    STD_SET_BIT(set, '-');
                    STD_SET_BIT(set, cur);
                } else {
                    STDi_FillBitset(set, prev, cur + 1);
                }
                range = FALSE;
                prev = 0;
            }
            fmt++;
        }

        matched = TRUE;
        if (!(flags & flag_ignored)) {
            count++;
            setDst = va_arg(args, char *);
        }
        if (width == 0) {
            width = 0x7fffffff;
        }
        ch = *src;
        while (ch != '\0' && width > 0) {
            if (include != ((set[(u32)(u8)ch >> 5] >> ((u32)(u8)ch & 31)) & 1)) {
                break;
            }
            if (setDst != NULL) {
                *setDst++ = ch;
            }
            src++;
            ch = *src;
            width--;
        }
        if (setDst != NULL) {
            *setDst = '\0';
        }
        continue;
    }
finish:
    return (*src == '\0' && !matched) ? -1 : count;
}

