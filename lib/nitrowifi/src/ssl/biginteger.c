// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/ssl/biginteger.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "ssl_internal.h"
#include "gfl/std.h"
#include "nitrowifi/cps.h"

// A multi-precision integer is a little-endian array of n u16 digits, every length given explicitly by the caller.
// No swan name for the object or any of its functions; named for what each does from the asm's shape

// pokebw2's BigInti_GetLength
static int count_digits(const u16 *digits, int n) {
    while (n != 0 && digits[n - 1] == 0) {
        n--;
    }
    return n;
}

// pokebw2's BigInt_GetSign
s32 CPSi_big_sign(const u16 *digits, int n) {
    if (digits[n - 1] & 0x8000) {
        return -1;
    }
    return count_digits(digits, n) != 0;
}

// pokebw2's BigInt_Add
void CPSi_big_add(u16 *dst, const u16 *a, const u16 *b, int n) {
    int lenA = count_digits(a, n);
    int lenB = count_digits(b, n);
    int len = lenA >= lenB ? lenA : lenB;
    int i;
    u32 carry;

    if (len != n) {
        len++;
    }
    carry = 0;
    for (i = 0; i < len; i++) {
        u32 sum = carry + (a[i] + b[i]);
        dst[i] = (u16)sum;
        carry = sum >> 16;
    }
    if (dst != a && dst != b) {
        MI_CpuFill8(dst + i, 0, (n - i) * 2);
    }
}

// Adds value into dst/src starting at digit 0, propagating the carry, and copies the rest of src unchanged
// pokebw2's BigInti_AddDigit
static void CPSi_big_add_small(u16 *dst, const u16 *src, u32 value, int n) {
    int i;

    for (i = 0; i < n; i++) {
        value += src[i];
        dst[i] = (u16)value;
        value >>= 16;
        if (value == 0) {
            break;
        }
    }
    if (dst != src) {
        for (i++; i < n; i++) {
            dst[i] = src[i];
        }
    }
}

// pokebw2's BigInt_Negate
void CPSi_big_negate(u16 *digits, int n) {
    int i;

    for (i = 0; i < n; i++) {
        digits[i] = ~digits[i];
    }
    CPSi_big_add_small(digits, digits, 1, n);
}

// pokebw2's BigInt_Sub
void CPSi_big_sub(u16 *dst, const u16 *a, const u16 *b, int n) {
    int lenA = count_digits(a, n);
    int lenB = count_digits(b, n);
    int len = lenA >= lenB ? lenA : lenB;
    int i;
    s32 borrow;

    if (len != n) {
        len++;
    }
    borrow = 0;
    i = 0;
    while (i < len || (i < n && borrow != 0)) {
        borrow += (s32)a[i] - b[i];
        dst[i] = (u16)borrow;
        borrow >>= 16;
        i++;
    }
    if (dst != a && dst != b) {
        MI_CpuFill8(dst + i, 0, (n - i) * 2);
    }
}

// Subtracts value from src starting at digit 0, propagating the borrow, and copies the rest of src unchanged
// pokebw2's BigInti_SubDigit
static void CPSi_big_sub_small(u16 *dst, const u16 *src, u32 value, int n) {
    int i;

    for (i = 0; i < n; i++) {
        value = src[i] - value;
        dst[i] = (u16)value;
        value = (value >> 16) & 1;
        if (value == 0) {
            break;
        }
    }
    if (dst != src) {
        for (i++; i < n; i++) {
            dst[i] = src[i];
        }
    }
}

// pokebw2's BigInti_Compare
static int CPSi_big_compare(const u16 *a, const u16 *b, int n) {
    int i;

    for (i = n - 1; i >= 0; i--) {
        if (a[i] > b[i]) {
            return 1;
        }
        if (a[i] < b[i]) {
            return -1;
        }
    }
    return 0;
}

// Adds value into digits starting at index, propagating the carry, without touching digits below index
// pokebw2's BigInti_AddDigitFrom
static void CPSi_big_add_part(u16 *digits, u32 value, int index, int n) {
    while (value != 0 && index < n) {
        value += digits[index];
        digits[index] = (u16)value;
        value >>= 16;
        index++;
    }
}

