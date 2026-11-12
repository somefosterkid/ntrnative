// Black's code at ov009 0x0217d3ac-0x0217ea58. GameSpy's big-number code (gsLargeInt's role; name guessed): add,
// subtract, multiply, divide, compare, hex string conversion, with the timing counters 0x0217d3ac/0x0217d3f8.

#include "string.h"
#include "types.h"
#include "gs/nonport.h"
#include "gs/md5c.h"
#include "gs/largeint.h"


typedef struct LargeIntTimer {
    gsi_time start;
    gsi_time total;
    u32 count;
    u32 running;
} LargeIntTimer;

extern LargeIntTimer data_ov009_021ad064[];

#pragma thumb off

void func_ov009_0217d3f8(s32 param_1);
BOOL func_ov009_0217d8e0(const LargeIntBuf *n, LargeIntBuf *d, LargeIntBuf *quot, LargeIntBuf *rem);
BOOL func_ov009_0217d904(u32 *n, u32 nLen, LargeIntBuf *d, LargeIntBuf *quotOut, LargeIntBuf *remOut);
int func_ov009_0217d448(const u32 *a, u32 aLength, const u32 *b, u32 bLength);
u32 func_ov009_0217dca4(u32 *a, u32 aLen, u32 *b, u32 bLen, u32 unused, s32 *out);
BOOL func_ov009_0217e7c4(LargeIntBuf *mod, u32 *dest);
BOOL func_ov009_0217e504(LargeIntBuf *a, LargeIntBuf *b, const LargeIntBuf *mod, u32 modInv, LargeIntBuf *dest);
BOOL func_ov009_0217e934(LargeIntBuf *n);
int func_ov009_0217e9ec(const LargeIntBuf *value);

void func_ov009_0217d3ac(s32 index) {
    if (data_ov009_021ad064[index].running == 0) {
        data_ov009_021ad064[index].count++;
        data_ov009_021ad064[index].start = current_time_hires();
        data_ov009_021ad064[index].running = 1;
    }
}

void func_ov009_0217d3f8(s32 index) {
    if (data_ov009_021ad064[index].running == 1) {
        data_ov009_021ad064[index].total += current_time_hires() - data_ov009_021ad064[index].start;
        data_ov009_021ad064[index].running = 0;
    }
}

int func_ov009_0217d448(const u32 *a, u32 aLength, const u32 *b, u32 bLength) {
    while (a[aLength - 1] == 0 && aLength != 0) {
        aLength--;
    }
    while (b[bLength - 1] == 0 && bLength != 0) {
        bLength--;
    }
    if (aLength < bLength) {
        return -1;
    }
    if (aLength > bLength) {
        return 1;
    }
    for (; aLength != 0; aLength--) {
        if (a[aLength - 1] < b[aLength - 1]) {
            return -1;
        }
        if (a[aLength - 1] > b[aLength - 1]) {
            return 1;
        }
    }
    return 0;
}

#pragma thumb on

#pragma thumb off
BOOL func_ov009_0217d4d8(LargeIntBuf *lint) {
    while (lint->length != 0 && lint->data[lint->length - 1] == 0) {
        lint->length--;
    }
    return TRUE;
}

#include "mem.h"
#include "nitro/std.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
typedef struct BigNum {
    u32 length;
    u32 data[0x40];
} BigNum;

u32 func_ov009_0217d508(u32 *a, u32 aLen, u32 *b, u32 bLen, u32 *dest, u32 *destLen, u32 maxLen) {
    u64 carry;
    u32 i;
    u32 shortest;
    u32 longLen;
    u32 *longer;

    i = 0;
    carry = 0;
    func_ov009_0217d3ac(3);
    if (maxLen < aLen || maxLen < bLen) {
        return 0;
    }
    if (aLen < bLen) {
        shortest = aLen;
        longLen = bLen;
        longer = b;
    } else {
        shortest = bLen;
        longLen = aLen;
        longer = a;
    }
    for (; i < shortest; i++) {
        carry += (u64)a[i] + b[i];
        dest[i] = (u32)carry;
        carry >>= 32;
    }
    while (carry != 0 && i < longLen) {
        carry += longer[i];
        dest[i] = (u32)carry;
        carry >>= 32;
        i++;
    }
    if (carry != 0) {
        if (maxLen <= i) {
            return 0;
        }
        dest[i] = (u32)carry;
        carry = 0;
        i++;
    }
    if (i < longLen) {
        if (dest + i != longer + i) {
            memcpy(dest + i, longer + i, (longLen - i) * 4);
        }
        i = longLen;
    }
    *destLen = i;
    func_ov009_0217d3f8(3);
    return carry == 0;
}


