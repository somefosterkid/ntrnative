// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_printf.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). TwlSDK has this code in std_sprintf.c, under the STD_ names
// it has here, and os_printf.c's OS_ functions call it.
#include <nitro.h>

#ifdef SDK_LINK_ISD
    #pragma warn_extracomma off
    #include <isdbglib.h>
    # pragma warn_extracomma reset
#else
    void ISDPrint(const char *);
#endif

#if defined(SDK_CW_WARNOFF_SAFESTRB)
    #include <nitro/code32.h>
#endif

typedef struct dst_string_tag {
    size_t len;
    char * cur;
    char * base;
} dst_string;

static void string_put_char (dst_string * p, char c)
{
    if (p->len > 0)
        *p->cur = c, --p->len;

    ++p->cur;
}

static void string_fill_char (dst_string * p, char c, int n)
{
    if (n > 0) {
        size_t i, k = p->len;

        if (k > (size_t) n)
            k = (size_t) n;

        for (i = 0; i < k; ++i)
            p->cur[i] = c;

        p->len -= k;
        p->cur += n;
    }
}

static void string_put_string (dst_string * p, const char * s, int n)
{
    if (n > 0) {
        size_t i, k = p->len;

        if (k > (size_t) n)
            k = (size_t) n;

        for (i = 0; i < k; ++i)
            p->cur[i] = s[i];

        p->len -= k;
        p->cur += n;
    }
}

SDK_WEAK_SYMBOL int STD_TSPrintf (char * dst, const char * fmt, ...)
{
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = STD_TVSNPrintf(dst, 0x7FFFFFFF, fmt, va);
    va_end(va);
    return ret;
}


SDK_WEAK_SYMBOL int STD_TSNPrintf (char * dst, size_t len, const char * fmt, ...)
{
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = STD_TVSNPrintf(dst, len, fmt, va);
    va_end(va);
    return ret;
}