// pokebw2's BigInt_Mul
void CPSi_big_mult(u16 *dst, const u16 *a, const u16 *b, int n) {
    int lenA, lenB;
    int i;

    MI_CpuFill8(dst, 0, n * 2);
    lenA = count_digits(a, n);
    lenB = count_digits(b, n);
    for (i = 0; i < lenB; i++) {
        int j = 0;
        int maxJ = n - i;

        for (; j < lenA && j < maxJ; j++) {
            CPSi_big_add_part(dst, (u32)a[j] * b[i], i + j, n);
        }
    }
}

// pokebw2's BigInti_MulDigit
static void CPSi_big_mult_small(u16 *dst, const u16 *src, u32 value, int n) {
    int len = count_digits(src, n);
    u32 carry = 0;
    int i;

    for (i = 0; i < len; i++) {
        carry += src[i] * value;
        dst[i] = (u16)carry;
        carry >>= 16;
    }
    if (i < n) {
        dst[i++] = (u16)carry;
    }
    MI_CpuFill8(dst + i, 0, (n - i) * 2);
}

// pokebw2's BigInti_Square
static void CPSi_big_sqr(u16 *dst, const u16 *src, int n) {
    int len = count_digits(src, n);
    int doubleLen = len * 2;
    int k;
    int pos;
    int outer;
    int inner;
    u32 product;

    if (doubleLen < n) {
        MI_CpuFill8(dst + doubleLen, 0, (n - doubleLen) * 2);
    }

    for (k = 0; k < len; k++) {
        int idx = k * 2;
        u32 sq;

        if (idx >= n) {
            break;
        }
        sq = (u32)src[k] * src[k];
        dst[idx] = (u16)sq;
        if (idx == n - 1) {
            break;
        }
        dst[idx + 1] = (u16)(sq >> 16);
    }

    for (outer = 0; outer < len; outer++) {
        for (inner = outer + 1; inner < len && (pos = outer + inner) < n; inner++) {
            
            product = (u32)src[inner] * src[outer];

            if (product <= 0x7fff8000) {
                CPSi_big_add_part(dst, product * 2, pos, n);
            } else {
                CPSi_big_add_part(dst, product, pos, n);
                CPSi_big_add_part(dst, product, pos, n);
            }
        }
    }
}

s64 get48bits_1(u16 *s) {
    return (u64)*s << 0x20;
}

