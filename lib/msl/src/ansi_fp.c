// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/ansi_fp.c: ported as it is,
// with credit (a project decision), but for the count of a double's trailing zeros:
// zeldaret-st's MSL counts them with the compiler's intrinsics, Black's with __msl_generic_count_bits64 (math_api.c).
// __equals_dec, __less_dec and __minus_dec, which zeldaret-st lacks, are MSL's; __dec2num is listed where it goes.
#include <ansi_fp.h>
#include <fdlibm.h>
#include <float.h>
#include <math.h>
#include <system/fp.h>
#include "types.h"

typedef unsigned long long d_int;
#define SIGDIGLEN 32

// The powers of ten that __dec2num scales its running value by, for up to eight digits at a time (not const, as
// MSL has it: Black's is in .data)
static double pow_10[8] = {1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8};

static inline int count_trailing(double x) {
    unsigned long long bits = *(unsigned long long *) &x | 0x0010000000000000;

    return __msl_generic_count_bits64((bits & -bits) - 1);
}

static int __must_round(const decimal *d, int digits) {
    unsigned char const *i = d->sig.text + digits;

    if (*i > 5) {
        return 1;
    }

    if (*i < 5) {
        return -1;
    }

    {
        unsigned char const *e = d->sig.text + d->sig.length;

        for (i++; i < e; i++) {
            if (*i != 0) {
                return 1;
            }
        }
    }

    if (d->sig.text[digits - 1] & 1) {
        return 1;
    }

    return -1;
}

static void __dorounddecup(decimal *d, int digits) {
    unsigned char *b = d->sig.text;
    unsigned char *i = b + digits - 1;

    while (1) {
        if (*i < 9) {
            *i += 1;
            break;
        }
        if (i == b) {
            *i = 1;
            d->exp++;
            break;
        }
        *i-- = 0;
    }
}

static void __rounddec(decimal *d, int digits) {
    if (digits > 0 && digits < d->sig.length) {
        int unkBool   = __must_round(d, digits);
        d->sig.length = digits;

        if (unkBool >= 0) {
            __dorounddecup(d, digits);
        }
    }
}

void __ull2dec(decimal *result, unsigned long long val) {
    result->sgn        = 0;
    result->sig.length = 0;

    for (; val != 0; val /= 10) {
        result->sig.text[result->sig.length++] = (unsigned char) (val % 10);
    }

    {
        unsigned char *i = result->sig.text;
        unsigned char *j = result->sig.text + result->sig.length;

        for (; i < --j; ++i) {
            unsigned char t = *i;
            *i              = *j;
            *j              = t;
        }
    }

    result->exp = result->sig.length - 1;
}

void __timesdec(decimal *result, const decimal *x, const decimal *y) {
    unsigned int accumulator = 0;
    unsigned char mantissa[SIGDIGLEN * 2];
    int i = x->sig.length + y->sig.length - 1;
    unsigned char *ip = mantissa + i + 1;
    unsigned char *ep = ip;

    result->sgn = 0;

    for (; i > 0; i--) {
        int k = y->sig.length - 1;
        int j = i - k - 1;
        int l;
        int t;
        const unsigned char *jp;
        const unsigned char *kp;

        if (j < 0) {
            j = 0;
            k = i - 1;
        }

        jp = x->sig.text + j;
        kp = y->sig.text + k;
        l  = k + 1;
        t  = x->sig.length - j;

        if (l > t) {
            l = t;
        }

        for (; l > 0; l--, jp++, kp--) {
            accumulator += *jp * *kp;
        }

        *--ip = (unsigned char) (accumulator % 10);
        accumulator /= 10;
    }

    result->exp = (short) (x->exp + y->exp);

    if (accumulator) {
        *--ip = (unsigned char) (accumulator);
        result->exp++;
    }

    for (i = 0; i < SIGDIGLEN && ip < ep; i++, ip++) {
        result->sig.text[i] = *ip;
    }
    result->sig.length = (unsigned char) (i);

    if (ip < ep && *ip >= 5) {
        if (*ip == 5) {
            unsigned char *jp = ip + 1;
            for (; jp < ep; jp++) {
                if (*jp != 0) {
                    goto round;
                }
            }
            if ((ip[-1] & 1) == 0) {
                return;
            }
        }
    round:
        __dorounddecup(result, result->sig.length);
    }
}