u32 func_ov009_0217d640(u32 *a, u32 aLen, u32 *b, u32 bLen, u32 *dest, u32 *destLen) {
    u64 borrow, diff;
    u32 i;
    u32 shortest;

    borrow = 0;
    if (aLen < bLen) {
        shortest = aLen;
    } else {
        shortest = bLen;
    }
    i = 0;
    func_ov009_0217d3ac(4);
    for (; i < shortest; i++) {
        diff = (u64)b[i] - a[i] - borrow;
        dest[i] = (u32)diff;
        borrow = diff >> 63;
    }
    for (; i < bLen; i++) {
        diff = (u64)b[i] - borrow;
        dest[i] = (u32)diff;
        borrow = diff >> 63;
    }
    if (borrow != 0) {
        func_ov009_0217d3f8(4);
        return 0;
    }
    for (; i < aLen; i++) {
        if (a[i] != 0) {
            func_ov009_0217d3f8(4);
            return 0;
        }
    }
    *destLen = bLen;
    func_ov009_0217d3f8(4);
    return 1;
}

u32 func_ov009_0217d750(u32 *a, u32 aLen, u32 *b, u32 bLen, void *dest, u32 *destLen, u32 maxLen) {
    u32 temp[65] = {0};
    int len;
    u32 i;
    u32 j;
    s32 k;
    u64 prod;

    *destLen = 0;
    func_ov009_0217d3ac(0);
    for (i = 0; i < bLen; i++) {
        if (b[i] == 0) {
            continue;
        }
        for (j = 0; j < aLen; j++) {
            k = i + j;
            prod = (u64)a[j] * b[i];
            if (k >= maxLen) {
                func_ov009_0217d3f8(0);
                return 0;
            }
            while (prod != 0) {
                prod += temp[k + 1];
                temp[k + 1] = (u32)prod;
                prod >>= 32;
                k++;
                if (k > maxLen || (k == maxLen && prod != 0)) {
                    func_ov009_0217d3f8(0);
                    return 0;
                }
            }
            if (k > (s32)temp[0]) {
                temp[0] = k;
            }
        }
    }
    while (temp[0] != 0 && temp[temp[0]] == 0) {
        temp[0]--;
    }
    len = temp[0];
    *destLen = len;
    memcpy(dest, &temp[1], len << 2);
    func_ov009_0217d3f8(0);
    return 1;
}

BOOL func_ov009_0217d8e0(const LargeIntBuf *n, LargeIntBuf *d, LargeIntBuf *quot, LargeIntBuf *rem) {
    return func_ov009_0217d904(n->data, n->length, d, quot, rem);
}


BOOL func_ov009_0217d904(u32 *n, u32 nLen, LargeIntBuf *d, LargeIntBuf *quotOut, LargeIntBuf *remOut) {
    int bit = 31;
    u32 *dData = d->data;
    u32 dLen = d->length;
    BOOL done = FALSE;
    s32 off;
    s32 cur;
    u32 topWord;
    s32 top;
    LargeIntBuf quot;
    u32 nBytes;

    func_ov009_0217d3ac(5);
    u32 t[128] = {0};
    if (nLen > 64 && quotOut != NULL) {
        return 0;
    }
    while (nLen != 0 && n[nLen - 1] == 0) {
        nLen--;
    }
    while (dLen != 0 && dData[dLen - 1] == 0) {
        dLen--;
    }
    nBytes = nLen << 2;
    memcpy(t, n, nBytes);
    memset(&quot, 0, sizeof(quot));
    if (nLen == 0 || dLen == 0) {
        if (quotOut != NULL) {
            quotOut->data[0] = 0;
            quotOut->length = 0;
        }
        if (remOut != NULL) {
            remOut->data[0] = 0;
            remOut->length = 0;
        }
        func_ov009_0217d3f8(5);
        if (dLen == 0) {
            return FALSE;
        }
        return TRUE;
    }
    if (func_ov009_0217d448(t, nLen, dData, dLen) == -1) {
        if (quotOut != NULL) {
            quotOut->length = 0;
            quotOut->data[0] = 0;
        }
        remOut->length = nLen;
        memcpy(remOut->data, t, nBytes);
        func_ov009_0217d3f8(5);
        return 1;
    }
    top = dLen - 1;
    topWord = dData[top];
    while ((topWord & (1 << bit)) == 0 && bit >= 0) {
        bit--;
    }
    if (bit == -1) {
        func_ov009_0217d3f8(5);
        return 0;
    }
    cur = dLen;
    bit += top * 32;
    off = nLen - dLen;
    do {
        s32 cmp = func_ov009_0217d448(&t[off], cur, dData, dLen);
        if (cmp == -1) {
            if (off > 0) {
                off--;
                cur++;
            } else {
                done = TRUE;
            }
        } else if (cmp == 0) {
            memset(&t[off], 0, cur << 2);
            quot.data[off]++;
            if (quot.length < off + cur) {
                quot.length = off + cur;
            }
            off -= cur;
            cur = 1;
            if (off < 0) {
                done = TRUE;
            }
        } else {
            long est = 0;
            if (!func_ov009_0217dca4(&t[off], cur, dData, dLen, bit, &est)) {
                func_ov009_0217d3f8(5);
                return 0;
            }
            quot.data[off] = est + quot.data[off];
            if (quot.length < off + cur) {
                quot.length = off + cur;
            }
            while (t[off + cur - 1] == 0 && cur > 1) {
                cur--;
            }
            while (t[cur + off - 1] == 0 && off > 1) {
                off--;
            }
        }
    } while (!done);
    if (off >= 0) {
        memcpy(remOut->data, &t[off], cur << 2);
        remOut->length = cur;
    } else {
        remOut->data[0] = 0;
        remOut->length = 0;
    }
    if (quotOut != NULL) {
        memcpy(quotOut->data, quot.data, quot.length << 2);
        quotOut->length = quot.length;
    }
    func_ov009_0217d3f8(5);
    return 1;
}


