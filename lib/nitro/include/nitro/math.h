// NitroSDK's math macros. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/nitro/include/nitro/math.h, trimmed to what Black's C uses so far.
#ifndef BWPORT_NITRO_MATH_H
#define BWPORT_NITRO_MATH_H

#include "types.h"

#define MATH_ABS(a) (((a) < 0) ? -(a) : (a))

#define MATH_CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define MATH_MAX(a, b) (((a) >= (b)) ? (a) : (b))
#define MATH_MIN(a, b) (((a) <= (b)) ? (a) : (b))

#define MATH_ROUNDUP(x, base) (((x) + ((base) - 1)) & ~((base) - 1))
#define MATH_ROUNDDOWN(x, base) ((x) & ~((base) - 1))
#define MATH_ROUNDUP32(x) MATH_ROUNDUP(x, 32)
#define MATH_ROUNDDOWN32(x) MATH_ROUNDDOWN(x, 32)

// The block size of the hash functions (math/dgt.h)
#define MATH_HASH_BLOCK_SIZE (512 / 8)

static inline int MATH_IMin(int a, int b) {
    return (a <= b) ? a : b;
}

static inline int MATH_IMax(int a, int b) {
    return (a >= b) ? a : b;
}

typedef struct {
    u64 x;
    u64 mul;
    u64 add;
} MATHRandContext32;

typedef struct {
    u32 x;
    u32 mul;
    u32 add;
} MATHRandContext16;

static inline void MATH_InitRand16(MATHRandContext16 *context, u32 seed) {
    context->x = seed;
    context->mul = 1566083941LL;
    context->add = 2531011;
}

static inline u16 MATH_Rand16(MATHRandContext16 *context, u16 max) {
    context->x = context->mul * context->x + context->add;
    if (max == 0) {
        return (u16)(context->x >> 16);
    }
    return (u16)(((context->x >> 16) * max) >> 16);
}

// CRC-16/CCITT through a table, and SHA-1
typedef struct {
    u16 table[256];
} MATHCRC16Table;

typedef struct {
    u32 state[5];
    union {
        u8 bytes[64];
        u32 words[16];
    } block;
    u32 blockPos;   // bytes of block filled
    u32 blockCount; // 64-byte blocks processed, as a 64-bit count in two words
    u32 blockCountHigh;
} MATHSHA1Context;

#define MATH_SHA1_DIGEST_SIZE 20

// Quicksort of num elements of width bytes; stackBuffer may be NULL
typedef s32 (*MATHCompareFunc)(void *a, void *b);
void MATH_QSort(void *head, u32 num, u32 width, MATHCompareFunc comp, void *stackBuf);

// The number of leading zero bits (32 for 0): NitroSDK's MATH_CountLeadingZeros, whose inline version is ARM code
#ifdef __MWERKS__
#pragma thumb off
static inline u32 MATH_CountLeadingZerosInline(u32 x) {
    asm {
        clz x, x
    }
    return x;
}
#pragma thumb reset
#else
// C for a PC build (MWCC defines __MWERKS__): the leading zero bits counted
static inline u32 MATH_CountLeadingZerosInline(u32 x) {
    u32 n = 0;

    if (x == 0) {
        return 32;
    }
    while (!(x & 0x80000000)) {
        x <<= 1;
        n++;
    }
    return n;
}
#endif

static inline u32 MATH_CountLeadingZeros(u32 x) {
    return MATH_CountLeadingZerosInline(x);
}

// The number of set bits, NitroSDK's MATH_CountPopulation (swan's name)
u8 MATH_CountPopulation(u32 x);

static inline void MATH_InitRand32(MATHRandContext32 *context, u64 seed) {
    context->x = seed;
    context->mul = (1566083941LL << 32) + 1812433253LL;
    context->add = 2531011;
}

// A random number below max, or any 32-bit number when max is 0
static inline u32 MATH_Rand32(MATHRandContext32 *context, u32 max) {
    context->x = context->mul * context->x + context->add;
    if (max == 0) {
        return (u32)(context->x >> 32);
    } else {
        return (u32)(((context->x >> 32) * max) >> 32);
    }
}