void __str2dec(decimal *d, const char *str, short exp) {
    const unsigned char *s = (const unsigned char *) str;
    int i;

    d->exp = exp;
    d->sgn = 0;

    for (i = 0; i < SIGDIGLEN && *s;) {
        d->sig.text[i++] = *s++ - '0';
    }
    d->sig.length = i;

    if (*s != 0) {
        if (*s < 5) {
            return;
        }
        if (*s > 5) {
            goto round;
        }

        {
            const unsigned char *p = s + 1;

            for (; *p != 0; p++) {
                if (*p != '0') {
                    goto round;
                }
            }

            if ((d->sig.text[i - 1] & 1) == 0) {
                return;
            }
        }
    round:
        __dorounddecup(d, d->sig.length);
    }
}

void __two_exp(decimal *result, long exp) {
    switch (exp) {
        case -64:
            __str2dec(result, "542101086242752217003726400434970855712890625", -20);
            return;
        case -53:
            __str2dec(result, "11102230246251565404236316680908203125", -16);
            return;
        case -32:
            __str2dec(result, "23283064365386962890625", -10);
            return;
        case -16:
            __str2dec(result, "152587890625", -5);
            return;
        case -8:
            __str2dec(result, "390625", -3);
            return;
        case -7:
            __str2dec(result, "78125", -3);
            return;
        case -6:
            __str2dec(result, "15625", -2);
            return;
        case -5:
            __str2dec(result, "3125", -2);
            return;
        case -4:
            __str2dec(result, "625", -2);
            return;
        case -3:
            __str2dec(result, "125", -1);
            return;
        case -2:
            __str2dec(result, "25", -1);
            return;
        case -1:
            __str2dec(result, "5", -1);
            return;
        case 0:
            __str2dec(result, "1", 0);
            return;
        case 1:
            __str2dec(result, "2", 0);
            return;
        case 2:
            __str2dec(result, "4", 0);
            return;
        case 3:
            __str2dec(result, "8", 0);
            return;
        case 4:
            __str2dec(result, "16", 1);
            return;
        case 5:
            __str2dec(result, "32", 1);
            return;
        case 6:
            __str2dec(result, "64", 1);
            return;
        case 7:
            __str2dec(result, "128", 2);
            return;
        case 8:
            __str2dec(result, "256", 2);
            return;
    }

    {
        decimal x2, temp;

        __two_exp(&x2, ((long) ((0x80000000UL & exp) >> 31) + exp) >> 1);
        __timesdec(result, &x2, &x2);

        if (exp & 1) {
            temp = *result;

            if (exp > 0) {
                __str2dec(&x2, "2", 0);
            } else {
                __str2dec(&x2, "5", -1);
            }

            __timesdec(result, &temp, &x2);
        }
    }
}

int __equals_dec(const decimal *x, const decimal *y) {
    int i, l;

    if (x->sig.text[0] == 0) {
        if (y->sig.text[0] == 0) {
            return 1;
        }
        return 0;
    }

    if (y->sig.text[0] == 0) {
        if (x->sig.text[0] == 0) {
            return 1;
        }
        return 0;
    }

    if (x->exp == y->exp) {
        l = x->sig.length;
        if (l > y->sig.length) {
            l = y->sig.length;
        }
        for (i = 0; i < l; ++i) {
            if (x->sig.text[i] != y->sig.text[i]) {
                return 0;
            }
        }
        if (l == x->sig.length) {
            x = y;
        }
        for (; i < x->sig.length; ++i) {
            if (x->sig.text[i] != 0) {
                return 0;
            }
        }
        return 1;
    }

    return 0;
}