// The top digits as a u64, built in r1:r0 directly. These three are Nintendo's assembly: no MWCC version or level
// compiles any C of them to this (each load used at once, no zero halves); NitroWiFi ships SSL only as a library.
#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): p[0] in the high word, p[-1] in
// the low one's top half
u64 get48bits_2(u16 *p) {
    return ((u64)p[0] << 32) | ((u32)p[-1] << 16);
}
#else
asm u64 get48bits_2(register u16 *p) {
    ldrh r1, [r0, #0]
    ldrh r0, [r0, #-2]
    mov r0, r0, lsl #16
    bx lr
}
#endif

#ifndef __MWERKS__
u64 get48bits_3(u16 *p) {
    return ((u64)p[0] << 32) | ((u32)p[-1] << 16) | p[-2];
}
#else
asm u64 get48bits_3(register u16 *p) {
    ldrh r1, [r0, #0]
    ldrh r2, [r0, #-2]
    ldrh r3, [r0, #-4]
    orr r0, r3, r2, lsl #16
    bx lr
}
#endif

#ifndef __MWERKS__
u64 get64bits(u16 *p) {
    return ((u64)p[0] << 48) | ((u64)p[-1] << 32) | ((u32)p[-2] << 16) | p[-3];
}
#else
asm u64 get64bits(register u16 *p) {
    ldrh r2, [r0, #0]
    ldrh r3, [r0, #-2]
    orr r1, r3, r2, lsl #16
    ldrh r2, [r0, #-4]
    ldrh r3, [r0, #-6]
    orr r0, r3, r2, lsl #16
    bx lr
}
#endif




// Divides dividend by divisor, both n digits long. quotient and remainder may each be NULL; workBuf needs 3*n
// digits of scratch
// pokebw2's BigInt_Div
void CPSi_big_div(u16 *quotient, const u16 *dividend, const u16 *divisor, u16 *remainder, int n, u16 *workBuf) {
    int pos;
    int lenB;
    int lenA;
    u32 digit;
    u16 *qbuf;
    u16 *rem;

    qbuf = workBuf + n;
    rem = qbuf + n;
    MI_CpuFill8(qbuf, 0, n * 4);
    lenA = count_digits(dividend, n);
    lenB = count_digits(divisor, n);

    if (lenA > 0 && lenB > 0) {
        pos = lenB + (n - lenA) - 1;

        if (pos >= n) {
            MI_CpuCopy8(dividend, rem, n * 2);
        } else {
            u64 divisorTop;

            MI_CpuCopy8(dividend, qbuf + pos, lenA * 2);

            if (lenB > 2) {
                divisorTop = get48bits_3((u16 *)divisor + lenB - 1);
            } else if (lenB > 1) {
                divisorTop = get48bits_2((u16 *)divisor + lenB - 1);
            } else {
                divisorTop = get48bits_1((u16 *)divisor + lenB - 1);
            }

            for (; pos < n; pos++) {
                u64 numerator;

                memmove(qbuf + 1, qbuf, (2 * n - 1) * 2);
                numerator = get64bits(rem + lenB);
                digit = (u32)(numerator / divisorTop);
                if (digit > 0xffff) {
                    digit = 0xffff;
                }

                for (;;) {
                    CPSi_big_mult_small(workBuf, divisor, (u16)digit, n);
                    if (CPSi_big_compare(rem, workBuf, n) >= 0) {
                        break;
                    }
                    digit--;
                }
                CPSi_big_sub(rem, rem, workBuf, n);
                qbuf[0] = (u16)digit;
            }
        }
    }

    if (quotient != NULL) {
        MI_CpuCopy8(qbuf, quotient, n * 2);
    }
    if (remainder != NULL) {
        MI_CpuCopy8(rem, remainder, n * 2);
    }
}

// pokebw2's BigInt_ModExp
void CPSi_big_power(u16 *dst, const u16 *base, const u16 *exponent, int n, const u16 *modulus) {
    u16 *work = data_ov009_021a8280(n * 8);
    u16 *temp;
    int expLen;
    u32 totalBits;
    int bit;

    if (work == NULL) {
        return;
    }
    temp = work + n;

    MI_CpuFill8(dst + 1, 0, (n - 1) * 2);
    dst[0] = 1;

    expLen = count_digits(exponent, n);
    bit = (n - expLen) * 16;
    totalBits = n * 16;

    while (bit < totalBits) {
        if (exponent[n - (bit >> 4) - 1] & (0x8000u >> (bit & 0xf))) {
            MI_CpuCopy8(base, dst, n * 2);
            bit++;
            break;
        }
        bit++;
    }

    while (bit < totalBits) {
        CPSi_big_sqr(work, dst, n);
        MI_CpuCopy8(work, dst, n * 2);
        if (modulus != NULL) {
            CPSi_big_div(NULL, dst, modulus, dst, n, temp);
        }
        if (exponent[n - (bit >> 4) - 1] & (0x8000u >> (bit & 0xf))) {
            CPSi_big_mult(work, dst, base, n);
            MI_CpuCopy8(work, dst, n * 2);
            if (modulus != NULL) {
                CPSi_big_div(NULL, dst, modulus, dst, n, temp);
            }
        }
        bit++;
    }

    data_ov009_021a82b0(work);
}

// Computes result = a modular inverse-like value of value mod modulus, through the extended Euclidean algorithm's
// division steps; scratch needs 6*n digits
// pokebw2's BigInti_ModInverse
static void CPSi_big_modinv(u16 *result, const u16 *value, const u16 *modulus, int n, u16 *scratch) {
    u16 *quotient = scratch + n;
    u16 *big = quotient + n;
    u16 *coefCur = big + n;
    u16 *coefPrev = coefCur + n;
    u16 *remainder = coefPrev + n;
    u16 *workBuf = remainder + n;
    int size = n * 2;

    MI_CpuCopy8(value, scratch, size);
    MI_CpuCopy8(modulus, big, size);
    *(u16 *)((u8 *)big + size) = 1;

    if (CPSi_big_sign(scratch, n) > 0) {
        do {
            CPSi_big_div(quotient, big, scratch, remainder, n, workBuf);
            MI_CpuCopy8(scratch, big, size);
            MI_CpuCopy8(remainder, scratch, size);
            CPSi_big_mult(remainder, quotient, coefCur, n);
            CPSi_big_sub(remainder, coefPrev, remainder, n);
            MI_CpuCopy8(coefCur, coefPrev, size);
            MI_CpuCopy8(remainder, coefCur, size);
        } while (CPSi_big_sign(scratch, n) > 0);
    }

    CPSi_big_add(coefPrev, coefPrev, modulus, n);
    CPSi_big_div(NULL, coefPrev, modulus, result, n, workBuf);
}

// One Montgomery-ish step: snapshots acc into scratch, squares it (multiplier == BIGINT_SQUARE) or multiplies it by
// multiplier, then reduces the result modulo modulus using the precomputed reciprocal mu, with q and t as scratch
static void CPSi_big_montmult(u16 *acc, u16 *scratch, const u16 *multiplier, int n, int modLen, const u16 *modulus,
                                const u16 *mu, u16 *q, u16 *t) {
    int size = n * 2;
    int hiSize;

    MI_CpuCopy8(acc, scratch, size);
    if (multiplier == BIGINT_SQUARE) {
        CPSi_big_sqr(acc, scratch, n);
    } else if (multiplier != NULL) {
        CPSi_big_mult(acc, scratch, multiplier, n);
    }

    CPSi_big_mult(q, acc, mu, modLen);
    hiSize = (n - modLen) * 2;
    MI_CpuFill8(q + modLen, 0, hiSize);
    CPSi_big_mult(t, q, modulus, n);
    CPSi_big_add(acc, acc, t, n);
    memmove(acc, acc + modLen, hiSize);
    MI_CpuFill8(acc + n - modLen, 0, modLen * 2);

    switch (CPSi_big_compare(acc, modulus, n)) {
    case 0:
        MI_CpuFill8(acc, 0, size);
        break;
    case 1:
        CPSi_big_sub(acc, acc, modulus, n);
        break;
    }
}

// pokebw2's BigInt_ModExpFast
void CPSi_big_montpower(u16 *dst, const u16 *base, const u16 *exponent, int n, const u16 *modulus) {
    u16 *work = data_ov009_021a8280(n * 22);
    int modLen;
    int bit;
    u16 *mu;
    u16 *montBase;
    u16 *buf3;
    u16 *buf4;
    u16 *buf5;
    u16 *workBuf;
    u32 totalBits;

    if (work == NULL) {
        return;
    }
    MI_CpuFill8(work, 0, n * 22);

    mu = work + n;
    montBase = mu + n;
    buf3 = montBase + n;
    buf4 = buf3 + n;
    buf5 = buf4 + n;
    workBuf = buf5 + n;

    modLen = count_digits(modulus, n);
    work[modLen] = 1;

    CPSi_big_modinv(mu, work, modulus, n, montBase);
    CPSi_big_mult(buf3, work, mu, n);
    CPSi_big_sub_small(mu, buf3, 1, n);
    CPSi_big_div(mu, mu, modulus, NULL, n, workBuf);

    CPSi_big_mult(montBase, base, work, n);
    CPSi_big_div(NULL, montBase, modulus, montBase, n, workBuf);

    CPSi_big_div(NULL, work, modulus, dst, n, workBuf);

    totalBits = modLen * 16;
    for (bit = 0; bit < totalBits; bit++) {
        CPSi_big_montmult(dst, buf5, BIGINT_SQUARE, n, modLen, modulus, mu, buf3, buf4);
        if (exponent[modLen - (bit >> 4) - 1] & (0x8000u >> (bit & 0xf))) {
            CPSi_big_montmult(dst, buf5, montBase, n, modLen, modulus, mu, buf3, buf4);
        }
    }
    CPSi_big_montmult(dst, buf5, NULL, n, modLen, modulus, mu, buf3, buf4);

    data_ov009_021a82b0(work);
}

// pokebw2's BigInt_BytesToDigits
void CPSi_big_from_char(u16 *dst, const u8 *src, int nbytes, int n) {
    int val;

    val = 8;
    MI_CpuFill8(dst, 0, n * 2);
    src = &src[nbytes - 1];
    while (nbytes > 1) {
        u16 hi = src[-1];
        u16 lo = src[0];

        nbytes -= 2;
        *(dst++) = (u16)((hi << val) + lo);
        src -= 2;
    }
    if (nbytes > 0) {
        *dst = src[0];
    }
}

// The original accepts the source digit count n but overwrites its register before reading it.
// pokebw2's BigInt_DigitsToBytes
void CPSi_char_from_big(u8 *dst, u16 *src, int nbytes, int n) {
    dst += nbytes - 1;
    while (nbytes > 1) {
        u8 *hi = dst - 1;
        *dst = *src;
        nbytes -= 2;
        dst -= 2;
        *hi = *src++ >> 8;
    }
    if (nbytes > 0) {
        *dst = *src;
    }
}