// From libntr (MIT, https://github.com/cybervisi0n/libntr), libraries/std/src/std_sprintf.c: ported as it is, with credit (tools/ref_port.py)
SDK_WEAK_SYMBOL int STD_TVSNPrintf (char * dst, size_t len, const char * fmt, va_list vlist)
{
    enum {
        flag_blank = 000001,
        flag_plus = 000002,
        flag_sharp = 000004,
        flag_minus = 000010,
        flag_zero = 000020,
        flag_l1 = 000040,
        flag_h1 = 000100,
        flag_l2 = 000200,
        flag_h2 = 000400,
        flag_unsigned = 010000,
        flag_end
    };
    char buf[24];
    int n_buf;
    char prefix[2];
    int n_prefix;
    dst_string str;
    const char * s = fmt;

    str.len = len;
    str.cur = str.base = dst;
    while (*s != '\0') {
        int flag;
        int width;
        int precision;
        int radix;
        const char * p_start;
        int hex_char;
        u64 val;
        int n_pad;

        if (STD_IsSjisLeadByte(*s)) {
            string_put_char(&str, *s++);
            if (*s != '\0') {
                string_put_char(&str, *s++);
            }
            continue;
        }
        if (*s != '%') {
            string_put_char(&str, *s++);
            continue;
        }

        flag = 0;
        width = 0;
        p_start = s;
        precision = -1;
        radix = 10;
        hex_char = 'a' - 10;
        for (;;) {
            switch (*++s) {
            case '+':
                if (s[-1] == ' ') {
                    flag |= flag_plus;
                    continue;
                }
                break;
            case ' ':
                flag |= flag_blank;
                continue;
            case '-':
                flag |= flag_minus;
                continue;
            case '0':
                flag |= flag_zero;
                continue;
            }
            break;
        }

        if (*s == '*') {
            s++;
            width = va_arg(vlist, int);
            if (width < 0) {
                width = -width;
                flag |= flag_minus;
            }
        } else {
            while (*s >= '0' && *s <= '9') {
                width = width * 10 + *s++ - '0';
            }
        }

        if (*s == '.') {
            s++;
            precision = 0;
            if (*s == '*') {
                s++;
                precision = va_arg(vlist, int);
                if (precision < 0) {
                    precision = -1;
                }
            } else {
                while (*s >= '0' && *s <= '9') {
                    precision = precision * 10 + *s++ - '0';
                }
            }
        }

        switch (*s) {
        case 'h':
            s++;
            if (*s != 'h') {
                flag |= flag_h1;
            } else {
                s++;
                flag |= flag_h2;
            }
            break;
        case 'l':
            s++;
            if (*s != 'l') {
                flag |= flag_l1;
            } else {
                s++;
                flag |= flag_l2;
            }
            break;
        }

        switch (*s) {
        case 'd':
        case 'i':
            break;
        case 'o':
            radix = 8;
            flag |= flag_unsigned;
            break;
        case 'u':
            flag |= flag_unsigned;
            break;
        case 'X':
            hex_char = 'A' - 10;
            goto put_hexadecimal;
        case 'p':
            flag |= flag_sharp;
            precision = 8;
            goto put_hexadecimal;
        case 'c':
            if (precision < 0) {
                int c = va_arg(vlist, int);

                if (flag & flag_minus) {
                    string_put_char(&str, c);
                    string_fill_char(&str, ' ', width - 1);
                } else {
                    string_fill_char(&str, (flag & flag_zero) ? '0' : ' ', width - 1);
                    string_put_char(&str, c);
                }
                s++;
                continue;
            }
            goto put_invalid;
        case 's': {
            int n = 0;
            const char * p_buf = va_arg(vlist, const char *);

            if (precision < 0) {
                while (p_buf[n] != '\0') {
                    n++;
                }
            } else {
                while (n < precision && p_buf[n] != '\0') {
                    n++;
                }
            }
            width -= n;
            if (flag & flag_minus) {
                string_put_string(&str, p_buf, n);
                string_fill_char(&str, ' ', width);
            } else {
                string_fill_char(&str, (flag & flag_zero) ? '0' : ' ', width);
                string_put_string(&str, p_buf, n);
            }
            s++;
            continue;
        }
        case '%':
            if (p_start + 1 == s) {
                string_put_char(&str, *s++);
                continue;
            }
        default:
        put_invalid:
            string_put_string(&str, p_start, s - p_start);
            continue;
        case 'x':
        put_hexadecimal:
            radix = 16;
            flag |= flag_unsigned;
            break;
        }

        n_prefix = 0;
        if (flag & flag_minus) {
            flag &= ~flag_zero;
        }
        if (precision < 0) {
            precision = 1;
        } else {
            flag &= ~flag_zero;
        }

        if (flag & flag_unsigned) {
            if (flag & flag_h2) {
                val = (u8)va_arg(vlist, u32);
            } else if (flag & flag_h1) {
                val = (u16)va_arg(vlist, u32);
            } else if (flag & flag_l2) {
                val = va_arg(vlist, u64);
            } else {
                val = va_arg(vlist, u32);
            }
            flag &= ~(flag_blank | flag_plus);
            if (flag & flag_sharp) {
                if (radix == 16) {
                    if (val != 0) {
                        prefix[0] = hex_char + (10 + 'x' - 'a');
                        prefix[1] = '0';
                        n_prefix = 2;
                    }
                } else if (radix == 8) {
                    prefix[0] = '0';
                    n_prefix = 1;
                }
            }
        } else {
            if (flag & flag_h2) {
                val = (s8)va_arg(vlist, s32);
            } else if (flag & flag_h1) {
                val = (s16)va_arg(vlist, s32);
            } else if (flag & flag_l2) {
                val = va_arg(vlist, s64);
            } else {
                val = va_arg(vlist, s32);
            }
            if ((val >> 32) & 0x80000000) {
                val = ~val + 1;
                prefix[0] = '-';
                n_prefix = 1;
            } else if (val != 0 || precision != 0) {
                if (flag & flag_plus) {
                    prefix[0] = '+';
                    n_prefix = 1;
                } else if (flag & flag_blank) {
                    prefix[0] = ' ';
                    n_prefix = 1;
                }
            }
        }

        n_buf = 0;
        switch (radix) {
        case 8:
            while (val != 0) {
                int d = (int)(val & 0x07);

                val >>= 3;
                buf[n_buf++] = (char)(d + '0');
            }
            break;
        case 10:
            if ((val >> 32) == 0) {
                u32 v = (u32)val;

                while (v != 0) {
                    u32 r = v / 10;
                    int d = (int)(v - r * 10);

                    v = r;
                    buf[n_buf++] = (char)(d + '0');
                }
            } else {
                while (val != 0) {
                    u64 r = val / 10;
                    int d = (int)(val - r * 10);

                    val = r;
                    buf[n_buf++] = (char)(d + '0');
                }
            }
            break;
        case 16:
            while (val != 0) {
                int d = (int)(val & 0x0f);

                val >>= 4;
                buf[n_buf++] = (char)((d < 10) ? (d + '0') : (d + hex_char));
            }
            break;
        }

        if (n_prefix > 0 && prefix[0] == '0') {
            n_prefix = 0;
            buf[n_buf++] = '0';
        }
        n_pad = precision - n_buf;
        if (flag & flag_zero) {
            int pad = width - n_buf - n_prefix;

            if (n_pad < pad) {
                n_pad = pad;
            }
        }
        if (n_pad > 0) {
            width -= n_pad;
        }
        width -= n_prefix + n_buf;

        if (!(flag & flag_minus)) {
            string_fill_char(&str, ' ', width);
        }
        while (n_prefix > 0) {
            string_put_char(&str, prefix[--n_prefix]);
        }
        string_fill_char(&str, '0', n_pad);
        while (n_buf > 0) {
            string_put_char(&str, buf[--n_buf]);
        }
        if (flag & flag_minus) {
            string_fill_char(&str, ' ', width);
        }
        s++;
    }

    if (str.len != 0) {
        *str.cur = '\0';
    } else if (len != 0) {
        str.base[len - 1] = '\0';
    }
    return str.cur - str.base;
}

#if defined(SDK_CW_WARNOFF_SAFESTRB)
    #include <nitro/codereset.h>
#endif

    #ifdef OS_Printf
        #undef OS_Printf
    #endif

    void OS_Printf(const char * fmt, ...);