int __less_dec(const decimal *x, const decimal *y) {
    int i, l;

    if (x->sig.text[0] == 0) {
        if (y->sig.text[0] != 0) {
            return 1;
        }
        return 0;
    }

    if (y->sig.text[0] == 0) {
        return 0;
    }

    if (x->exp == y->exp) {
        l = x->sig.length;
        if (l > y->sig.length) {
            l = y->sig.length;
        }
        for (i = 0; i < l; ++i) {
            if (x->sig.text[i] < y->sig.text[i]) {
                return 1;
            }
            if (y->sig.text[i] < x->sig.text[i]) {
                return 0;
            }
        }
        if (l == x->sig.length) {
            for (; i < y->sig.length; ++i) {
                if (y->sig.text[i] != 0) {
                    return 1;
                }
            }
        }
        return 0;
    }

    return x->exp < y->exp;
}

void __minus_dec(decimal *z, const decimal *x, const decimal *y) {
    int zlen, dexp;
    unsigned char *ib, *i, *ie;
    const unsigned char *jb, *j, *jn;

    *z = *x;

    if (y->sig.text[0] == 0) {
        return;
    }

    zlen = z->sig.length;
    if (zlen < y->sig.length) {
        zlen = y->sig.length;
    }

    dexp = z->exp - y->exp;
    zlen += dexp;

    if (zlen > SIGDIGLEN) {
        zlen = SIGDIGLEN;
    }

    while (z->sig.length < zlen) {
        z->sig.text[z->sig.length++] = 0;
    }

    ib = z->sig.text;
    i  = ib + zlen;

    if (y->sig.length + dexp < zlen) {
        i = ib + (y->sig.length + dexp);
    }

    jb = y->sig.text;
    j  = jb + (i - ib - dexp);
    jn = j;

    while (i > ib && j > jb) {
        --i;
        --j;
        if (*i < *j) {
            unsigned char *k = i - 1;
            while (*k == 0) {
                --k;
            }
            while (k != i) {
                --*k;
                *++k += 10;
            }
        }
        *i -= *j;
    }

    if (jn - jb < y->sig.length) {
        int round_down = 0;
        if (*jn < 5) {
            round_down = 1;
        } else if (*jn == 5) {
            const unsigned char *je = y->sig.text + y->sig.length;

            for (j = jn + 1; j < je; ++j) {
                if (*j != 0) {
                    goto done;
                }
            }
            i = ib + (jn - jb) + dexp - 1;
            if (*i & 1) {
                round_down = 1;
            }
        }
        if (round_down) {
            if (*i < 1) {
                unsigned char *k = i - 1;
                while (*k == 0) {
                    --k;
                }
                while (k != i) {
                    --*k;
                    *++k += 10;
                }
            }
            *i -= 1;
        }
    }

done:
    for (i = ib; *i == 0; ++i) {
    }

    if (i > ib) {
        unsigned char dl = (unsigned char) (i - ib);
        z->exp -= dl;
        ie = ib + z->sig.length;
        for (; i < ie; ++i, ++ib) {
            *ib = *i;
        }
        z->sig.length -= dl;
    }

    ib = z->sig.text;
    for (i = ib + z->sig.length; i > ib;) {
        --i;
        if (*i != 0) {
            break;
        }
    }
    z->sig.length = (unsigned char) (i - ib + 1);
}

void __num2dec_internal(decimal *d, double x) {
    unsigned char sign = signbit(x) != 0;

    if (x == 0) {
        d->sgn         = sign;
        d->exp         = 0;
        d->sig.length  = 1;
        d->sig.text[0] = 0;
        return;
    }

    if (!isfinite(x)) {
        d->sgn         = sign;
        d->exp         = 0;
        d->sig.length  = 1;
        d->sig.text[0] = fpclassify(x) == 1 ? 'N' : 'I';
        return;
    }

    if (sign != 0) {
        x = -x;
    }

    {
        int exp;
        double frac          = frexp(x, &exp);
        int num_bits_extract = 53 - count_trailing(frac);
        decimal int_d, pow2_d;

        __two_exp(&pow2_d, exp - num_bits_extract);
        __ull2dec(&int_d, ldexp(frac, num_bits_extract));
        __timesdec(d, &int_d, &pow2_d);
        d->sgn = sign;
    }
}

