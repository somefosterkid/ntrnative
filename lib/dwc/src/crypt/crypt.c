// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/crypt/crypt.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dwc/crypt.h"

#include "crypt_internal.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "gfl/std.h"
#include "nitro/math.h"
#include "system/aeabi.h"

// The signature check of overlay 189 (ARM code beside the HTTP client): the server's answer to a Pokémon's check is signed,
// and this library verifies the signature with an RSA public key, a SHA-1 digest and a library of big numbers. The ROM
// names none of it, so the names say what the code does; they are none of the library's own.
//
// A big number keeps its words, the least significant first, with a length in words that has no zero word on top, and
// the sign apart. The calls that need working numbers take them from a pool, which hands out the next of its twelve
// numbers and takes them back in order.

#define BIGNUM_HEAP 0x1
#define BIGNUM_STATIC 0x2
#define BIGNUM_FREED 0x8000

#define POOL_SIZE 12
#define POOL_HEAP 0x1
#define POOL_CANCELED 0x4000

typedef struct BigNum {
    u32 *digits;
    int length;
    int capacity;
    int negative;
    u32 flags;
} BigNum;

typedef struct CryptProgress CryptProgress;
struct CryptProgress {
    int (*func)(CryptProgress *progress, int a1, int count);
};

typedef struct BigNumPool {
    int used;
    BigNum numbers[POOL_SIZE];
    u8 unkF4[0x14];
    u32 flags;
    CryptProgress *progress;
} BigNumPool;

// Divides by a number whose reciprocal is kept, to reduce a product with a few subtractions: the modulus, its reciprocal,
// the modulus' length in bits and the length in bits that the reciprocal is for
typedef struct Reducer {
    BigNum modulus;
    BigNum reciprocal;
    int modulusBits;
    int reciprocalBits;
    u32 flags;
} Reducer;

// The numbers of the Montgomery form: the modulus, R squared and the inverse that the reduction needs
typedef struct Montgomery {
    int unk0;
    int unk4;
    int words;
    BigNum rSquared;
    BigNum modulus;
    BigNum unk34;
    u32 inverse;
    u32 flags;
} Montgomery;

static void *func_ov114_021caab4(u32 size);
static void func_ov114_021caae8(void *ptr);
static int func_ov114_021cab34(u32 x);
static int func_ov114_021cab94(BigNum *n);
static void func_ov114_021cabdc(BigNum *n);
static void func_ov114_021cabc0(BigNum *n);
static BigNum *func_ov114_021cac24(BigNum *n, int words);
static void func_ov114_021cac8c(BigNum *n);
static void func_ov114_021cacd0(BigNum *n);
static BigNum *func_ov114_021cace4(void);
static BigNumPool *func_ov114_021cad1c(void);
static void func_ov114_021cad48(BigNumPool *pool);
static void func_ov114_021cad6c(BigNumPool *pool);
static BigNum *func_ov114_021cadac(BigNum *n, int words);
static BigNum *func_ov114_021cae20(BigNum *dest, BigNum *src);
static void func_ov114_021cae94(BigNum *n);
static BOOL func_ov114_021caec8(BigNum *n, u32 word);
static BigNum *func_ov114_021caf24(const u8 *bytes, int size, BigNum *out);
static int func_ov114_021cb010(BigNum *n, u8 *out);
static int func_ov114_021cb078(BigNum *a, BigNum *b);
static BOOL func_ov114_021cb0c8(BigNum *n, int bit);
static BOOL func_ov114_021cb15c(BigNum *n, int bit);
static BOOL func_ov114_021cb1a8(BigNum *dest, BigNum *src, int bits);
static BOOL func_ov114_021cb2a4(BigNum *dest, BigNum *src, int bits);
static BOOL func_ov114_021cb3b4(BigNum *n, u32 word);
static BOOL func_ov114_021cb494(BigNum *n, u32 word);
static BOOL func_ov114_021cb5a0(BigNum *dest, BigNum *a, BigNum *b);
static BOOL func_ov114_021cb664(BigNum *dest, BigNum *a, BigNum *b);
static BOOL func_ov114_021cb764(BigNum *dest, BigNum *a, BigNum *b);
static BOOL func_ov114_021cb8b0(BigNum *dest, BigNum *a, BigNum *b);
static BOOL func_ov114_021cb9bc(BigNum *dest, BigNum *a, BigNum *b, BigNumPool *pool);
static BOOL func_ov114_021cbb80(BigNum *quotient, BigNum *remainder, BigNum *numerator, BigNum *denominator,
                                BigNumPool *pool);
static BOOL func_ov114_021cc0fc(BigNum *remainder, BigNum *numerator, BigNum *denominator, BigNumPool *pool);
static BOOL func_ov114_021cc124(BigNum *dest, BigNum *base, BigNum *exponent, BigNum *modulus, BigNumPool *pool);
static BOOL func_ov114_021cc178(BigNum *dest, BigNum *a, BigNumPool *pool);
static void func_ov114_021cc2b0(u32 *dest, const u32 *a, int size, u32 *scratch);
static void func_ov114_021cc38c(u32 *dest, u32 *product, const u32 *modulus, int size, u32 inverse);
static u32 func_ov114_021cc4a0(u32 x);
static int func_ov114_021cc530(BigNum *exponent, const u8 **encoded, int unused, BigNumPool *pool);
static int func_ov114_021cc674(u8 *dest, BigNum *exponent, int window);
static BOOL func_ov114_021cc80c(Montgomery *mont, BigNum *modulus, BigNumPool *pool);
static Montgomery *func_ov114_021cc9c4(void);
static void func_ov114_021cc9f0(Montgomery *mont);
static void func_ov114_021cca24(Montgomery *mont);
static void func_ov114_021cca5c(Reducer *reducer);
static void func_ov114_021cca80(Reducer *reducer);
static int func_ov114_021ccaac(Reducer *reducer, BigNum *modulus, BigNumPool *pool);
static BOOL func_ov114_021ccae4(BigNum *dest, BigNum *a, BigNum *b, Reducer *reducer, BigNumPool *pool);
static BOOL func_ov114_021ccb88(BigNum *quotient, BigNum *remainder, BigNum *numerator, Reducer *reducer, BigNumPool *pool);
static int func_ov114_021ccdfc(BigNum *reciprocal, BigNum *modulus, int bits, BigNumPool *pool);
static BOOL func_ov114_021cce7c(BigNum *dest, BigNum *base, BigNum *exponent, BigNum *modulus,
                        BigNumPool *pool);
static BOOL func_ov114_021cd210(BigNum *dest, BigNum *base, BigNum *exponent, BigNum *modulus,
                       BigNumPool *pool, Montgomery *mont);
static u32 func_ov114_021cd984(u32 *dest, const u32 *a, int aSize, u32 b);
static u32 func_ov114_021cda38(u32 *dest, const u32 *a, int aSize, u32 b);
#ifndef __MWERKS__
static void func_ov114_021cdac0(u32 *dest, const u32 *a, int size);
static u32 func_ov114_021cdae8(u32 high, u32 low, u32 divisor);
static u32 func_ov114_021cdb08(u32 *dest, const u32 *a, const u32 *b, int size);
static u32 func_ov114_021cdb9c(u32 *dest, const u32 *a, const u32 *b, int size);
#else
static asm void func_ov114_021cdac0(u32 *dest, const u32 *a, int size);
static asm u32 func_ov114_021cdae8(u32 high, u32 low, u32 divisor);
static asm u32 func_ov114_021cdb08(u32 *dest, const u32 *a, const u32 *b, int size);
static asm u32 func_ov114_021cdb9c(u32 *dest, const u32 *a, const u32 *b, int size);
#endif
static BOOL func_ov114_021ca5e4(const u8 **data, u32 *size, int tag, u32 *length);
static BOOL func_ov114_021ca69c(const u8 *block, u32 size, const u8 **out, u32 *outSize);
static BOOL func_ov114_021ca734(const u8 *data, u32 size, const u8 **oid, u32 *oidSize, const u8 **digest,
                            u32 *digestSize);
static int func_ov114_021ca494(u8 *out, int outSize, const u8 *input, int inputSize, const u8 *modulus, int modulusSize,
                     u32 exponent);
static int func_ov114_021ca8ec(u8 *out, int outSize, const u8 *signature, int signatureSize, const u8 *modulus,
                         int modulusSize, u32 exponent);
static BOOL func_ov114_021ca9f8(const u8 *digest, const u8 *signature, const u8 *modulus);

// The allocator that the game gave: its function to free, the one to allocate, and an argument that is not used
static struct {
    CryptFreeFunc free;
    CryptAllocFunc alloc;
    u32 arg;
// pokebw2's sAllocator
} data_ov114_021d422c;

// The number of trailing zero bits of each value of a window, for windows of up to 6 bits: a window of w bits reads the
// last 2^w of them
// pokebw2's sTrailingZeros
static const u8 data_ov114_021ce228[64] = {
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
};
static const u8 *data_ov114_021cf930[7] = {
    data_ov114_021ce228 + 63, data_ov114_021ce228 + 62, data_ov114_021ce228 + 60, data_ov114_021ce228 + 56,
    data_ov114_021ce228 + 48, data_ov114_021ce228 + 32, data_ov114_021ce228,
};

// The bit length of each value of a nibble
static const s8 data_ov114_021ce202[16] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};

// The encodings of the exponents 3, 17 and 65537, which the key uses: a length, the window and the table's size, and
// the pairs of a window's value and the zeros after it
const u32 data_ov114_021ce1e0 = 1;
const BigNum data_ov114_021ce214 = {(u32 *)&data_ov114_021ce1e0, 1, 1, 0, 0};
static const u8 data_ov114_021ce1f8[10] = {0, 8, 1, 1, 1, 16, 1, 0, 0, 0};
static const u8 data_ov114_021ce1e4[10] = {0, 8, 1, 1, 1, 1, 1, 0, 0, 0};
static const u8 data_ov114_021ce1ee[10] = {0, 8, 1, 1, 1, 4, 1, 0, 0, 0};