u32 func_ov009_0217dca4(u32 *a, u32 aLen, u32 *b, u32 bLen, u32 unused, s32 *out) {
    BigNum product;
    u64 quot;

    func_ov009_0217d3ac(6);
    if (aLen > bLen) {
        quot = ((u64)a[aLen - 1] << 32) | (a[aLen - 2] & b[bLen - 1]);
    } else {
        quot = a[aLen - 1];
    }
    quot /= b[bLen - 1];
    BigNum quotient = { 0 };

    quotient.data[0] = quot;
    quotient.data[1] = quot >> 32;
    if (quotient.data[1] != 0) {
        quotient.data[0] = 0xffffffff;
        quotient.data[1] = 0;
    }
    quotient.length = 1;
    if (!func_ov009_0217d750(b, bLen, quotient.data, quotient.length, product.data, &product.length, 0x40)) {
        func_ov009_0217d3f8(6);
        return 0;
    }
    while (func_ov009_0217d448(product.data, product.length, a, aLen) == 1) {
        quotient.data[0] >>= 1;
        if (!func_ov009_0217d750(b, bLen, quotient.data, quotient.length, product.data, &product.length, 0x40)) {
            func_ov009_0217d3f8(6);
            return 0;
        }
    }
    func_ov009_0217d640(product.data, product.length, a, aLen, a, &aLen);
    *out = quotient.data[0];
    func_ov009_0217d3f8(6);
    return 1;
}

BOOL func_ov009_0217de94(const LargeIntBuf *lint, LargeIntBuf *mod, LargeIntBuf *dest) {
    int len;
    u64 carry = 0;
    u32 overflow = 0;
    u32 destLen = 0;
    int i;
    int k;
    BOOL result;

    len = lint->length;
    func_ov009_0217d3ac(7);
    {
        u32 squares[128] = {0};
        u32 temp[128] = {0};

        for (i = 1; i < len; i++) {
            for (k = 0; k < i; k++) {
                carry += (u64)lint->data[i] * lint->data[k] + temp[i + k];
                temp[i + k] = (u32)carry;
                carry >>= 32;
            }
            if (carry != 0) {
                temp[i + k] = (u32)carry;
                carry >>= 32;
            }
        }

        for (i = 0; i < len * 2; i++) {
            u32 next = (temp[i] & 0x80000000) == 0x80000000 ? 1 : 0;
            temp[i] = overflow + (temp[i] << 1);
            overflow = next;
        }

        for (i = 0; i < len; i++) {
            u64 sq = (u64)lint->data[i] * lint->data[i];
            squares[i * 2] = (u32)sq;
            squares[i * 2 + 1] = (u32)(sq >> 32);
        }

        destLen = len * 2;
        func_ov009_0217d508(temp, destLen, squares, destLen, squares, &destLen, 128);
        result = func_ov009_0217d904(squares, destLen, mod, NULL, dest);
    }
    func_ov009_0217d3f8(7);
    return result;
}