void __num2dec(const decform *form, double x, decimal *d) {
    short digits = form->digits;
    int i;
    __num2dec_internal(d, x);

    if (d->sig.text[0] > 9) {
        return;
    }

    if (digits > SIGDIGLEN) {
        digits = SIGDIGLEN;
    }

    __rounddec(d, digits);

    while (d->sig.length < digits) {
        d->sig.text[d->sig.length++] = 0;
    }

    d->exp -= d->sig.length - 1;

    for (i = 0; i < d->sig.length; i++) {
        d->sig.text[i] += '0';
    }
}

double __dec2num(const decimal *d) {
    decimal dec;
    decimal max;
    decimal tmp;
    decimal tmp2;
    decimal diff1;
    decimal diff2;
    double result;
    long long *bits;
    double val;
    double next;
    long long *nextbits;
    unsigned char *p;
    unsigned char *end;
    int exp10;
    int up;
    int down;

    if (d->sig.length == 0) {
        return copysign(0.0, d->sgn == 0 ? 1.0 : -1.0);
    }
    switch (d->sig.text[0]) {
    case '0':
        return copysign(0.0, d->sgn == 0 ? 1.0 : -1.0);
    case 'I':
        return copysign(INFINITY, d->sgn == 0 ? 1.0 : -1.0);
    case 'N':
        bits = (long long *)&result;
        *bits = 0x7ff0000000000000LL;
        if (d->sgn) {
            *bits |= 0x8000000000000000LL;
        }
        *bits |= 0x0008000000000000LL;
        return result;
    }
    dec = *d;
    end = dec.sig.text + dec.sig.length;
    for (p = dec.sig.text; p < end; p++) {
        *p -= '0';
    }
    dec.exp += dec.sig.length - 1;
    exp10 = dec.exp;
    __str2dec(&max, "179769313486231580793728714053034151", 308);
    if (__less_dec(&max, &dec)) {
        return copysign(INFINITY, d->sgn == 0 ? 1.0 : -1.0);
    }
    p = dec.sig.text;
    val = *p++;
    while (p < end) {
        int i;
        unsigned long chunk;
        double prod;
        double sum;
        int n;

        n = (end - p) % 8;
        chunk = 0;
        if (n == 0) {
            n = 8;
        }
        for (i = 0; i < n; i++) {
            chunk = chunk * 10 + *p++;
        }
        prod = val * pow_10[n - 1];
        sum = prod + chunk;
        if (chunk != 0 && prod == sum) {
            break;
        }
        val = sum;
        exp10 -= n;
    }
    if (exp10 < 0) {
        val /= pow(5.0, -exp10);
    } else {
        val *= pow(5.0, exp10);
    }
    val = ldexp(val, exp10);
    if (__fpclassifyd(val) == FP_INFINITE) {
        val = DBL_MAX;
    }
    nextbits = (long long *)&next;
    up = 0;
    __num2dec_internal(&tmp, val);
    if (!__equals_dec(&tmp, &dec)) {
        if (__less_dec(&tmp, &dec)) {
            up = 1;
        }
        next = val;
        down = !up;
        for (;;) {
            if (!down) {
                *nextbits += 1;
                if (__fpclassifyd(next) == FP_INFINITE) {
                    goto done;
                }
            } else {
                *nextbits -= 1;
            }
            __num2dec_internal(&tmp2, next);
            if (up && !__less_dec(&tmp2, &dec)) {
                break;
            }
            if (!up && !__less_dec(&dec, &tmp2)) {
                double v;

                diff1 = tmp;
                tmp = tmp2;
                tmp2 = diff1;
                v = val;
                val = next;
                next = v;
                break;
            }
            tmp = tmp2;
            val = next;
        }
        __minus_dec(&diff1, &dec, &tmp);
        __minus_dec(&diff2, &tmp2, &dec);
        if (__equals_dec(&diff1, &diff2)) {
            if ((*(long long *)&val & 1) != 0) {
                val = next;
            }
        } else if (!__less_dec(&diff1, &diff2)) {
            val = next;
        }
    }
done:
    if (dec.sgn) {
        val = doubleSub(0.0, val);
    }
    return val;
}


// Black's functions in order (Black's size), to write here:
// __dec2num: 0x5f4