// Raises the input to the power of the exponent modulo the modulus, all as bytes with the most significant first, and
// returns the length of the output, or a negative number: -1 when it does not fit, -2 when it failed and -3 for a
// missing argument
static int func_ov114_021ca494(u8 *out, int outSize, const u8 *input, int inputSize, const u8 *modulus, int modulusSize,
                     u32 exponent) {
    BigNum inputNum;
    BigNum resultNum;
    BigNum exponentNum;
    BigNum modulusNum;
    BigNumPool *pool;
    int result;

    if (out == NULL || input == NULL || modulus == NULL) {
        return -3;
    }
    pool = func_ov114_021cad1c();
    func_ov114_021cacd0(&inputNum);
    func_ov114_021cacd0(&resultNum);
    func_ov114_021cacd0(&exponentNum);
    func_ov114_021cacd0(&modulusNum);
    if (pool == NULL) {
        result = -2;
    } else if (func_ov114_021caf24(input, inputSize, &inputNum) == NULL) {
        result = -2;
    } else if (!func_ov114_021caec8(&exponentNum, exponent)) {
        result = -2;
    } else if (func_ov114_021caf24(modulus, modulusSize, &modulusNum) == NULL) {
        result = -2;
    } else if (!func_ov114_021cc124(&resultNum, &inputNum, &exponentNum, &modulusNum, pool)) {
        result = -2;
    } else if (outSize < (func_ov114_021cab94(&resultNum) + 7) / 8) {
        result = -1;
    } else {
        result = func_ov114_021cb010(&resultNum, out);
    }
    func_ov114_021cabdc(&inputNum);
    func_ov114_021cabdc(&resultNum);
    func_ov114_021cabdc(&exponentNum);
    func_ov114_021cabdc(&modulusNum);
    if (pool != NULL) {
        func_ov114_021cad6c(pool);
    }
    return result;
}

// Reads the tag and the length of a piece of ASN.1, leaving the data at its content and the size at what is left
// pokebw2's ReadAsn1
static BOOL func_ov114_021ca5e4(const u8 **data, u32 *size, int tag, u32 *length) {
    const u8 *p = *data;
    u32 left = *size;
    u32 value = 0;
    u8 count;
    u32 b;

    if (*p++ != tag) {
        return FALSE;
    }
    if (left < 1) {
        return FALSE;
    }
    b = *p;
    left--;
    if ((b & 0x80) != 0) {
        count = b & 0x7f;
        if (left < count + 1) {
            return FALSE;
        }
        left -= count;
        if (length != NULL) {
            u32 digit = *++p & 0x7f;

            do {
                value = digit + (value << 7);
            } while (--count != 0);
        } else {
            p += count;
        }
    } else {
        p++;
        value = b;
        if (left < 1) {
            return FALSE;
        }
        left--;
    }
    *data = p;
    *size = left;
    if (length != NULL) {
        *length = value;
    }
    return TRUE;
}

// Takes the padding off a block that a signature decrypts to: a 1, at least 8 bytes of 255 and a 0
// pokebw2's ParsePadding
static BOOL func_ov114_021ca69c(const u8 *block, u32 size, const u8 **out, u32 *outSize) {
    const u8 *p;
    const u8 *end;
    int count;

    if (size < 10) {
        return FALSE;
    }
    end = block + size;
    if (block[0] != 1) {
        return FALSE;
    }
    p = block + 1;
    count = 0;
    do {
        if (*p++ != 0xff) {
            return FALSE;
        }
        count++;
    } while (count < 8);
    while (p != end) {
        if (*p != 0xff) {
            break;
        }
        p++;
    }
    if (p == end) {
        return FALSE;
    }
    if (*p != 0) {
        return FALSE;
    }
    *outSize = end - (p + 1);
    *out = p + 1;
    return TRUE;
}

// Reads a digest out of its ASN.1: a sequence of the algorithm (an identifier and a null) and the digest
static BOOL func_ov114_021ca734(const u8 *data, u32 size, const u8 **oid, u32 *oidSize, const u8 **digest,
                            u32 *digestSize) {
    u32 length;

    if (!func_ov114_021ca5e4(&data, &size, 0x30, NULL)) {
        return FALSE;
    }
    if (!func_ov114_021ca5e4(&data, &size, 0x30, NULL)) {
        return FALSE;
    }
    if (!func_ov114_021ca5e4(&data, &size, 6, &length)) {
        return FALSE;
    }
    if (oid != NULL) {
        *oid = data;
    }
    if (oidSize != NULL) {
        *oidSize = length;
    }
    data += length;
    if (size < length) {
        return FALSE;
    }
    size -= length;
    if (!func_ov114_021ca5e4(&data, &size, 5, &length)) {
        return FALSE;
    }
    data += length;
    if (size < length) {
        return FALSE;
    }
    size -= length;
    if (!func_ov114_021ca5e4(&data, &size, 4, &length)) {
        return FALSE;
    }
    if (size != length) {
        return FALSE;
    }
    if (digest != NULL) {
        *digest = data;
    }
    if (digestSize != NULL) {
        *digestSize = length;
    }
    return TRUE;
}

// Decrypts a signature with the public key and takes the digest out of what it holds. The length of the digest, or -1
static int func_ov114_021ca8ec(u8 *out, int outSize, const u8 *signature, int signatureSize, const u8 *modulus,
                         int modulusSize, u32 exponent) {
    u8 block[0x100];
    const u8 *unpadded;
    u32 unpaddedSize = 0;
    const u8 *digest;
    u32 digestSize = 0;
    int size;
    u32 i;

    size = func_ov114_021ca494(block, sizeof(block), signature, signatureSize, modulus, modulusSize, exponent);
    if (size < 0) {
        return -1;
    }
    if (!func_ov114_021ca69c(block, size, &unpadded, &unpaddedSize)) {
        return -1;
    }
    if (!func_ov114_021ca734(unpadded, unpaddedSize, NULL, NULL, &digest, &digestSize)) {
        return -1;
    }
    if (digestSize > outSize) {
        return -1;
    }
    for (i = 0; i < digestSize; i++) {
        *out++ = digest[i];
    }
    for (; i < outSize; i++) {
        *out++ = 0;
    }
    return digestSize;
}