// The SHA-1 of a message in three steps: MATH_SHA1Init, MATH_SHA1Update and MATH_SHA1GetHash's code
void MATH_SHA1Init(MATHSHA1Context *context);
void MATH_SHA1Update(MATHSHA1Context *context, const void *data, u32 size);
void MATH_SHA1GetHash(MATHSHA1Context *context, void *digest);
// The SHA-1 digest of size bytes, through the three above (MATH_CalcSHA1's code)
void MATH_CalcSHA1(u8 *digest, const void *data, u32 size);
// The CRC-16/CCITT of size bytes through a table (MATH_CalcCRC16CCITT's code)
u16 MATH_CalcCRC16CCITT(const MATHCRC16Table *table, const void *data, u32 size);
// Fills the table for the CRC-16/CCITT with the polynomial poly (MATHi_CRC16InitTable's code)
void MATHi_CRC16InitTable(MATHCRC16Table *table, u16 poly);

// Added for math_crc.c: the CRC-8 and CRC-32 tables
typedef struct {
    u8 table[256];
} MATHCRC8Table;

typedef struct {
    u32 table[256];
} MATHCRC32Table;

// The state of an MD5 or SHA-1 digest: the chaining words, the block being filled and the count of what was fed in
typedef struct {
    u32 state[4];
    u64 length; // in bytes
    union {
        u8 bytes[64];
        u32 words[16];
    } block;
} MATHMD5Context;

// What MATH_CalcHMACSHA1's generic HMAC needs to know about a hash: the sizes of its digest and block, a context and a
// buffer for a digest, and its three calls
typedef struct {
    u32 digestSize;
    u32 blockSize;
    void *context;
    void *digest;
    void (*init)(void *context);
    void (*update)(void *context, const void *data, u32 size);
    void (*getHash)(void *context, void *digest);
} MATHHashInfo;

// math_crc.c. Swan: MATHi_CRC16InitTable (MATHi_CRC16InitTable)
void MATHi_CRC8InitTable(MATHCRC8Table *table, u8 poly);
void MATHi_CRC16InitTableRev(MATHCRC16Table *table, u16 poly);
void MATHi_CRC16UpdateRev(const MATHCRC16Table *table, u16 *crc, const void *data, u32 size);
void MATHi_CRC32InitTableRev(MATHCRC32Table *table, u32 poly);
u8 MATH_CalcCRC8(const MATHCRC8Table *table, const void *data, u32 size);
u16 MATH_CalcCRC16(const MATHCRC16Table *table, const void *data, u32 size);
u32 MATH_CalcCRC32(const MATHCRC32Table *table, const void *data, u32 size);
// The HMAC over any hash that info describes (out is the MAC; data, key and their sizes as given to MATH_CalcHMACSHA1)
void MATHi_CalcHMAC(void *out, const void *data, u32 dataSize, const void *key, u32 keySize, const MATHHashInfo *info);
// Whether SHA-1 runs its overlay-table variant (MATHi_SHA1ProcessBlockForOverlay), which fs_overlay.c turns on to check
// an overlay table's digest; returns the old mode
int MATHi_SetOverlayTableMode(int flag);
// pokebw2's sha1_block
// sha1_block.c
// The SHA-1 compression of the context's block into its state, in ARM
void MATHi_SHA1ProcessBlock(MATHSHA1Context *context);

// math_md5.c (MD5) and math_sha1.c (SHA-1 and HMAC-SHA1); the ROM's data order says they are two files, not one
void MATH_MD5Init(MATHMD5Context *context);
void MATH_MD5Update(MATHMD5Context *context, const void *data, u32 size);
void MATH_MD5GetHash(MATHMD5Context *context, void *digest);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/math/dgt.h, include/nitro/math/math.h
static inline void MATH_MD5GetDigest (MATHMD5Context * context, void * digest)
{
	MATH_MD5GetHash(context, digest);
}
static inline void MATH_SHA1GetDigest (MATHSHA1Context * context, void * digest)
{
	MATH_SHA1GetHash(context, digest);
}
static inline int MATH_IAbs (int a)
{
	return (a < 0) ? -a : a;
}
static inline u32 MATH_CountTrailingZeros(u32 x)
{
	return (u32)(32 - MATH_CountLeadingZeros((u32)(~x & (x - 1))));
}
static inline u32 MATH_GetLeastSignificantBit(u32 x)
{
	return (u32)(x & - (s32)x);
}
static inline u32 MATH_GetMostSignificantBit(u32 x)
{
	return (u32)(x & ((s32)0x80000000 >> MATH_CountLeadingZeros(x)));
}

#endif // BWPORT_NITRO_MATH_H