BOOL func_ov009_0217e078(const LargeIntBuf *b, const LargeIntBuf *p, const LargeIntBuf *m, LargeIntBuf *dest) {
    LargeIntBuf R;
    LargeIntBuf Rmod;
    LargeIntBuf R2;
    LargeIntBuf one;
    LargeIntBuf baseM;
    LargeIntBuf base;
    LargeIntBuf power;
    LargeIntBuf mod;
    u32 modInv;
    int bits;
    int top;
    int i;

    func_ov009_0217d3ac(8);
    memset(&R, 0, sizeof(R));
    memset(&Rmod, 0, sizeof(Rmod));
    memset(&R2, 0, sizeof(R2));
    memset(&one, 0, sizeof(one));
    memset(&baseM, 0, sizeof(baseM));
    memcpy(&base, b, sizeof(LargeIntBuf));
    memcpy(&power, p, sizeof(LargeIntBuf));
    memcpy(&mod, m, sizeof(LargeIntBuf));
    func_ov009_0217d4d8(&base);
    func_ov009_0217d4d8(&power);
    func_ov009_0217d4d8(&mod);

    if (mod.length == 0) {
        dest->length = 0;
        func_ov009_0217d3f8(8);
        return FALSE;
    }
    if (mod.length == 1 && mod.data[0] == 1) {
        dest->length = 0;
        dest->data[0] = 0;
        func_ov009_0217d3f8(8);
        return TRUE;
    }
    if (power.length == 0) {
        dest->length = 1;
        dest->data[0] = 1;
        func_ov009_0217d3f8(8);
        return TRUE;
    }
    if ((mod.data[0] & 1) == 0) {
        dest->length = 0;
        dest->data[0] = 0;
        func_ov009_0217d3f8(8);
        return FALSE;
    }
    if (func_ov009_0217d448(base.data, base.length, mod.data, mod.length) != -1) {
        func_ov009_0217d8e0(&base, &mod, NULL, &base);
    }
    if (base.length == 0) {
        dest->length = 0;
        dest->data[0] = 0;
        func_ov009_0217d3f8(8);
        return TRUE;
    }

    top = power.length - 1;
    bits = 32;
    while ((power.data[power.length - 1] & (1 << (bits - 1))) == 0) {
        bits--;
    }
    bits += top * 32;

    memset(&R, 0, sizeof(R));
    R.length = mod.length + 1;
    if (R.length > 64) {
        return FALSE;
    }
    R.data[R.length - 1] = 1;
    func_ov009_0217e7c4(&mod, &modInv);
    if (!func_ov009_0217d8e0(&R, &mod, NULL, &Rmod)) {
        func_ov009_0217d3f8(8);
        return FALSE;
    }
    if (!func_ov009_0217de94(&Rmod, &mod, &R2)) {
        func_ov009_0217d3f8(8);
        return FALSE;
    }
    if (!func_ov009_0217e504(&base, &R2, &mod, modInv, &baseM)) {
        func_ov009_0217d3f8(8);
        return FALSE;
    }
    memcpy(dest, &Rmod, sizeof(LargeIntBuf));
    for (i = bits - 1; i >= 0; i--) {
        func_ov009_0217e504(dest, dest, &mod, modInv, dest);
        if ((power.data[(u32)i >> 5] & (1 << (i & 31))) == (1 << (i & 31))) {
            func_ov009_0217e504(dest, &baseM, &mod, modInv, dest);
        }
    }
    one.length = 1;
    one.data[0] = 1;
    func_ov009_0217e504(dest, &one, &mod, modInv, dest);
    func_ov009_0217d3f8(8);
    return TRUE;
}


