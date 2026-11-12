// MSL's printf.c, from zeldaret-st's (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3 libs/c/src/wprintf.c, the
// wide version of this file, with its characters made narrow (a project decision):
// long2str (whose test of LONG_MIN is longlong2str's 64-bit one in Black's copy), longlong2str and round_decimal
// match; parse_format, double2hex and float2str don't yet (the first two are static: MWCC drops them until
// __pformatter calls them). __StringWrite, vsnprintf and sprintf are MSL's. The file's name is pokediamond's object's
// (MSL_Common_printf.o), its functions' pokebw2's.
#include <ansi_fp.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <mbstring.h>
#include <mem.h>
#include <secure_error.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIG_DIG 32

#define LDBL_MANT_DIG 24
#define LDBL_MAX_EXP 128
#define TARGET_FLOAT_BITS 32
#define TARGET_FLOAT_BYTES (TARGET_FLOAT_BITS / 8)
#define TARGET_FLOAT_MAX_EXP LDBL_MAX_EXP
#define TARGET_FLOAT_MANT_DIG LDBL_MANT_DIG
#define TARGET_FLOAT_IMPLICIT_J_BIT 1
#define TARGET_FLOAT_MANT_BITS (TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

typedef long long intmax_t;

enum justification_options {
    left_justification,
    right_justification,
    zero_fill
};

enum sign_options {
    only_minus,
    sign_always,
    space_holder
};

enum argument_options {
    // normal_argument,
    // char_argument,
    // short_argument,
    // long_argument,
    // long_long_argument,
    // wchar_argument,
    // intmax_argument,
    // size_t_argument,
    // ptrdiff_argument,
    // long_double_argument
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    long_long_argument,
    wchar_argument,
    intmax_argument,
    size_t_argument,
    ptrdiff_argument,
    long_double_argument
};

typedef struct {
    unsigned char justification_options;
    unsigned char sign_options;
    unsigned char precision_specified;
    unsigned char alternate_form;
    unsigned char argument_options;
    char conversion_char;
    int field_width;
    int precision;
} print_format;