// Whether the signature, of 128 bytes, holds the digest: the key's modulus is 128 bytes and its exponent is 65537
// pokebw2's VerifyDigest
static BOOL func_ov114_021ca9f8(const u8 *digest, const u8 *signature, const u8 *modulus) {
    u8 extracted[0x14];
    int i;

    if (func_ov114_021ca8ec(extracted, 0x14, signature, 0x80, modulus, 0x80, 0x10001) != 0x14) {
        return FALSE;
    }
    for (i = 0; i < 0x14; i++) {
        if (extracted[i] != digest[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's Crypt_VerifySignature
int func_ov114_021caa70(const void *data, u32 size, void *signature, const u8 *key) {
    u8 digest[0x14];

    MATH_CalcSHA1(digest, data, size);
    return func_ov114_021ca9f8(digest, signature, key);
}

// pokebw2's CryptAlloc
static void *func_ov114_021caab4(u32 size) {
    if (data_ov114_021d422c.alloc != NULL) {
        return data_ov114_021d422c.alloc(size);
    }
    return OS_AllocFromHeap(0, -1, size);
}

// pokebw2's CryptFree
static void func_ov114_021caae8(void *ptr) {
    if (data_ov114_021d422c.free != NULL) {
        data_ov114_021d422c.free(ptr);
        return;
    }
    OS_FreeToHeap(0, -1, ptr);
}

// pokebw2's Crypt_SetAllocator
void func_ov114_021cab1c(CryptAllocFunc alloc, CryptFreeFunc free, u32 arg) {
    data_ov114_021d422c.alloc = alloc;
    data_ov114_021d422c.free = free;
    data_ov114_021d422c.arg = arg;
}

// The length in bits of a word
// pokebw2's BitLength32
static int func_ov114_021cab34(u32 x) {
    int shift;
    u32 rest;

    if ((x & 0xffff0000) != 0) {
        shift = (x & 0xff000000) != 0 ? 24 : 16;
    } else {
        shift = (x & 0xff00) != 0 ? 8 : 0;
    }
    rest = x >> shift;
    if ((rest & 0xf0) != 0) {
        return data_ov114_021ce202[rest >> 4] + shift + 4;
    }
    return data_ov114_021ce202[rest] + shift;
}

// pokebw2's BitLength
static int func_ov114_021cab94(BigNum *n) {
    int top;

    if (n->length == 0) {
        return 0;
    }
    top = n->length - 1;
    return func_ov114_021cab34(n->digits[top]) + top * 32;
}

// pokebw2's FreeNum
static void func_ov114_021cabc0(BigNum *n) {
    if (n == NULL) {
        return;
    }
    func_ov114_021cae94(n);
    func_ov114_021cabdc(n);
}

// pokebw2's CryptDestroy
static void func_ov114_021cabdc(BigNum *n) {
    if (n == NULL) {
        return;
    }
    if (n->digits != NULL && (n->flags & BIGNUM_STATIC) == 0) {
        func_ov114_021caae8(n->digits);
    }
    n->flags |= BIGNUM_FREED;
    if ((n->flags & BIGNUM_HEAP) != 0) {
        func_ov114_021caae8(n);
    }
}

// Makes room for words if the number has less, as func_ov114_021cadac does, without the call when it has enough
static inline BigNum *Reserve(BigNum *n, int words) {
    return words > n->capacity ? func_ov114_021cadac(n, words) : n;
}

// Makes room for a number of bits, with a word to spare when it lacks room
static inline BigNum *ReserveBits(BigNum *n, int bits) {
    return (bits + 31) / 32 <= n->capacity ? n : func_ov114_021cadac(n, bits / 32 + 1);
}

// Makes room for words, with zeros above the length
// pokebw2's CryptResize
static BigNum *func_ov114_021cac24(BigNum *n, int words) {
    int i;

    if (n->length < words) {
        if (Reserve(n, words) == NULL) {
            return NULL;
        }
        if (n->digits != NULL) {
            for (i = n->length; i < words; i++) {
                n->digits[i] = 0;
            }
        }
    }
    return n;
}

// Drops the zero words on top
// pokebw2's CryptNormalize
static void func_ov114_021cac8c(BigNum *n) {
    u32 *top;

    if (n->length > 0) {
        top = n->digits + (n->length - 1);
        while (n->length > 0) {
            if (*top-- != 0) {
                return;
            }
            n->length--;
        }
    }
}

// pokebw2's InitNum
static void func_ov114_021cacd0(BigNum *n) {
    MI_CpuFill8(n, 0, sizeof(BigNum));
}

// pokebw2's NewNum
static BigNum *func_ov114_021cace4(void) {
    BigNum *n = func_ov114_021caab4(sizeof(BigNum));

    if (n == NULL) {
        return NULL;
    }
    n->flags = BIGNUM_HEAP;
    n->length = 0;
    n->negative = 0;
    n->capacity = 0;
    n->digits = NULL;
    return n;
}

// pokebw2's NewPool
static BigNumPool *func_ov114_021cad1c(void) {
    BigNumPool *pool = func_ov114_021caab4(sizeof(BigNumPool));

    if (pool == NULL) {
        return NULL;
    }
    func_ov114_021cad48(pool);
    pool->flags = POOL_HEAP;
    return pool;
}

// pokebw2's InitPool
static void func_ov114_021cad48(BigNumPool *pool) {
    MI_CpuFill8(pool, 0, sizeof(BigNumPool));
    pool->used = 0;
    pool->flags = 0;
}

// pokebw2's FreePool
static void func_ov114_021cad6c(BigNumPool *pool) {
    int i;

    for (i = 0; i < POOL_SIZE; i++) {
        func_ov114_021cabc0(&pool->numbers[i]);
    }
    if ((pool->flags & POOL_HEAP) != 0) {
        func_ov114_021caae8(pool);
    }
}

// Makes room for words, keeping the words it has
// pokebw2's CryptExpand
static BigNum *func_ov114_021cadac(BigNum *n, int words) {
    u32 *digits;

    if (words > n->capacity) {
        if ((n->flags & BIGNUM_STATIC) != 0) {
            return NULL;
        }
        digits = func_ov114_021caab4((words + 1) * sizeof(u32));
        if (digits == NULL) {
            return NULL;
        }
        if (n->digits != NULL) {
            MI_CpuCopy8(n->digits, digits, n->length * sizeof(u32));
            func_ov114_021caae8(n->digits);
        }
        n->digits = digits;
        n->capacity = words;
    }
    return n;
}

// pokebw2's CryptCopy
static BigNum *func_ov114_021cae20(BigNum *dest, BigNum *src) {
    if (dest == src) {
        return dest;
    }
    if (Reserve(dest, src->length) == NULL) {
        return NULL;
    }
    MI_CpuCopy8(src->digits, dest->digits, src->length * sizeof(u32));
    dest->length = src->length;
    if (dest->length == 0 && dest->digits != NULL) {
        dest->digits[0] = 0;
    }
    dest->negative = src->negative;
    return dest;
}

// pokebw2's CryptClear
static void func_ov114_021cae94(BigNum *n) {
    if (n->digits != NULL) {
        MI_CpuFill8(n->digits, 0, n->capacity * sizeof(u32));
    }
    n->length = 0;
    n->negative = 0;
}

// pokebw2's CryptSetWord
static BOOL func_ov114_021caec8(BigNum *n, u32 word) {
    if (ReserveBits(n, 32) == NULL) {
        return FALSE;
    }
    n->negative = 0;
    n->length = 0;
    n->digits[0] = word;
    if (n->digits[0] != 0) {
        n->length = 1;
    }
    return TRUE;
}

// Reads size bytes, the most significant first, into out or into a new number
// pokebw2's FromBytes
static BigNum *func_ov114_021caf24(const u8 *bytes, int size, BigNum *out) {
    u32 word;
    u32 n;
    u32 words;
    u32 phase;

    if (out == NULL) {
        out = func_ov114_021cace4();
    }
    if (out == NULL) {
        return NULL;
    }
    word = 0;
    n = size;
    if (n == 0) {
        out->length = 0;
        return out;
    }
    if (ReserveBits(out, (int)(n + 2) * 8) == NULL) {
        return NULL;
    }
    words = (n - 1) / 4 + 1;
    phase = (n - 1) % 4;
    out->length = words;
    while (n-- > 0) {
        word = (word << 8) | *bytes++;
        if (phase-- == 0) {
            out->digits[--words] = word;
            word = 0;
            phase = 3;
        }
    }
    func_ov114_021cac8c(out);
    return out;
}

// Writes the number as bytes, the most significant first, and returns how many
// pokebw2's ToBytes
static int func_ov114_021cb010(BigNum *n, u8 *out) {
    int bytes = (func_ov114_021cab94(n) + 7) / 8;
    int i = bytes;

    while (i-- > 0) {
        *out++ = n->digits[i / 4] >> (i % 4 * 8);
    }
    return bytes;
}

// Compares the sizes, which is the difference of their lengths, or the most significant word that differs
// pokebw2's CryptCompare
static int func_ov114_021cb078(BigNum *a, BigNum *b) {
    int diff = a->length - b->length;
    const u32 *ap;
    const u32 *bp;
    int i;

    if (diff != 0) {
        return diff;
    }
    ap = a->digits;
    bp = b->digits;
    for (i = a->length - 1; i >= 0; i--) {
        if (ap[i] != bp[i]) {
            return ap[i] > bp[i] ? 1 : -1;
        }
    }
    return 0;
}

// pokebw2's CryptSetBit
static BOOL func_ov114_021cb0c8(BigNum *n, int bit) {
    int word = bit / 32;
    int shift = bit % 32;
    int i;

    if (n->length <= word) {
        if (Reserve(n, word + 1) == NULL) {
            return FALSE;
        }
        for (i = n->length; i < word + 1; i++) {
            n->digits[i] = 0;
        }
        n->length = word + 1;
    }
    n->digits[word] |= 1 << shift;
    return TRUE;
}

// pokebw2's CryptTestBit
static BOOL func_ov114_021cb15c(BigNum *n, int bit) {
    int word;
    int shift;

    if (bit < 0) {
        return FALSE;
    }
    word = bit / 32;
    shift = bit % 32;
    if (n->length <= word) {
        return FALSE;
    }
    return (n->digits[word] & (1 << shift)) != 0;
}

// pokebw2's ShiftLeft
static BOOL func_ov114_021cb1a8(BigNum *dest, BigNum *src, int bits) {
    int words = bits / 32;
    int shift;
    int back;
    int i;
    const u32 *in;
    u32 *out;
    u32 word;

    if (Reserve(dest, src->length + words + 1) == NULL) {
        return FALSE;
    }
    dest->negative = src->negative;
    shift = bits % 32;
    back = 32 - shift;
    in = src->digits;
    out = dest->digits;
    out[src->length + words] = 0;
    if (shift == 0) {
        for (i = src->length - 1; i >= 0; i--) {
            out[words + i] = in[i];
        }
    } else {
        for (i = src->length - 1; i >= 0; i--) {
            word = in[i];
            out[words + i + 1] |= word >> back;
            out[words + i] = word << shift;
        }
    }
    MI_CpuFill8(out, 0, words * sizeof(u32));
    dest->length = src->length + words + 1;
    func_ov114_021cac8c(dest);
    return TRUE;
}

// pokebw2's ShiftRight
static BOOL func_ov114_021cb2a4(BigNum *dest, BigNum *src, int bits) {
    int words;
    int back;
    int shift;
    int count;
    int i;
    u32 *in;
    u32 *out;
    u32 word;
    u32 low;

    words = bits / 32;
    shift = bits % 32;
    back = 32 - shift;
    if (src->length < words) {
        func_ov114_021caec8(dest, 0);
        return TRUE;
    }
    if (dest != src) {
        if (Reserve(dest, src->length - words + 2) == NULL) {
            return FALSE;
        }
        dest->negative = src->negative;
    }
    in = src->digits + words;
    count = src->length - words;
    dest->length = count;
    out = dest->digits;
    if (shift == 0) {
        for (i = count + 1; i > 0; i--) {
            *out++ = *in++;
        }
    } else {
        word = *in++;
        for (i = 1; i < count; i++) {
            low = word >> shift;
            word = *in++;
            *out++ = low | (word << back);
        }
        *out++ = word >> shift;
        *out = 0;
    }
    func_ov114_021cac8c(dest);
    return TRUE;
}

// Adds a word to the number, whatever its sign
// pokebw2's AddWord
static BOOL func_ov114_021cb3b4(BigNum *n, u32 word) {
    int i;
    u32 *digits;
    u32 low;

    if (word == 0) {
        return TRUE;
    }
    if (n->negative) {
        if (n->length > 1) {
            n->negative = 0;
            i = func_ov114_021cb494(n, word);
            n->negative = 1;
            return i;
        }
        digits = n->digits;
        low = digits[0];
        if (low > word) {
            digits[0] = low - word;
        } else if (low < word) {
            n->negative = 0;
            digits[0] = word - low;
        } else {
            n->negative = 0;
            n->length = 0;
        }
        return TRUE;
    }
    if (Reserve(n, n->length + 1) == NULL) {
        return FALSE;
    }
    n->digits[n->length] = 0;
    for (i = 0;;) {
        u32 sum = word + n->digits[i];

        n->digits[i] = sum;
        if (word > sum) {
            word = 1;
            i++;
        } else {
            break;
        }
    }
    if (i >= n->length) {
        n->length++;
    }
    return TRUE;
}

// pokebw2's SubWord
static BOOL func_ov114_021cb494(BigNum *n, u32 word) {
    int i;
    u32 *digits;
    u32 low;

    if (word == 0) {
        return TRUE;
    }
    if (n->negative) {
        n->negative = 0;
        i = func_ov114_021cb3b4(n, word);
        n->negative = 1;
        return i;
    }
    if (n->length <= 1) {
        if (n->length == 0) {
            if (Reserve(n, 1) == NULL) {
                return FALSE;
            }
            n->digits[0] = word;
            n->negative = 1;
            n->length = 1;
            return TRUE;
        }
        digits = n->digits;
        low = digits[0];
        if (low == word) {
            n->length = 0;
        } else if (low > word) {
            digits[0] = low - word;
        } else {
            n->negative = 1;
            digits[0] = word - low;
        }
        return TRUE;
    }
    for (i = 0;;) {
        u32 digit = n->digits[i];

        if (digit >= word) {
            n->digits[i] = digit - word;
            break;
        }
        n->digits[i] = digit - word;
        word = 1;
        i++;
    }
    if (n->digits[i] == 0 && i == n->length - 1) {
        n->length--;
    }
    return TRUE;
}

// pokebw2's AddSigned
static BOOL func_ov114_021cb5a0(BigNum *dest, BigNum *a, BigNum *b) {
    BigNum *swap;

    if (a->negative ^ b->negative) {
        if (a->negative != 0) {
            swap = a;
            a = b;
            b = swap;
        }
        if (func_ov114_021cb078(a, b) < 0) {
            if (!func_ov114_021cb764(dest, b, a)) {
                return FALSE;
            }
            dest->negative = 1;
        } else {
            if (!func_ov114_021cb764(dest, a, b)) {
                return FALSE;
            }
            dest->negative = 0;
        }
        return TRUE;
    }
    if (a->negative != 0) {
        dest->negative = 1;
    } else {
        dest->negative = 0;
    }
    if (!func_ov114_021cb664(dest, a, b)) {
        return FALSE;
    }
    return TRUE;
}

// Adds the sizes of two numbers
// pokebw2's AddMagnitude
static BOOL func_ov114_021cb664(BigNum *dest, BigNum *a, BigNum *b) {
    BigNum *swap;
    int max;
    int i;
    u32 carry;
    u32 digit;
    u32 *aDigits;
    u32 *destDigits;

    if (a->length < b->length) {
        swap = a;
        a = b;
        b = swap;
    }
    max = a->length;
    i = b->length;
    if (Reserve(dest, max + 1) == NULL) {
        return FALSE;
    }
    dest->length = max;
    aDigits = a->digits;
    destDigits = dest->digits;
    carry = func_ov114_021cdb08(destDigits, aDigits, b->digits, i);
    destDigits += i;
    aDigits += i;
    if (carry != 0) {
        while (i < max) {
            digit = *aDigits++;
            i++;
            if ((*destDigits++ = digit + 1) >= digit) {
                carry = 0;
                break;
            }
        }
        if (i >= max && carry != 0) {
            *destDigits++ = 1;
            dest->length++;
        }
    }
    if (destDigits != aDigits) {
        for (; i < max; i++) {
            *destDigits++ = *aDigits++;
        }
    }
    return TRUE;
}

// Subtracts a smaller size from a bigger one
// pokebw2's SubMagnitude
static BOOL func_ov114_021cb764(BigNum *dest, BigNum *a, BigNum *b) {
    int max;
    int min;
    int i;
    int borrow;
    u32 x;
    u32 y;
    u32 *aDigits;
    u32 *bDigits;
    u32 *destDigits;

    if (a->length < b->length) {
        return FALSE;
    }
    max = a->length;
    min = b->length;
    if (Reserve(dest, max) == NULL) {
        return FALSE;
    }
    aDigits = a->digits;
    bDigits = b->digits;
    destDigits = dest->digits;
    borrow = 0;
    for (i = 0; i < min; i++) {
        x = *aDigits++;
        y = *bDigits++;
        if (borrow) {
            borrow = x <= y;
            x = x - y - 1;
        } else {
            borrow = x < y;
            x = x - y;
        }
        *destDigits++ = x;
    }
    if (borrow) {
        while (i < max) {
            i++;
            x = *aDigits++;
            y = x - 1;
            *destDigits++ = y;
            if (x > y) {
                break;
            }
        }
    }
    if (destDigits != aDigits) {
        for (;;) {
            if (i++ >= max) {
                break;
            }
            destDigits[0] = aDigits[0];
            if (i++ >= max) {
                break;
            }
            destDigits[1] = aDigits[1];
            if (i++ >= max) {
                break;
            }
            destDigits[2] = aDigits[2];
            if (i++ >= max) {
                break;
            }
            destDigits[3] = aDigits[3];
            destDigits += 4;
            aDigits += 4;
        }
    }
    dest->length = max;
    func_ov114_021cac8c(dest);
    return TRUE;
}

// pokebw2's SubSigned
static BOOL func_ov114_021cb8b0(BigNum *dest, BigNum *a, BigNum *b) {
    BigNum *swap;
    BOOL add = FALSE;
    int negative = 0;
    int size;

    if (a->negative != 0) {
        if (b->negative != 0) {
            swap = a;
            a = b;
            b = swap;
        } else {
            add = TRUE;
            negative = 1;
        }
    } else if (b->negative != 0) {
        add = TRUE;
    }
    if (add) {
        if (!func_ov114_021cb664(dest, a, b)) {
            return FALSE;
        }
        dest->negative = negative;
        return TRUE;
    }
    size = a->length > b->length ? a->length : b->length;
    if (Reserve(dest, size) == NULL) {
        return FALSE;
    }
    if (func_ov114_021cb078(a, b) < 0) {
        if (!func_ov114_021cb764(dest, b, a)) {
            return FALSE;
        }
        dest->negative = 1;
    } else {
        if (!func_ov114_021cb764(dest, a, b)) {
            return FALSE;
        }
        dest->negative = 0;
    }
    return TRUE;
}

void func_ov114_021cba90(u32 *dest, const u32 *a, int aSize, const u32 *b, int bSize);

// pokebw2's CryptMul
static BOOL func_ov114_021cb9bc(BigNum *dest, BigNum *a, BigNum *b, BigNumPool *pool) {
    int total;
    int aSize = a->length;
    int bSize = b->length;
    int negative;
    BigNum *work;

    if (aSize == 0 || bSize == 0) {
        func_ov114_021caec8(dest, 0);
        return TRUE;
    }
    total = aSize + bSize;
    negative = a->negative ^ b->negative;
    if (dest == a || dest == b) {
        work = &pool->numbers[pool->used + 1];
    } else {
        work = dest;
    }
    if (Reserve(work, total) == NULL) {
        return FALSE;
    }
    work->length = total;
    func_ov114_021cba90(work->digits, a->digits, aSize, b->digits, bSize);
    dest->negative = negative;
    func_ov114_021cac8c(work);
    if (dest != work) {
        func_ov114_021cae20(dest, work);
    }
    return TRUE;
}

void func_ov114_021cba90(u32 *dest, const u32 *a, int aSize, const u32 *b, int bSize) {
    u32 *top;

    if (aSize < bSize) {
        int swapSize;
        const u32 *swap;

        swapSize = aSize;
        aSize = bSize;
        bSize = swapSize;
        swap = a;
        a = b;
        b = swap;
    }
    top = &dest[aSize];
    top[0] = func_ov114_021cda38(dest, a, aSize, b[0]);
    for (;;) {
        if (--bSize <= 0) {
            return;
        }
        top[1] = func_ov114_021cd984(&dest[1], a, aSize, b[1]);
        if (--bSize <= 0) {
            return;
        }
        top[2] = func_ov114_021cd984(&dest[2], a, aSize, b[2]);
        if (--bSize <= 0) {
            return;
        }
        top[3] = func_ov114_021cd984(&dest[3], a, aSize, b[3]);
        if (--bSize <= 0) {
            return;
        }
        top[4] = func_ov114_021cd984(&dest[4], a, aSize, b[4]);
        top += 4;
        dest += 4;
        b += 4;
    }
}


// Multiplies the words of two numbers the way it is taught: one row of the shorter number for each of the longer
// OpenSSL's macros of bn_lcl.h, which the library is built from (pokeplatinum's lib/crypto/src/bignum.h, the same
// library, has them): the 16-bit halves of a word, a half moved up to the high half, and mul64, which multiplies the
// word whose halves are in l and h by the one whose halves are bl and bh from 16-bit products, and leaves the low and
// the high word of the product in l and h
#define LBITS(a) ((a) & 0xffff)
#define HBITS(a) (((a) >> 16) & 0xffff)
#define L2HBITS(a) ((u32)((a) & 0xffff) << 16)

#define mul64(l, h, bl, bh)                \
    {                                      \
        u32 m, m1, lt, ht;                 \
                                           \
        lt = l;                            \
        ht = h;                            \
        m = (bh) * (lt);                   \
        lt = (bl) * (lt);                  \
        m1 = (bl) * (ht);                  \
        ht = (bh) * (ht);                  \
        m = (m + m1) & 0xffffffff;         \
        if (m < m1)                        \
            ht += L2HBITS(1L);             \
        ht += HBITS(m);                    \
        m1 = L2HBITS(m);                   \
        lt = (lt + m1) & 0xffffffff;       \
        if (lt < m1)                       \
            ht++;                          \
        (l) = lt;                          \
        (h) = ht;                          \
    }

// Divides, schoolbook: the denominator is shifted up until its top bit is set, then each word of the quotient is
// estimated from the top words of what is left, and the estimate is corrected
// pokebw2's BigDiv
static BOOL func_ov114_021cbb80(BigNum *quotient, BigNum *remainder, BigNum *numerator, BigNum *denominator,
                                BigNumPool *pool) {
    int shift, i, j, loop;
    BigNum *product, window, *shiftedNumerator, *shiftedDenominator, *result;
    u32 *resultDigit, *numeratorTop;
    u32 top, next;
    int denominatorSize;
    int numeratorSize;

    if (denominator->length == 0 || (denominator->length == 1 && denominator->digits[0] == 0)) {
        return FALSE;
    }
    if (func_ov114_021cb078(numerator, denominator) < 0) {
        if (remainder != NULL) {
            if (func_ov114_021cae20(remainder, numerator) == NULL) {
                return FALSE;
            }
        }
        if (quotient != NULL) {
            func_ov114_021caec8(quotient, 0);
        }
        return TRUE;
    }

    product = &pool->numbers[pool->used];
    product->negative = 0;
    shiftedNumerator = &pool->numbers[pool->used + 1];
    shiftedDenominator = &pool->numbers[pool->used + 2];
    if (quotient == NULL) {
        result = &pool->numbers[pool->used + 3];
    } else {
        result = quotient;
    }

    shift = 32 - func_ov114_021cab94(denominator) % 32;
    if (!func_ov114_021cb1a8(shiftedDenominator, denominator, shift)) {
        return FALSE;
    }
    shiftedDenominator->negative = 0;
    shift += 32;
    if (!func_ov114_021cb1a8(shiftedNumerator, numerator, shift)) {
        return FALSE;
    }
    shiftedNumerator->negative = 0;
    denominatorSize = shiftedDenominator->length;
    numeratorSize = shiftedNumerator->length;
    loop = numeratorSize - denominatorSize;

    func_ov114_021cacd0(&window);
    window.digits = &shiftedNumerator->digits[loop];
    window.length = denominatorSize;
    window.capacity = shiftedNumerator->capacity + 1;

    top = shiftedDenominator->digits[denominatorSize - 1];
    next = denominatorSize == 1 ? 0 : shiftedDenominator->digits[denominatorSize - 2];

    numeratorTop = &shiftedNumerator->digits[numeratorSize - 1];

    if (Reserve(result, loop + 1) == NULL) {
        goto fail;
    }
    result->negative = numerator->negative ^ denominator->negative;
    result->length = loop;
    resultDigit = &result->digits[loop - 1];

    if (Reserve(product, denominatorSize + 1) == NULL) {
        goto fail;
    }

    if (func_ov114_021cb078(&window, shiftedDenominator) >= 0) {
        if (!func_ov114_021cb764(&window, &window, shiftedDenominator)) {
            goto fail;
        }
        *resultDigit = 1;
        result->digits[result->length - 1] = 1;
    } else {
        result->length--;
    }
    resultDigit--;

    for (i = 0; i < loop - 1; i++) {
        u32 q;
        u32 n0, n1;
        u32 t1l, t1h, t2l, t2h, t3l, t3h, ql, qh, t3t;

        window.digits--;
        window.length++;
        n0 = numeratorTop[0];
        n1 = numeratorTop[-1];
        if (n0 == top) {
            q = 0xffffffff;
        } else {
            q = func_ov114_021cdae8(n0, n1, top);
        }

        t1h = n0;
        t1l = n1;
        for (;;) {
            t2l = LBITS(next);
            t2h = HBITS(next);
            ql = LBITS(q);
            qh = HBITS(q);
            mul64(t2l, t2h, ql, qh);

            t3t = LBITS(top);
            t3h = HBITS(top);
            mul64(t3t, t3h, ql, qh);
            t3l = (t1l - t3t) & 0xffffffff;
            if (t3l > t1l) {
                t3h++;
            }
            t3h = (t1h - t3h) & 0xffffffff;

            if (t3h) {
                break;
            }
            if (t2h < t3l) {
                break;
            }
            if (t2h == t3l && t2l <= numeratorTop[-2]) {
                break;
            }
            q--;
        }

        product->digits[denominatorSize] = func_ov114_021cda38(product->digits, shiftedDenominator->digits,
                                                               denominatorSize, q);
        for (j = denominatorSize + 1; j > 0; j--) {
            if (product->digits[j - 1]) {
                break;
            }
        }
        product->length = j;

        j = window.length;
        func_ov114_021cb8b0(&window, &window, product);
        shiftedNumerator->length = shiftedNumerator->length + window.length - j;

        if (window.negative) {
            q--;
            j = window.length;
            func_ov114_021cb5a0(&window, &window, shiftedDenominator);
            shiftedNumerator->length += window.length - j;
        }
        *resultDigit-- = q;
        numeratorTop--;
    }
    func_ov114_021cac8c(shiftedNumerator);
    if (remainder != NULL) {
        int negative = numerator->negative;

        if (!func_ov114_021cb2a4(remainder, shiftedNumerator, shift)) {
            return FALSE;
        }
        remainder->negative = negative;
    }
    return TRUE;
fail:
    return FALSE;
}
static BOOL func_ov114_021cc0fc(BigNum *remainder, BigNum *numerator, BigNum *denominator, BigNumPool *pool) {
    return func_ov114_021cbb80(NULL, remainder, numerator, denominator, pool);
}

// Raises to a power modulo, by Montgomery's method when the modulus is odd
// pokebw2's ModExp
static BOOL func_ov114_021cc124(BigNum *dest, BigNum *base, BigNum *exponent, BigNum *modulus, BigNumPool *pool) {
    if (modulus->length > 0 && (modulus->digits[0] & 1) != 0) {
        return func_ov114_021cd210(dest, base, exponent, modulus, pool, NULL);
    }
    return func_ov114_021cce7c(dest, base, exponent, modulus, pool);
}

// pokebw2's CryptSquare
static BOOL func_ov114_021cc178(BigNum *dest, BigNum *a, BigNumPool *pool) {
    int used = pool->used;
    BigNum *work;
    int max;
    int size = a->length;
    BigNum *scratch = &pool->numbers[used];
    u32 small4[8];
    u32 small8[16];

    if (a != dest) {
        work = dest;
    } else {
        work = &pool->numbers[used + 1];
    }
    if (size <= 0) {
        dest->length = 0;
        return TRUE;
    }
    max = size * 2;
    if (Reserve(work, max) == NULL) {
        return FALSE;
    }
    work->length = max;
    work->negative = 0;
    if (size == 4) {
        func_ov114_021cc2b0(work->digits, a->digits, 4, small4);
    } else if (size == 8) {
        func_ov114_021cc2b0(work->digits, a->digits, 8, small8);
    } else {
        if (Reserve(scratch, max) == NULL) {
            return FALSE;
        }
        func_ov114_021cc2b0(work->digits, a->digits, size, scratch->digits);
    }
    if (max > 0 && work->digits[max - 1] == 0) {
        work->length--;
    }
    if (work != dest) {
        func_ov114_021cae20(dest, work);
    }
    return TRUE;
}

// Squares: the products of the pairs of different words are added up once, doubled, and the squares of the words
// are added
// pokebw2's SquareWords
static void func_ov114_021cc2b0(u32 *dest, const u32 *a, int size, u32 *scratch) {
    int i;
    int remaining;
    int max;
    const u32 *in;
    u32 *out;

    max = size * 2;
    in = a;
    out = dest;
    out[0] = out[max - 1] = 0;
    out++;
    remaining = size;
    if (--remaining > 0) {
        in++;
        out[remaining] = func_ov114_021cda38(out, in, remaining, a[0]);
        out += 2;
    }
    for (i = size - 2; i > 0; i--) {
        const u32 *current = in;

        remaining--;
        in++;
        out[remaining] = func_ov114_021cd984(out, in, remaining, *current);
        out += 2;
    }
    func_ov114_021cdb08(dest, dest, dest, max);
    func_ov114_021cdac0(scratch, a, size);
    func_ov114_021cdb08(dest, dest, scratch, max);
}

// Reduces a product of 2 * size words by the modulus the Montgomery way: size words of multiples of the modulus are
// added so that the low half is zero, and the high half, less the modulus when it is not smaller, is the result
// pokebw2's MontReduce
static void func_ov114_021cc38c(u32 *dest, u32 *product, const u32 *modulus, int size, u32 inverse) {
    u32 *high = product + size;
    int i;
    u32 carry = 0;
    u32 sum;
    u32 word;
    int j;

    for (i = 0; i < size; i++) {
        word = func_ov114_021cd984(product, modulus, size, product[0] * inverse);
        sum = word + carry;
        carry = sum < carry;
        word = *high + sum;
        *high = word;
        if (word < sum) {
            carry++;
        }
        product++;
        high++;
    }
    j = size - 1;
    if (carry == 0) {
        if (product[j] == modulus[j]) {
            while (j > 0) {
                if (product[j] != modulus[j]) {
                    break;
                }
                j--;
            }
        }
        carry = product[j] >= modulus[j];
    }
    if (carry != 0) {
        func_ov114_021cdb9c(dest, product, modulus, size);
    } else {
        for (i = 0; i < size; i++) {
            dest[i] = product[i];
        }
    }
}

// The inverse of 2^32 modulo a word, by Euclid's algorithm, or 0 when they have a common factor
// pokebw2's InverseOfWord
static u32 func_ov114_021cc4a0(u32 x) {
    u32 a;
    u32 prev;
    u32 b = -x % x;
    u32 prevPrev;
    int sign;
    u32 result;

    a = x;
    prevPrev = 0;
    prev = 1;
    sign = -1;
    while (b != 0) {
        u32 remainder = a % b;
        u32 next = a / b;

        next *= prev;
        next += prevPrev;
        prevPrev = prev;
        prev = next;
        a = b;
        b = remainder;
        sign = -sign;
    }
    if (sign < 0) {
        prevPrev = x - prevPrev;
    }
    if (a != 1) {
        result = 0;
    } else {
        result = prevPrev % x;
    }
    return result;
}

// Writes the exponent as the windows that a power is computed with, after a header: the length, the window's bits and
// the size of the table of odd powers. The exponents 3, 17 and 65537, which keys use, are kept ready
// pokebw2's EncodeExponent
static int func_ov114_021cc530(BigNum *exponent, const u8 **encoded, int unused, BigNumPool *pool) {
    u8 *bytes;
    int window;
    int entries;
    const u8 *table = NULL;
    BigNum *work = &pool->numbers[pool->used];
    int perWord;
    int size;
    int bits = exponent->length * 32;

    if (exponent->length == 0) {
        return 0;
    }
    if (exponent->length == 1) {
        if (exponent->length == 1 && exponent->digits[0] == 0x10001) {
            table = data_ov114_021ce1f8;
        } else if (exponent->digits[0] == 0x11 && exponent->length == 1) {
            table = data_ov114_021ce1ee;
        } else if (exponent->digits[0] == 3 && exponent->length == 1) {
            table = data_ov114_021ce1e4;
        }
        window = 1;
        entries = 1;
        perWord = 32;
    } else if (bits >= 256) {
        window = 5;
        entries = 16;
        perWord = 7;
    } else if (bits >= 128) {
        window = 4;
        entries = 8;
        perWord = 8;
    } else {
        window = 3;
        entries = 4;
        perWord = 11;
    }
    size = (exponent->length * perWord * 2 + 7) / 4;
    if (table == NULL) {
        if (Reserve(work, size) == NULL) {
            return 0;
        }
        bytes = (u8 *)work->digits;
        size = func_ov114_021cc674(bytes + 4, exponent, window) + 2;
        bytes[0] = size >> 8;
        bytes[1] = size;
        bytes[2] = window;
        bytes[3] = entries;
        table = bytes;
    } else {
        size = 8;
    }
    *encoded = table;
    return size + 2;
}

// Cuts the exponent into windows from the top: a pair of bytes for each, the window's value (odd) and the number of
// zero bits after it, and a pair of zeros at the end. Runs of 256 zeros or more are pairs of 0 and 255 first
// pokebw2's EncodeWindows
static int func_ov114_021cc674(u8 *dest, BigNum *exponent, int window) {
    const u8 *zerosTable;
    int total;
    u32 low;
    u8 *out;
    u32 zeros;
    int wordsLeft;
    u32 high;
    u32 *next;
    u32 current;
    u32 index;
    u32 skip;
    int size;
    u32 bitPos;
    u32 mask;

    if (window > 6) {
        window = 6;
    }
    zerosTable = data_ov114_021cf930[window];
    mask = (1 << window) - 1;
    bitPos = 0;
    total = (window + exponent->length * 32 - 1) / window * 2 + 2;
    out = dest + total;
    *out-- = 0;
    *out-- = 0;
    next = exponent->digits;
    wordsLeft = exponent->length;
    current = low = *next++;
    if (wordsLeft <= 1) {
        high = 0;
    } else {
        high = *next++;
    }
    zeros = 0;
    for (;;) {
        for (;;) {
            index = current & mask;
            skip = zerosTable[index];
            if (skip == 0) {
                break;
            }
            bitPos += skip;
            zeros += skip;
            if (bitPos >= 32) {
                if (wordsLeft <= 1) {
                    break;
                }
                wordsLeft--;
                low = high;
                high = 0;
                if (wordsLeft > 1) {
                    high = *next++;
                }
                bitPos -= 32;
            }
            if (bitPos == 0) {
                current = low;
            } else {
                current = (low >> bitPos) | (high << (32 - bitPos));
            }
        }
        if (index == 0) {
            break;
        }
        *out-- = zeros;
        *out-- = index;
        if (zeros >= 256) {
            while (zeros >= 256) {
                zeros -= 256;
                *out-- = 0xff;
                *out-- = 0;
            }
        }
        bitPos += window;
        zeros = window;
        if (bitPos >= 32) {
            if (wordsLeft <= 1) {
                break;
            }
            wordsLeft--;
            low = high;
            high = 0;
            if (wordsLeft > 1) {
                high = *next++;
            }
            bitPos -= 32;
        }
        if (bitPos == 0) {
            current = low;
        } else {
            current = (low >> bitPos) | (high << (32 - bitPos));
        }
    }
    out++;
    size = 2;
    while (out[0] != 0 || out[1] != 0) {
        dest[0] = out[0];
        dest[1] = out[1];
        out += 2;
        size += 2;
        dest += 2;
    }
    dest[0] = 0;
    dest[1] = 0;
    return size;
}

// Sets up the Montgomery form for an odd modulus: its copy, R squared modulo it, and the word that the reduction
// multiplies by
// pokebw2's MontInit
static BOOL func_ov114_021cc80c(Montgomery *mont, BigNum *modulus, BigNumPool *pool) {
    BOOL ok = FALSE;
    BigNum work;
    u32 first;
    u32 high;
    u32 low;

    if (modulus->length == 0) {
        return FALSE;
    }
    if (func_ov114_021cae20(&mont->modulus, modulus) == NULL) {
        return FALSE;
    }
    func_ov114_021cacd0(&work);
    mont->unk0 = 1;
    mont->words = (func_ov114_021cab94(modulus) + 31) / 32;
    if (!func_ov114_021caec8(&mont->rSquared, 0)) {
        return FALSE;
    }
    if (!func_ov114_021cb0c8(&mont->rSquared, 32)) {
        goto done;
    }
    first = modulus->digits[0];
    if (!func_ov114_021caec8(&work, func_ov114_021cc4a0(first))) {
        goto done;
    }
    if (!func_ov114_021cb1a8(&work, &work, 32)) {
        goto done;
    }
    if (!(work.length == 0 || (work.length == 1 && work.digits[0] == 0))) {
        func_ov114_021cb494(&work, 1);
    } else if (!func_ov114_021caec8(&work, -1)) {
        goto done;
    }
    low = work.length >= 1 ? work.digits[0] : 0;
    high = work.length >= 2 ? work.digits[1] : 0;
    mont->inverse = func_ov114_021cdae8(high, low, first);
    func_ov114_021caec8(&mont->rSquared, 0);
    if (!func_ov114_021cb0c8(&mont->rSquared, mont->words * 64)) {
        goto done;
    }
    func_ov114_021cc0fc(&mont->rSquared, &mont->rSquared, &mont->modulus, pool);
    if (func_ov114_021cac24(&mont->rSquared, mont->words) != NULL) {
        ok = TRUE;
    }
done:
    func_ov114_021cabdc(&work);
    return ok;
}

// pokebw2's NewMont
static Montgomery *func_ov114_021cc9c4(void) {
    Montgomery *mont = func_ov114_021caab4(sizeof(Montgomery));

    if (mont == NULL) {
        return NULL;
    }
    func_ov114_021cc9f0(mont);
    mont->flags = 1;
    return mont;
}

// pokebw2's InitMont
static void func_ov114_021cc9f0(Montgomery *mont) {
    mont->unk0 = 0;
    mont->words = 0;
    func_ov114_021cacd0(&mont->rSquared);
    func_ov114_021cacd0(&mont->modulus);
    func_ov114_021cacd0(&mont->unk34);
    mont->flags = 0;
}

// pokebw2's FreeMont
static void func_ov114_021cca24(Montgomery *mont) {
    func_ov114_021cabdc(&mont->rSquared);
    func_ov114_021cabdc(&mont->modulus);
    func_ov114_021cabdc(&mont->unk34);
    if ((mont->flags & 1) != 0) {
        func_ov114_021caae8(mont);
    }
}

// pokebw2's InitReducer
static void func_ov114_021cca5c(Reducer *reducer) {
    func_ov114_021cacd0(&reducer->modulus);
    func_ov114_021cacd0(&reducer->reciprocal);
    reducer->modulusBits = 0;
    reducer->flags = 0;
}

// pokebw2's FreeReducer
static void func_ov114_021cca80(Reducer *reducer) {
    func_ov114_021cabdc(&reducer->modulus);
    func_ov114_021cabdc(&reducer->reciprocal);
    if ((reducer->flags & 1) != 0) {
        func_ov114_021caae8(reducer);
    }
}

// pokebw2's SetReducerModulus
static int func_ov114_021ccaac(Reducer *reducer, BigNum *modulus, BigNumPool *pool) {
    func_ov114_021cae20(&reducer->modulus, modulus);
    func_ov114_021caec8(&reducer->reciprocal, 0);
    reducer->modulusBits = func_ov114_021cab94(modulus);
    reducer->reciprocalBits = 0;
    return TRUE;
}

// Multiplies two numbers and reduces the product
// pokebw2's ModMul
static BOOL func_ov114_021ccae4(BigNum *dest, BigNum *a, BigNum *b, Reducer *reducer, BigNumPool *pool) {
    BigNum *product = &pool->numbers[pool->used++];
    BOOL ok = FALSE;

    if (b != NULL) {
        if (a == b) {
            if (!func_ov114_021cc178(product, a, pool)) {
                goto done;
            }
        } else {
            if (!func_ov114_021cb9bc(product, a, b, pool)) {
                goto done;
            }
        }
    } else {
        product = a;
    }
    func_ov114_021ccb88(NULL, dest, product, reducer, pool);
    ok = TRUE;
done:
    pool->used--;
    return ok;
}

// Whether the number is zero
static inline BOOL IsZero(BigNum *n) {
    return n->length == 0 || (n->length == 1 && n->digits[0] == 0);
}

// Divides with the reciprocal of the modulus: the quotient is taken from the top half of the product with the
// reciprocal, and off by at most 3
// pokebw2's CryptReduce
static BOOL func_ov114_021ccb88(BigNum *quotient, BigNum *remainder, BigNum *numerator, Reducer *reducer, BigNumPool *pool) {
    int saved = pool->used;
    BigNum *first = &pool->numbers[pool->used++];
    BigNum *second = &pool->numbers[pool->used++];
    BOOL ok = FALSE;
    int bits;
    int shift;
    int tries;
    int half;

    if (quotient == NULL) {
        quotient = &pool->numbers[pool->used++];
    }
    if (remainder == NULL) {
        remainder = &pool->numbers[pool->used++];
    }
    if (func_ov114_021cb078(numerator, &reducer->modulus) < 0) {
        func_ov114_021caec8(quotient, 0);
        func_ov114_021cae20(remainder, numerator);
        pool->used = saved;
        return TRUE;
    }
    bits = func_ov114_021cab94(numerator);
    if (bits < reducer->modulusBits * 2) {
        bits = reducer->modulusBits * 2;
        shift = 0;
    } else {
        shift = (bits - reducer->modulusBits * 2) / 2;
    }
    half = bits / 2;
    if (bits != reducer->reciprocalBits) {
        reducer->reciprocalBits = func_ov114_021ccdfc(&reducer->reciprocal, &reducer->modulus, bits, pool);
    }
    if (func_ov114_021cb2a4(first, numerator, half - shift) && func_ov114_021cb9bc(second, first, &reducer->reciprocal, pool)
        && func_ov114_021cb2a4(quotient, second, shift + half)) {
        quotient->negative = 0;
        if (func_ov114_021cb9bc(second, &reducer->modulus, quotient, pool) && func_ov114_021cb764(remainder, numerator, second)) {
            remainder->negative = 0;
            tries = 0;
            while (func_ov114_021cb078(remainder, &reducer->modulus) >= 0) {
                if (tries++ > 2) {
                    goto done;
                }
                if (!func_ov114_021cb764(remainder, remainder, &reducer->modulus)) {
                    goto done;
                }
                if (!func_ov114_021cb3b4(quotient, 1)) {
                    goto done;
                }
            }
            remainder->negative = IsZero(remainder) ? 0 : numerator->negative;
            ok = TRUE;
            quotient->negative = numerator->negative ^ reducer->modulus.negative;
        }
    }
done:
    pool->used = saved;
    return ok;
}

// The reciprocal of the modulus, scaled by 2^bits, and the bits when it worked
// pokebw2's ComputeReciprocal

static int func_ov114_021ccdfc(BigNum *quotient, BigNum *denominator, int bits, BigNumPool *pool) {
    BigNum power;
    int result = -1;

    func_ov114_021cacd0(&power);
    func_ov114_021caec8(&power, 0);
    if (func_ov114_021cb0c8(&power, bits)) {
        if (func_ov114_021cbb80(quotient, NULL, &power, denominator, pool)) {
            result = bits;
        }
    }
    func_ov114_021cabdc(&power);
    return result;
}

// Raises to a power modulo with squarings and multiplications, a window of the exponent at a time
static BOOL func_ov114_021cce7c(BigNum *dest, BigNum *base, BigNum *exponent, BigNum *modulus,
                        BigNumPool *pool) {
    int i, j, bits, ret = FALSE, wstart, wend, window, wvalue;
    int start = 1, ts = 0;
    BigNum *aa;
    BigNum table[16];
    Reducer reducer;

    bits = func_ov114_021cab94(exponent);

    if (base->length == 0 || (base->length == 1 && base->digits[0] == 0)) {
        func_ov114_021caec8(dest, 0);
        return TRUE;
    }
    if (exponent->length == 0 || (exponent->length == 1 && exponent->digits[0] == 0)) {
        func_ov114_021caec8(dest, 1);
        return TRUE;
    }
    if (exponent->length == 1 && exponent->digits[0] == 1) {
        func_ov114_021cae20(dest, base);
        return TRUE;
    }
    func_ov114_021cca5c(&reducer);
    if (func_ov114_021ccaac(&reducer, modulus, pool) <= 0) {
        goto err;
    }
    func_ov114_021cacd0(&table[0]);
    ts = 1;
    aa = &pool->numbers[pool->used++];
    if (!func_ov114_021cc0fc(&table[0], base, modulus, pool)) {
        goto err;
    }
    if (!func_ov114_021ccae4(aa, &table[0], &table[0], &reducer, pool)) {
        goto err;
    }
    if (bits <= 17) {
        window = 1;
    } else if (bits >= 256) {
        window = 5;
    } else if (bits >= 128) {
        window = 4;
    } else {
        window = 3;
    }
    j = 1 << (window - 1);
    for (i = 1; i < j; i++) {
        func_ov114_021cacd0(&table[i]);
        if (!func_ov114_021ccae4(&table[i], &table[i - 1], aa, &reducer, pool)) {
            goto err;
        }
    }
    ts = i;
    start = 1;
    wvalue = 0;
    wstart = bits - 1;
    wend = 0;
    if (!func_ov114_021caec8(dest, 1)) {
        goto err;
    }
    for (;;) {
        if (func_ov114_021cb15c(exponent, wstart) == 0) {
            if (!start) {
                if (!func_ov114_021ccae4(dest, dest, dest, &reducer, pool)) {
                    goto err;
                }
            }
            if (wstart == 0) {
                break;
            }
            wstart--;
            continue;
        }
        j = wstart;
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0) {
                break;
            }
            if (func_ov114_021cb15c(exponent, wstart - i)) {
                wvalue <<= i - wend;
                wvalue |= 1;
                wend = i;
            }
        }
        j = wend + 1;
        if (!start) {
            for (i = 0; i < j; i++) {
                if (!func_ov114_021ccae4(dest, dest, dest, &reducer, pool)) {
                    goto err;
                }
            }
        }
        if (!func_ov114_021ccae4(dest, dest, &table[wvalue >> 1], &reducer, pool)) {
            goto err;
        }
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0) {
            break;
        }
    }
    ret = TRUE;
err:
    pool->used--;
    for (i = 0; i < ts; i++) {
        func_ov114_021cabc0(&table[i]);
    }
    func_ov114_021cca80(&reducer);
    return ret;
}

// Reads a pair of the exponent's windows: a window's value and the number of squarings before the next one; a number
// of squarings over 0xfe goes on in the next pairs of a zero value with 0xff
static inline void ReadWindow(const u8 **cursor, int *index, int *squarings) {
    int value = *(*cursor)++;

    *squarings = *(*cursor)++;
    if (*squarings == 0xff && value == 0) {
        value = *(*cursor)++;
        while (**cursor == 0xff && value == 0) {
            *squarings += 0x100;
            (*cursor)++;
            value = *(*cursor)++;
        }
        *squarings += *(*cursor)++ + 1;
    }
    *index = value;
}

// Raises to a power modulo an odd number in the Montgomery form, with a table of the odd powers of the base
static BOOL func_ov114_021cd210(BigNum *dest, BigNum *base, BigNum *exponent, BigNum *modulus,
                       BigNumPool *pool, Montgomery *mont) {
    u32 *table[16];
    const u8 *cursor = NULL;
    BOOL ok = FALSE;
    int saved;
    int entries;
    BigNum *reduced;
    Montgomery *made;
    CryptProgress *progress = NULL;
    int counter = 0;
    int n;
    int perWord;
    BigNum *scratchNum;
    BigNum *accNum;
    BigNum *tableNum;
    BigNum *productNum;
    int tableWords;
    int squarings;
    u32 *acc;
    u32 inverse;
    u32 *product;
    u32 *modulusDigits;
    int i;
    int index;
    u32 *scratch;

    if ((modulus->digits[0] & 1) == 0) {
        return FALSE;
    }
    saved = pool->used;
    if (base->length == 0 || (base->length == 1 && base->digits[0] == 0)) {
        func_ov114_021caec8(dest, 0);
        return TRUE;
    }
    if (exponent->length == 0 || (exponent->length == 1 && exponent->digits[0] == 0)) {
        func_ov114_021caec8(dest, 1);
        return TRUE;
    }
    if (exponent->length == 1 && exponent->digits[0] == 1) {
        func_ov114_021cae20(dest, base);
        return TRUE;
    }
    made = mont;
    if (made == NULL) {
        made = func_ov114_021cc9c4();
        if (made == NULL) {
            goto done;
        }
        if (!func_ov114_021cc80c(made, modulus, pool)) {
            goto done;
        }
    }
    progress = pool->progress;
    if (!func_ov114_021cc530(exponent, &cursor, 0, pool)) {
        goto done;
    }
    pool->used++;
    entries = cursor[3];
    perWord = (cursor[2] + 63) / cursor[2];
    cursor += 4;
    n = made->modulus.length;
    if (base->length == n && base->digits[n - 1] < modulus->digits[n - 1]) {
        reduced = base;
    } else if (base->length < n) {
        reduced = &pool->numbers[pool->used++];
        reduced->length = base->length;
        if (func_ov114_021cac24(reduced, n) == NULL) {
            goto done;
        }
        for (i = 0; i < base->length; i++) {
            reduced->digits[i] = base->digits[i];
        }
    } else {
        reduced = &pool->numbers[pool->used++];
        if (!func_ov114_021cc0fc(reduced, base, modulus, pool)) {
            goto done;
        }
        if (func_ov114_021cac24(reduced, n) == NULL) {
            goto done;
        }
    }
    tableWords = entries * n + (exponent->length * perWord * 2 + 7) / 4;
    tableNum = &pool->numbers[pool->used++];
    scratchNum = &pool->numbers[pool->used++];
    accNum = &pool->numbers[pool->used++];
    productNum = &pool->numbers[pool->used++];
    if (Reserve(dest, n) == NULL) {
        goto done;
    }
    if (Reserve(scratchNum, n * 4) == NULL) {
        goto done;
    }
    if (Reserve(accNum, n * 2) == NULL) {
        goto done;
    }
    if (Reserve(tableNum, tableWords) == NULL) {
        goto done;
    }
    if (Reserve(productNum, n * 2) == NULL) {
        goto done;
    }
    scratch = scratchNum->digits;
    acc = accNum->digits;
    product = productNum->digits;
    table[0] = tableNum->digits;
    inverse = made->inverse;
    modulusDigits = made->modulus.digits;
    func_ov114_021cba90(product, reduced->digits, n, made->rSquared.digits, n);
    func_ov114_021cc38c(table[0], product, modulusDigits, n, inverse);
    if (entries > 1) {
        func_ov114_021cc2b0(product, table[0], n, scratch);
        func_ov114_021cc38c(scratch, product, modulusDigits, n, inverse);
        for (i = 1; i < entries; i++) {
            table[i] = table[i - 1] + n;
            func_ov114_021cba90(product, table[i - 1], n, scratch, n);
            func_ov114_021cc38c(table[i], product, modulusDigits, n, inverse);
        }
    }
    ReadWindow(&cursor, &index, &squarings);
    MI_CpuCopy8(table[index >> 1], acc, n * sizeof(u32));
    while (squarings != 0) {
        if (progress != NULL) {
            if (progress->func(progress, 0xff, counter++) != 0) {
                goto done;
            }
        }
        if ((pool->flags & POOL_CANCELED) != 0) {
            goto done;
        }
        for (i = 0; i < squarings; i++) {
            func_ov114_021cc2b0(product, acc, n, scratch);
            func_ov114_021cc38c(acc, product, modulusDigits, n, inverse);
        }
        ReadWindow(&cursor, &index, &squarings);
        if (index == 0 && squarings == 0) {
            break;
        }
        if (squarings != 0 || index != 1) {
            func_ov114_021cba90(product, acc, n, table[index >> 1], n);
            func_ov114_021cc38c(acc, product, modulusDigits, n, inverse);
        } else {
            func_ov114_021cba90(product, acc, n, reduced->digits, n);
            func_ov114_021cc38c(dest->digits, product, modulusDigits, n, inverse);
            goto finish;
        }
    }
    for (i = n; i < n * 2; i++) {
        acc[i] = 0;
    }
    func_ov114_021cc38c(dest->digits, acc, modulusDigits, n, inverse);
finish:
    if ((pool->flags & POOL_CANCELED) != 0) {
        goto done;
    }
    dest->length = n;
    func_ov114_021cac8c(dest);
    ok = TRUE;
done:
    if (progress != NULL) {
        if (progress->func(progress, 0xff, -1) != 0) {
            ok = FALSE;
        }
    }
    if (mont == NULL && made != NULL) {
        func_ov114_021cca24(made);
    }
    pool->used = saved;
    return ok;
}

// The primitives of the numbers' words are MWCC `asm` functions: the assembly that the bundled big-number library
// shipped, unrolled by four with the long multiplications and the add with carry that the compiler does not use
// pokebw2's MulAddWords
#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): dest += a * word over size words;
// the carry word out
static u32 func_ov114_021cd984(u32 *dest, const u32 *a, int size, u32 word) {
    u32 carry = 0;

    for (; size > 0; size--) {
        u64 t = (u64)*a++ * word + *dest + carry;

        *dest++ = (u32)t;
        carry = (u32)(t >> 32);
    }
    return carry;
}
#else
static asm u32 func_ov114_021cd984(u32 *dest, const u32 *a, int size, u32 word) {
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    mov r8, r0
    mov lr, #0x0
    mov r0, #0x0
    subs r2, r2, #0x4
    blt MulAddWords_Rest
    ldr r7, [r8, #0x0]
    ldr r6, [r1], #0x4
MulAddWords_Loop4:
    adds r7, r7, r0
    adc r0, lr, #0x0
    umlal r7, r0, r3, r6
    ldr r5, [r8, #0x4]
    ldr r4, [r1], #0x4
    str r7, [r8], #0x4
    adds r5, r5, r0
    adc r0, lr, #0x0
    umlal r5, r0, r3, r4
    ldr r7, [r8, #0x4]
    ldr r6, [r1], #0x4
    str r5, [r8], #0x4
    adds r7, r7, r0
    adc r0, lr, #0x0
    umlal r7, r0, r3, r6
    ldr r5, [r8, #0x4]
    ldr r4, [r1], #0x4
    str r7, [r8], #0x4
    adds r5, r5, r0
    adc r0, lr, #0x0
    umlal r5, r0, r3, r4
    subs r2, r2, #0x4
    ldrge r7, [r8, #0x4]
    ldrge r6, [r1], #0x4
    str r5, [r8], #0x4
    bge MulAddWords_Loop4
MulAddWords_Rest:
    adds r2, r2, #0x4
    ldmleia sp!, {r4, r5, r6, r7, r8, pc}
MulAddWords_Loop1:
    ldr r7, [r8], #0x0
    ldr r6, [r1], #0x4
    adds r7, r7, r0
    adc r0, lr, #0x0
    umlal r7, r0, r3, r6
    subs r2, r2, #0x1
    str r7, [r8], #0x4
    bgt MulAddWords_Loop1
    ldmia sp!, {r4, r5, r6, r7, r8, pc}
}
#endif

// pokebw2's MulWord
#ifndef __MWERKS__
// C for a PC build: dest = a * word; the carry word out
static u32 func_ov114_021cda38(u32 *dest, const u32 *a, int size, u32 word) {
    u32 carry = 0;

    for (; size > 0; size--) {
        u64 t = (u64)*a++ * word + carry;

        *dest++ = (u32)t;
        carry = (u32)(t >> 32);
    }
    return carry;
}
#else
static asm u32 func_ov114_021cda38(u32 *dest, const u32 *a, int size, u32 word) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    mov r11, r0
    mov r0, #0x0
    subs r2, r2, #0x4
    blt MulWord_Rest
MulWord_Loop4:
    ldr r9, [r1], #0x4
    umull r9, r10, r3, r9
    ldr r7, [r1], #0x4
    ldr r5, [r1], #0x4
    umull r7, r8, r3, r7
    ldr lr, [r1], #0x4
    adds r9, r9, r0
    umull r5, r6, r3, r5
    adcs r7, r7, r10
    str r9, [r11], #0x4
    umull lr, r4, r3, lr
    str r7, [r11], #0x4
    adcs r5, r5, r8
    str r5, [r11], #0x4
    adcs lr, lr, r6
    adc r0, r4, #0x0
    str lr, [r11], #0x4
    subs r2, r2, #0x4
    bge MulWord_Loop4
MulWord_Rest:
    adds r2, r2, #0x4
    ldmleia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}
MulWord_Loop1:
    ldr r9, [r1], #0x4
    mov r10, #0x0
    umlal r0, r10, r9, r3
    subs r2, r2, #0x1
    str r0, [r11], #0x4
    mov r0, r10
    bgt MulWord_Loop1
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}
}
#endif

// pokebw2's SquareDigits
#ifndef __MWERKS__
// C for a PC build: each word's square in two words of dest
static void func_ov114_021cdac0(u32 *dest, const u32 *a, int size) {
    for (; size > 0; size--) {
        u64 t = (u64)*a * *a;

        a++;
        *dest++ = (u32)t;
        *dest++ = (u32)(t >> 32);
    }
}
#else
static asm void func_ov114_021cdac0(u32 *dest, const u32 *a, int size) {
    stmdb sp!, {r4, r5, lr}
    subs r2, r2, #0x1
    ldmltia sp!, {r4, r5, pc}
SquareDigits_Loop:
    ldr lr, [r1], #0x4
    umull r4, r5, lr, lr
    subs r2, r2, #0x1
    str r4, [r0], #0x4
    str r5, [r0], #0x4
    bge SquareDigits_Loop
    ldmia sp!, {r4, r5, pc}
}
#endif

// Divides a double word by a word; the quotient comes back as a double word with a zero high word
// pokebw2's DivWords
#ifndef __MWERKS__
// C for a PC build: the quotient's low word
static u32 func_ov114_021cdae8(u32 high, u32 low, u32 divisor) {
    return (u32)((((u64)high << 32) | low) / divisor);
}
#else
static asm u32 func_ov114_021cdae8(u32 high, u32 low, u32 divisor) {
    stmdb sp!, {r4, lr}
    mov r4, r0
    mov r0, r1
    mov r1, r4
    mov r3, #0x0
    bl _ll_udiv
    mov r1, #0x0
    ldmia sp!, {r4, pc}
}
#endif

// pokebw2's AddWords
#ifndef __MWERKS__
// C for a PC build: dest = a + b; the carry out
static u32 func_ov114_021cdb08(u32 *dest, const u32 *a, const u32 *b, int size) {
    u32 carry = 0;

    for (; size > 0; size--) {
        u64 t = (u64)*a++ + *b++ + carry;

        *dest++ = (u32)t;
        carry = (u32)(t >> 32);
    }
    return carry;
}
#else
static asm u32 func_ov114_021cdb08(u32 *dest, const u32 *a, const u32 *b, int size) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    mov r9, r0
    subs r3, r3, #0x4
    mov r0, #0x0
    mov lr, #0x0
    blt AddWords_Rest
AddWords_Loop4:
    ldr r8, [r1], #0x4
    ldr r6, [r2], #0x4
    subs r0, r0, #0x1
    ldr r7, [r1], #0x4
    ldr r5, [r2], #0x4
    adcs r8, r8, r6
    adcs r7, r7, r5
    str r8, [r9], #0x4
    str r7, [r9], #0x4
    ldr r8, [r1], #0x4
    ldr r6, [r2], #0x4
    ldr r7, [r1], #0x4
    ldr r5, [r2], #0x4
    adcs r8, r8, r6
    adcs r7, r7, r5
    adc r0, lr, #0x0
    str r8, [r9], #0x4
    subs r3, r3, #0x4
    str r7, [r9], #0x4
    bge AddWords_Loop4
AddWords_Rest:
    adds r3, r3, #0x4
    ldmleia sp!, {r4, r5, r6, r7, r8, r9, pc}
AddWords_Loop1:
    ldr r8, [r1], #0x4
    ldr r6, [r2], #0x4
    subs r0, r0, #0x1
    adcs r8, r8, r6
    adc r0, lr, #0x0
    str r8, [r9], #0x4
    subs r3, r3, #0x1
    bgt AddWords_Loop1
    ldmia sp!, {r4, r5, r6, r7, r8, r9, pc}
}
#endif

// pokebw2's SubWords
#ifndef __MWERKS__
// C for a PC build: dest = a - b; the borrow out
static u32 func_ov114_021cdb9c(u32 *dest, const u32 *a, const u32 *b, int size) {
    u32 borrow = 0;

    for (; size > 0; size--) {
        u64 t = (u64)*a++ - *b++ - borrow;

        *dest++ = (u32)t;
        borrow = (u32)(t >> 32) != 0;
    }
    return borrow;
}
#else
static asm u32 func_ov114_021cdb9c(u32 *dest, const u32 *a, const u32 *b, int size) {
    stmdb sp!, {r4, r5, r6, r7, lr}
    mov r7, r0
    subs r3, r3, #0x1
    mov r0, #0x0
    ldmltia sp!, {r4, r5, r6, r7, pc}
    mov lr, #0x0
SubWords_Loop:
    ldr r5, [r2], #0x4
    ldr r6, [r1], #0x4
    adds r5, r5, r0
    adc r0, lr, #0x0
    subs r6, r6, r5
    movlo r0, #0x1
    subs r3, r3, #0x1
    str r6, [r7], #0x4
    bge SubWords_Loop
    ldmia sp!, {r4, r5, r6, r7, pc}
}
#endif