BOOL func_ov009_0217e504(LargeIntBuf *a, LargeIntBuf *b, const LargeIntBuf *mod, u32 modInv, LargeIntBuf *dest) {
    int len = mod->length;
    u32 tempLen = 0;
    u32 temp[128] = {0};
    u32 *src;
    u32 *srcEnd;
    const u32 *modI;
    const u32 *modEnd;
    u32 *dst;
    u64 m;
    u64 carry;

    if (!func_ov009_0217d750(a->data, a->length, b->data, b->length, temp, &tempLen, 128)) {
        return FALSE;
    }
    srcEnd = &temp[mod->length - 1];
    modEnd = &mod->data[mod->length - 1];
    if (tempLen < mod->length * 2) {
        memset(&temp[tempLen], 0, (mod->length * 2 - tempLen) * 4);
        tempLen = mod->length * 2;
    }
    for (src = temp; src <= srcEnd; src++) {
        m = (u32)(modInv * (u64)*src);
        modI = mod->data;
        carry = 0;
        dst = src;
        if (modI <= modEnd) {
            for (; modI <= modEnd; modI++) {
                carry = m * *modI + *dst + (carry >> 32);
                *dst = (u32)carry;
                dst++;
            }
        }
        for (; (carry >> 32) != 0 && dst <= &temp[tempLen - 1]; dst++) {
            carry = (u64)*dst + (carry >> 32);
            *dst = (u32)carry;
        }
        if ((carry >> 32) != 0) {
            *dst = (u32)(carry >> 32);
            tempLen++;
        }
    }
    dst = temp + len;
    if (func_ov009_0217d448(dst, tempLen - len, mod->data, mod->length) != -1) {
        if (!func_ov009_0217d640(mod->data, mod->length, dst, tempLen - len, dest->data, &dest->length)) {
            memset(temp, 0, sizeof(temp));
            memset(dest, 0, sizeof(LargeIntBuf));
            return FALSE;
        }
    } else {
        memset(dest, 0, sizeof(LargeIntBuf));
        dest->length = mod->length;
        memcpy(dest->data, &temp[len], (tempLen - len) * 4);
        memset(temp, 0, sizeof(temp));
    }
    return TRUE;
}



BOOL func_ov009_0217e7c4(LargeIntBuf *mod, u32 *dest) {
    u64 x = 2;
    u64 y = 1;
    u32 i;
    u64 check;

    for (i = 2; i <= 32; i++) {
        check = (u64)mod->data[0] * y;
        if (x < (check & ((x << 1) - 1))) {
            y += x;
        }
        x <<= 1;
    }
    *dest = (u32)(x - y);
    return TRUE;
}

BOOL func_ov009_0217e848(LargeIntBuf *dest, const char *hex) {
    u32 *word = dest->data;
    u32 len;
    u32 byteIdx = 0;
    u32 val;
    u32 step;

    len = strlen(hex);
    if (len == 0) {
        dest->length = 0;
        dest->data[0] = 0;
        return TRUE;
    }
    if (len / 2 > 0x100) {
        return FALSE;
    }
    dest->length = (len + 7) / 8;
    dest->data[dest->length - 1] = 0;
    while (len != 0) {
        if (len >= 2) {
            STD_TSScanf(hex + len - 2, "%02x", &val);
        } else {
            STD_TSScanf(hex + len - 1, "%1x", &val);
        }
        if (byteIdx == 0) {
            *word = 0;
        }
        *word |= val << (byteIdx * 8);
        byteIdx++;
        if (byteIdx == 4) {
            byteIdx = 0;
            word++;
        }
        step = 2;
        if (len <= 2) {
            step = len;
        }
        len -= step;
    }
    return TRUE;
}


BOOL func_ov009_0217e934(LargeIntBuf *n) {
    s8 *lo = (s8 *)n->data;
    s8 *hi = (s8 *)(n->data + n->length) - 1;
    s8 t;

    if (n->length == 0) {
        return TRUE;
    }
    while (lo < hi) {
        t = *lo;
        *lo++ = *hi;
        *hi-- = t;
    }
    return TRUE;
}

void func_ov009_0217e978(const LargeIntBuf *value, s32 *context) {
    LargeIntBuf copy;
    u32 size;
    u8 *bytes;

    memcpy(&copy, value, sizeof(LargeIntBuf));
    size = func_ov009_0217e9ec(&copy);
    if (size != 0) {
        bytes = (u8 *)copy.data;
        if ((size & 3) != 0) {
            bytes += 4 - (size & 3);
        }
        func_ov009_0217e934(&copy);
        func_ov009_0218497c(context, bytes, size);
        func_ov009_0217e934(&copy);
    }
}



int func_ov009_0217e9ec(const LargeIntBuf *value) {
    int length = value->length;
    int bytes;
    u32 i;
    u32 mask;

    while (length > 0 && value->data[length - 1] == 0) {
        length--;
    }
    if (length == 0) {
        return 0;
    }
    bytes = length * 4;
    mask = 0xff;
    for (i = 1; i < 4; i++) {
        if (value->data[length - 1] <= mask) {
            bytes -= 4 - i;
            break;
        }
        mask <<= 8;
        mask |= 0xff;
    }
    return bytes;
}





#pragma thumb on