typedef struct {
    char *CharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __OutStrCtrl;

#define conversion_buff_size 512
#define conversion_max 509
#define bad_conversion 0xFFFF

int __pformatter(void *(*WriteProc)(void *, const char *, size_t), void *WriteProcArg, const char *format_str,
                 va_list arg, int is_secure);

static const char *parse_format(const char *format_string, va_list *arg, print_format *format) {
    print_format f;
    const char *s;
    int c;
    int flag_found;

    s = format_string;

    f.justification_options = right_justification;
    f.sign_options          = only_minus;
    f.precision_specified   = 0;
    f.alternate_form        = 0;
    f.argument_options      = normal_argument;
    f.field_width           = 0;
    f.precision             = 0;

    if ((c = *++s) == '%') {
        f.conversion_char = c;
        *format           = f;
        return ((const char *) s + 1);
    }

    while (true) {
        flag_found = 1;

        switch (c) {
            case '-':
                f.justification_options = left_justification;
                break;

            case '+':
                f.sign_options = sign_always;
                break;

            case ' ':
                if (f.sign_options != sign_always) {
                    f.sign_options = space_holder;
                }
                break;

            case '#':
                f.alternate_form = 1;
                break;

            case '0':
                if (f.justification_options != left_justification) {
                    f.justification_options = zero_fill;
                }
                break;

            default:
                flag_found = 0;
                break;
        }

        if (flag_found) {
            c = *++s;
        } else {
            break;
        }
    }

    if (c == '*') {
        if ((f.field_width = va_arg(*arg, int)) < 0) {
            f.justification_options = left_justification;
            f.field_width           = -f.field_width;
        }

        c = *++s;
    } else {
        while (isdigit(c)) {
            f.field_width = (f.field_width * 10) + (c - '0');
            c             = *++s;
        }
    }

    if (f.field_width > conversion_max) {
        f.conversion_char = bad_conversion;
        *format           = f;
        return ((const char *) s + 1);
    }

    if (c == '.') {
        f.precision_specified = 1;

        if ((c = *++s) == '*') {
            if ((f.precision = va_arg(*arg, int)) < 0) {
                f.precision_specified = 0;
            }

            c = *++s;
        } else {
            while (isdigit(c)) {
                f.precision = (f.precision * 10) + (c - '0');
                c           = *++s;
            }
        }
    }

    flag_found = 1;

    switch (c) {
        case 'h':
            f.argument_options = short_argument;
            if (s[1] == 'h') {
                f.argument_options = char_argument;
                c                  = *++s;
            }
            break;

        case 'l':
            f.argument_options = long_argument;
            if (s[1] == 'l') {
                f.argument_options = long_long_argument;
                c                  = *++s;
            }
            break;

        case 'L':
            f.argument_options = long_double_argument;
            break;

        case 'j':
            f.argument_options = intmax_argument;
            break;

        case 't':
            f.argument_options = ptrdiff_argument;
            break;

        case 'z':
            f.argument_options = size_t_argument;
            break;

        default:
            flag_found = 0;
            break;
    }

    if (flag_found) {
        c = *++s;
    }

    f.conversion_char = c;

    switch (c) {
        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
            if (f.argument_options == long_double_argument) {
                f.conversion_char = bad_conversion;
            } else if (!f.precision_specified) {
                f.precision = 1;
            } else if (f.justification_options == zero_fill) {
                f.justification_options = right_justification;
            }
            break;

        case 'f':
        case 'F':
            if (f.argument_options == short_argument || f.argument_options == intmax_argument ||
                f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument ||
                f.argument_options == long_long_argument) {
                f.conversion_char = bad_conversion;
            } else if (!f.precision_specified) {
                f.precision = 6;
            }
            break;

        case 'a':
        case 'A':
            if (!f.precision_specified) {
                f.precision = 0x0D;
            }
            if (f.argument_options == short_argument || f.argument_options == intmax_argument ||
                f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument ||
                f.argument_options == long_long_argument || f.argument_options == char_argument) {
                f.conversion_char = bad_conversion;
            }
            break;

        case 'g':
        case 'G':
            if (!f.precision) {
                f.precision = 1;
            }
            /* fall through */

        case 'e':
        case 'E':
            if (f.argument_options == short_argument || f.argument_options == char_argument ||
                f.argument_options == intmax_argument || f.argument_options == size_t_argument ||
                f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument) {
                f.conversion_char = bad_conversion;
            } else if (!f.precision_specified) {
                f.precision = 6;
            }
            break;

        case 'p':
            f.conversion_char  = 'x';
            f.alternate_form   = 1;
            f.argument_options = long_argument;
            f.precision        = 8;
            break;

        case 'c':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            } else if (f.precision_specified || f.argument_options != normal_argument) {
                f.conversion_char = bad_conversion;
            }
            break;

        case 's':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            } else if (f.argument_options != normal_argument) {
                f.conversion_char = bad_conversion;
            }
            break;

        case 'n':
            if (f.argument_options == long_double_argument) {
                f.conversion_char = bad_conversion;
            }
            break;

        default:
            f.conversion_char = bad_conversion;
            break;
    }

    *format = f;

    return ((const char *) s + 1);
}

char *long2str(long num, char *buff, print_format format) {
    unsigned long unsigned_num, base;
    char *p;
    int n, digits;
    int minus    = 0;
    unsigned_num = num;
    minus        = 0;

    p      = buff;
    *--p   = 0;
    digits = 0;

    if (!num && !format.precision && !(format.alternate_form && format.conversion_char == 'o')) {
        return p;
    }

    switch (format.conversion_char) {
        case 'd':
        case 'i':
            base = 10;

            if (num < 0) {
                if (num != 0x8000000000000000) {
                    unsigned_num = -num;
                }

                minus = 1;
            }
            break;

        case 'o':
            base                = 8;
            format.sign_options = only_minus;
            break;

        case 'u':
            base                = 10;
            format.sign_options = only_minus;
            break;

        case 'x':
        case 'X':
            base                = 16;
            format.sign_options = only_minus;
            break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10) {
            n += '0';
        } else {
            n -= 10;

            if (format.conversion_char == 'x') {
                n += 'a';
            } else {
                n += 'A';
            }
        }

        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    if (base == 8 && format.alternate_form && *p != '0') {
        *--p = '0';
        ++digits;
    }

    if (format.justification_options == zero_fill) {
        format.precision = format.field_width;

        if (minus || format.sign_options != only_minus) {
            --format.precision;
        }

        if (base == 16 && format.alternate_form) {
            format.precision -= 2;
        }
    }

    if (buff - p + format.precision > conversion_max) {
        return 0;
    }

    while (digits < format.precision) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && format.alternate_form) {
        *--p = format.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else if (format.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

char *longlong2str(long long num, char *pBuf, print_format fmt) {
    unsigned long long unsigned_num, base;
    char *p;
    int n, digits;
    int minus    = 0;
    unsigned_num = num;
    minus        = 0;
    p            = pBuf;
    *--p         = 0;
    digits       = 0;

    if (!num && !fmt.precision && !(fmt.alternate_form && fmt.conversion_char == 'o')) {
        return p;
    }

    switch (fmt.conversion_char) {
        case 'd':
        case 'i':
            base = 10;

            if (num < 0) {
                if (num != 0x8000000000000000) {
                    unsigned_num = -num;
                }

                minus = 1;
            }
            break;
        case 'o':
            base             = 8;
            fmt.sign_options = only_minus;
            break;
        case 'u':
            base             = 10;
            fmt.sign_options = only_minus;
            break;
        case 'x':
        case 'X':
            base             = 16;
            fmt.sign_options = only_minus;
            break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10) {
            n += '0';
        } else {
            n -= 10;
            if (fmt.conversion_char == 'x') {
                n += 'a';
            } else {
                n += 'A';
            }
        }

        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    if (base == 8 && fmt.alternate_form && *p != '0') {
        *--p = '0';
        ++digits;
    }

    if (fmt.justification_options == zero_fill) {
        fmt.precision = fmt.field_width;

        if (minus || fmt.sign_options != only_minus) {
            --fmt.precision;
        }

        if (base == 16 && fmt.alternate_form) {
            fmt.precision -= 2;
        }
    }

    if (pBuf - p + fmt.precision > conversion_max) {
        return 0;
    }

    while (digits < fmt.precision) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && fmt.alternate_form) {
        *--p = fmt.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (fmt.sign_options == sign_always) {
        *--p = '+';
    } else if (fmt.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

static char *double2hex(long double num, char *buff, print_format format) {
    char *p;
    unsigned char *q;
    unsigned char working_byte;
    long double ld;
    int expbits, expmask;
    unsigned snum;
    long exp;
    print_format exp_format;
    int hex_precision;
    int mantissa_bit;
    decform form;
    decimal dec;
    int radix_marker;
    int i;
    unsigned char *nump;

    radix_marker = '.';
    p            = buff;
    ld           = num;

    if (format.precision > 509) {
        return 0;
    }

    form.style  = (char) 0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);

    switch (*dec.sig.text) {
        case '0':
            dec.exp = 0;
            break;
        case 'I':
            if (dec.sgn) {
                p = buff - 5;
                if (format.conversion_char == 'A') {
                    strcpy(p, "-INF");
                } else {
                    strcpy(p, "-inf");
                }
            } else {
                p = buff - 4;
                if (format.conversion_char == 'A') {
                    strcpy(p, "INF");
                } else {
                    strcpy(p, "inf");
                }
            }
            return p;
        case 'N':
            if (dec.sgn) {
                p = buff - 5;
                if (format.conversion_char == 'A') {
                    strcpy(p, "-NAN");
                } else {
                    strcpy(p, "-nan");
                }
            } else {
                p = buff - 4;
                if (format.conversion_char == 'A') {
                    strcpy(p, "NAN");
                } else {
                    strcpy(p, "nan");
                }
            }
            return p;
    }

    exp_format.justification_options = right_justification;
    exp_format.sign_options          = sign_always;
    exp_format.precision_specified   = 0;
    exp_format.alternate_form        = 0;
    exp_format.argument_options      = normal_argument;
    exp_format.field_width           = 0;
    exp_format.precision             = 1;
    exp_format.conversion_char       = 'd';

    nump = (unsigned char *) &num;
    for (i = 0; i < TARGET_FLOAT_BYTES; i++) {
        unsigned char x                  = nump[i];
        nump[i]                          = nump[TARGET_FLOAT_BYTES - i + 3];
        nump[TARGET_FLOAT_BYTES - i + 3] = x;
    }

    expbits = TARGET_FLOAT_EXP_BITS;
    expmask = 0x7FF;

    snum = ((unsigned char *) &num)[0] << 25;
    if (TARGET_FLOAT_EXP_BITS > 7) {
        snum |= ((unsigned char *) &num)[1] << 17;
    }
    if (TARGET_FLOAT_EXP_BITS > 15) {
        snum |= ((unsigned char *) &num)[2] << 9;
    }
    if (TARGET_FLOAT_EXP_BITS > 23) {
        snum |= ((unsigned char *) &num)[3] << 1;
    }

    snum = (snum >> (29 - expbits)) & expmask;

    if (snum != 0) {
        exp = snum - 0x3FF;
    } else {
        exp = 0;
    }

    p = long2str(exp, buff, exp_format);
    if (format.conversion_char == 'a') {
        *--p = 'p';
    } else {
        *--p = 'P';
    }
    q = (unsigned char *) &num;

    if (TARGET_FLOAT_IMPLICIT_J_BIT) {
        mantissa_bit = (4 + expbits + format.precision * 4) - 1;
    } else {
        mantissa_bit = (1 + expbits + format.precision * 4) - 4;
    }

    for (hex_precision = format.precision; hex_precision >= 1; hex_precision--) {
        if (mantissa_bit < 64) {
            int mantissa_byte;

            mantissa_byte = mantissa_bit >> 3;
            working_byte  = (*(q + mantissa_byte)) >> (7 - (mantissa_bit & 7));

            if ((mantissa_bit & ~7) != ((mantissa_bit - 4) & ~7)) {
                working_byte |= ((*(q + (mantissa_byte - 1))) << 8) >> (7 - ((mantissa_bit) & 7));
            }

            if ((working_byte &= 0xF) < 10) {
                working_byte += '0';
            } else if (format.conversion_char == 'a') {
                working_byte += 'a' - 10;
            } else {
                working_byte += 'A' - 10;
            }
        } else {
            working_byte = '0';
        }

        *--p = working_byte;
        mantissa_bit -= 4;
    }

    if (TARGET_FLOAT_IMPLICIT_J_BIT) {
        if (format.precision || format.alternate_form) {
            *--p = radix_marker;
        }

        if (fabs(ld) != 0.0) {
            *--p = '1';
        } else {
            *--p = '0';
        }
    }

    if (format.conversion_char == 'a') {
        *--p = 'x';
    } else {
        *--p = 'X';
    }

    *--p = '0';

    if (dec.sgn) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else if (format.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

void round_decimal(decimal *dec, int new_length) {
    unsigned char c;
    unsigned char *p;
    int carry;

    if (new_length < 0) {
    return_zero:
        dec->exp        = 0;
        dec->sig.length = 1;
        *dec->sig.text  = '0';
        return;
    }

    if (new_length >= dec->sig.length) {
        return;
    }

    p = (unsigned char *) dec->sig.text + new_length + 1;
    c = *--p - '0';

    if (c == 5) {
        unsigned char *q = &((unsigned char *) dec->sig.text)[dec->sig.length];

        while (--q > p && *q == '0')
            ;
        carry = (q == p) ? p[-1] & 1 : 1;
    } else {
        carry = (c > 5);
    }

    while (new_length != 0) {
        c = *--p - '0' + carry;

        if ((carry = (c > 9)) != 0 || c == 0) {
            --new_length;
        } else {
            *p = c + '0';
            break;
        }
    }

    if (carry != 0) {
        dec->exp += 1;
        dec->sig.length = 1;
        *dec->sig.text  = '1';
        return;
    } else if (new_length == 0) {
        goto return_zero;
    }

    dec->sig.length = new_length;
}

char *float2str(long double num, char *wbuff, print_format format) {
    decimal dec;
    decform form;
    char *p;
    char *q;
    int n;
    int digits;
    int sign;
    int int_digits;
    int frac_digits;

    if (format.precision > conversion_max) {
        return 0;
    }

    form.style  = 0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);
    p = dec.sig.text + dec.sig.length;

    while (dec.sig.length > 1 && *--p == '0') {
        --dec.sig.length;
        ++dec.exp;
    }

    switch (*dec.sig.text) {
        case '0':
            dec.exp = 0;
            break;
        case 'I':
            if (num < 0) {
                p = wbuff - 5;
                if (isupper(format.conversion_char)) {
                    strcpy(p, "-INF");
                } else {
                    strcpy(p, "-inf");
                }
            } else {
                p = wbuff - 4;
                if (isupper(format.conversion_char)) {
                    strcpy(p, "INF");
                } else {
                    strcpy(p, "inf");
                }
            }
            return p;
        case 'N':
            if (dec.sgn) {
                p = wbuff - 5;
                if (isupper(format.conversion_char)) {
                    strcpy(p, "-NAN");
                } else {
                    strcpy(p, "-nan");
                }
            } else {
                p = wbuff - 4;
                if (isupper(format.conversion_char)) {
                    strcpy(p, "NAN");
                } else {
                    strcpy(p, "nan");
                }
            }
            return p;
    }

    dec.exp += dec.sig.length - 1;
    p    = wbuff;
    *--p = 0;

    switch (format.conversion_char) {
        case 'g':
        case 'G':
            if (dec.sig.length > format.precision) {
                round_decimal(&dec, format.precision);
            }

            if (dec.exp < -4 || dec.exp >= format.precision) {
                if (format.alternate_form) {
                    format.precision--;
                } else {
                    format.precision = dec.sig.length - 1;
                }

                if (format.conversion_char == 'g') {
                    format.conversion_char = 'e';
                } else {
                    format.conversion_char = 'E';
                }
                goto e_format;
            }

            if (format.alternate_form) {
                format.precision -= dec.exp + 1;
            } else {
                if ((format.precision = dec.sig.length - (dec.exp + 1)) < 0) {
                    format.precision = 0;
                }
            }
            goto f_format;

        case 'e':
        case 'E':
        e_format:
            if (dec.sig.length > format.precision + 1) {
                round_decimal(&dec, format.precision + 1);
            }

            n    = dec.exp;
            sign = '+';
            if (n < 0) {
                n    = -n;
                sign = '-';
            }

            for (digits = 0; n || digits < 2; digits++) {
                *--p = n % 10 + '0';
                n /= 10;
            }

            *--p = sign;
            *--p = format.conversion_char;

            if (wbuff - p + format.precision > conversion_max) {
                return 0;
            }

            if (dec.sig.length < format.precision + 1) {
                for (n = format.precision + 1 - dec.sig.length + 1; --n;) {
                    *--p = '0';
                }
            }

            for (n = dec.sig.length, q = dec.sig.text + dec.sig.length; --n;) {
                *--p = *--q;
            }

            if (format.precision || format.alternate_form) {
                *--p = '.';
            }

            *--p = *dec.sig.text;

            if (dec.sgn) {
                *--p = '-';
            } else if (format.sign_options == sign_always) {
                *--p = '+';
            } else if (format.sign_options == space_holder) {
                *--p = ' ';
            }
            break;

        case 'f':
        case 'F':
        f_format:
            if ((frac_digits = -dec.exp + dec.sig.length - 1) < 0) {
                frac_digits = 0;
            }

            if (frac_digits > format.precision) {
                round_decimal(&dec, dec.sig.length - (frac_digits - format.precision));
                if ((frac_digits = -dec.exp + dec.sig.length - 1) < 0) {
                    frac_digits = 0;
                }
            }

            if ((int_digits = dec.exp + 1) < 0) {
                int_digits = 0;
            }

            if (int_digits + frac_digits > conversion_max) {
                return 0;
            }

            q = dec.sig.text + dec.sig.length;
            for (digits = 0; digits < format.precision - frac_digits; digits++) {
                *--p = '0';
            }

            for (digits = 0; digits < frac_digits && digits < dec.sig.length; digits++) {
                *--p = *--q;
            }

            for (; digits < frac_digits; digits++) {
                *--p = '0';
            }

            if (format.precision || format.alternate_form) {
                *--p = '.';
            }

            if (int_digits) {
                for (digits = 0; digits < int_digits - dec.sig.length; digits++) {
                    *--p = '0';
                }

                for (; digits < int_digits; digits++) {
                    *--p = *--q;
                }
            } else {
                *--p = '0';
            }

            if (dec.sgn) {
                *--p = '-';
            } else if (format.sign_options == sign_always) {
                *--p = '+';
            } else if (format.sign_options == space_holder) {
                *--p = ' ';
            }
            break;
    }

    return p;
}

// Black's functions in order (Black's size), to write here:
// __pformatter: 0x8ac, which loads .data 0x020a8a40-0x020a8a78 (with double2hex and float2str); zeldaret-st's
// __wpformatter is its wide version

int __pformatter(void *(*WriteProc)(void *, const char *, size_t), void *WriteProcArg, const char *format_str,
                        va_list arg, int is_secure) {
    int num_chars;
    int chars_written;
    const char *format_ptr;
    long long_num;
    double long_double_num;
    char *buff_ptr;
    char *string_end;
    int total_chars;
    char buff[512];
    print_format format;
    char fill_char = ' ';
    long long long_long_num;
    const char *curr_format;

    format_ptr = format_str;
    chars_written = 0;

    while (*format_ptr != '\0') {
        curr_format = strchr(format_ptr, '%');
        if (curr_format == NULL) {
            num_chars = strlen(format_ptr);
            chars_written += num_chars;
            if (num_chars != 0 && !WriteProc(WriteProcArg, format_ptr, num_chars)) {
                return -1;
            }
            break;
        }

        num_chars = curr_format - format_ptr;
        chars_written += num_chars;
        if (num_chars != 0 && !WriteProc(WriteProcArg, format_ptr, num_chars)) {
            return -1;
        }

        format_ptr = parse_format(curr_format, &arg, &format);

        switch (format.conversion_char) {
        // The integers: the argument is taken at the size the length modifier says and cut down to a char or a short
        // for hh and h; the long longs go through longlong2str
        case 'd':
        case 'i':
            if (format.argument_options == long_argument) {
                long_num = va_arg(arg, long);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = va_arg(arg, long long);
            } else if (format.argument_options == intmax_argument) {
                long_long_num = va_arg(arg, long long);
            } else if (format.argument_options == size_t_argument) {
                long_num = va_arg(arg, size_t);
            } else if (format.argument_options == ptrdiff_argument) {
                long_num = va_arg(arg, long);
            } else {
                long_num = va_arg(arg, int);
            }

            if (format.argument_options == short_argument) {
                long_num = (short)long_num;
            }
            if (format.argument_options == char_argument) {
                long_num = (signed char)long_num;
            }

            if (format.argument_options == long_long_argument || format.argument_options == intmax_argument) {
                if ((buff_ptr = longlong2str(long_long_num, buff + sizeof(buff), format)) == NULL) {
                    goto conversion_error;
                }
            } else {
                if ((buff_ptr = long2str(long_num, buff + sizeof(buff), format)) == NULL) {
                    goto conversion_error;
                }
            }
            num_chars = buff + sizeof(buff) - 1 - buff_ptr;
            break;
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (format.argument_options == long_argument) {
                long_num = va_arg(arg, unsigned long);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = va_arg(arg, unsigned long long);
            } else if (format.argument_options == intmax_argument) {
                long_long_num = va_arg(arg, unsigned long long);
            } else if (format.argument_options == size_t_argument) {
                long_num = va_arg(arg, size_t);
            } else if (format.argument_options == ptrdiff_argument) {
                long_num = va_arg(arg, unsigned long);
            } else {
                long_num = va_arg(arg, unsigned int);
            }

            if (format.argument_options == short_argument) {
                long_num = (unsigned short)long_num;
            }
            if (format.argument_options == char_argument) {
                long_num = (unsigned char)long_num;
            }

            if (format.argument_options == long_long_argument || format.argument_options == intmax_argument) {
                if ((buff_ptr = longlong2str(long_long_num, buff + sizeof(buff), format)) == NULL) {
                    goto conversion_error;
                }
            } else {
                if ((buff_ptr = long2str(long_num, buff + sizeof(buff), format)) == NULL) {
                    goto conversion_error;
                }
            }
            num_chars = buff + sizeof(buff) - 1 - buff_ptr;
            break;
        // The floating-point numbers, whose long double is a double here
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (format.argument_options == long_double_argument) {
                long_double_num = va_arg(arg, long double);
            } else {
                long_double_num = va_arg(arg, double);
            }
            if ((buff_ptr = float2str(long_double_num, buff + sizeof(buff), format)) == NULL) {
                goto conversion_error;
            }
            num_chars = buff + sizeof(buff) - 1 - buff_ptr;
            break;
        case 'a':
        case 'A':
            if (format.argument_options == long_double_argument) {
                long_double_num = va_arg(arg, long double);
            } else {
                long_double_num = va_arg(arg, double);
            }
            if ((buff_ptr = double2hex(long_double_num, buff + sizeof(buff), format)) == NULL) {
                goto conversion_error;
            }
            num_chars = buff + sizeof(buff) - 1 - buff_ptr;
            break;
        // A string, of bytes or (for ls) of wide characters, which wcstombs turns into bytes in buff. A null pointer
        // prints as an empty string, or is reported if the call is secure
        case 's':
            if (format.argument_options == wchar_argument) {
                wchar_t *wcs_ptr = va_arg(arg, wchar_t *);

                if (is_secure && wcs_ptr == NULL) {
                    __msl_runtime_constraint_violation_s(NULL, NULL, -1);
                    return -1;
                }
                if (wcs_ptr == NULL) {
                    wcs_ptr = L"";
                }
                buff_ptr = buff;
                num_chars = wcstombs(buff_ptr, wcs_ptr, sizeof(buff));
                if (num_chars < 0) {
                    goto conversion_error;
                }
            } else {
                buff_ptr = va_arg(arg, char *);
            }

            if (is_secure && buff_ptr == NULL) {
                __msl_runtime_constraint_violation_s(NULL, NULL, -1);
                return -1;
            }
            if (buff_ptr == NULL) {
                buff_ptr = "";
            }

            if (format.alternate_form) {
                // A Pascal string: the length comes first
                num_chars = *buff_ptr++;
                if (format.precision_specified && num_chars > format.precision) {
                    num_chars = format.precision;
                }
            } else if (format.precision_specified) {
                num_chars = format.precision;
                string_end = memchr(buff_ptr, '\0', num_chars);
                if (string_end != NULL) {
                    num_chars = string_end - buff_ptr;
                }
            } else {
                num_chars = strlen(buff_ptr);
            }
            break;
        // Stores the number of characters so far, through the pointer (which buff_ptr holds), at the size of the length
        // modifier (a secure call reports it instead, and hh and the wide kind are ignored)
        case 'n':
            buff_ptr = va_arg(arg, char *);

            if (is_secure) {
                __msl_runtime_constraint_violation_s(NULL, NULL, -1);
                return -1;
            }
            switch (format.argument_options) {
            case normal_argument:
                *(int *)buff_ptr = chars_written;
                break;
            case short_argument:
                *(short *)buff_ptr = chars_written;
                break;
            case long_argument:
                *(long *)buff_ptr = chars_written;
                break;
            case intmax_argument:
                *(long long *)buff_ptr = chars_written;
                break;
            case size_t_argument:
                *(size_t *)buff_ptr = chars_written;
                break;
            case ptrdiff_argument:
                *(long *)buff_ptr = chars_written;
                break;
            case long_long_argument:
                *(long long *)buff_ptr = chars_written;
                break;
            }
            continue;
        case 'c':
            buff_ptr = buff;
            *buff_ptr = va_arg(arg, int);
            num_chars = 1;
            break;
        case '%':
            buff_ptr = buff;
            *buff_ptr = '%';
            num_chars = 1;
            break;
        // An invalid conversion (0xFF is what parse_format marks it with), or one that gave no text, ends the output
        // with the rest of the format string from its % on
        case 0xFF:
        default:
        conversion_error:
            num_chars = strlen(curr_format);
            chars_written += num_chars;
            if (num_chars != 0 && !WriteProc(WriteProcArg, curr_format, num_chars)) {
                return -1;
            }
            return chars_written;
        }

        // The text is in buff_ptr, num_chars long; it goes out padded to the field width, on the left with spaces or,
        // for zero fill, with zeros after the sign and the 0x of a %a
        total_chars = num_chars;

        if (format.justification_options != left_justification) {
            fill_char = (format.justification_options == zero_fill) ? '0' : ' ';

            if ((*buff_ptr == '+' || *buff_ptr == '-' || *buff_ptr == ' ') && fill_char == '0') {
                if (!WriteProc(WriteProcArg, buff_ptr, 1)) {
                    return -1;
                }
                ++buff_ptr;
                --num_chars;
            }

            if (format.justification_options == zero_fill &&
                (format.conversion_char == 'a' || format.conversion_char == 'A')) {
                if (num_chars < 2) {
                    return -1;
                }
                if (!WriteProc(WriteProcArg, buff_ptr, 2)) {
                    return -1;
                }
                num_chars -= 2;
                buff_ptr += 2;
            }

            while (total_chars < format.field_width) {
                if (!WriteProc(WriteProcArg, &fill_char, 1)) {
                    return -1;
                }
                ++total_chars;
            }
        }

        if (num_chars != 0 && !WriteProc(WriteProcArg, buff_ptr, num_chars)) {
            return -1;
        }

        // Left justified: the padding follows the text
        if (format.justification_options == left_justification) {
            while (total_chars < format.field_width) {
                char blank = ' ';

                if (!WriteProc(WriteProcArg, &blank, 1)) {
                    return -1;
                }
                ++total_chars;
            }
        }

        chars_written += total_chars;
    }

    return chars_written;
}

void *__StringWrite(void *osc, const char *Buffer, size_t NumChars) {
    size_t CharsToBeWritten;
    __OutStrCtrl *Oscp = (__OutStrCtrl *) osc;

    CharsToBeWritten = ((Oscp->CharsWritten + NumChars) <= Oscp->MaxCharCount) ? NumChars
                                                                               : Oscp->MaxCharCount - Oscp->CharsWritten;
    memcpy(Oscp->CharStr + Oscp->CharsWritten, Buffer, CharsToBeWritten);
    Oscp->CharsWritten += CharsToBeWritten;
    return (void *) 1;
}

int vsnprintf(char *s, size_t n, const char *format, va_list arg) {
    int end;
    __OutStrCtrl osc;

    osc.CharStr      = s;
    osc.MaxCharCount = n;
    osc.CharsWritten = 0;

    end = __pformatter(__StringWrite, &osc, format, arg, 0);

    if (s) {
        if (end < n) {
            s[end] = '\0';
        } else if (n > 0) {
            s[n - 1] = '\0';
        }
    }

    return end;
}

int sprintf(char *s, const char *format, ...) {
    va_list args;

    va_start(args, format);
    return vsnprintf(s, ULONG_MAX, format, args);
}
