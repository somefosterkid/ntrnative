// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/gf_standard.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "nitro/math.h"
#include "nitro/os.h"

// The Mersenne Twister, MT19937
#define MT_N 624
#define MT_M 397
#define MT_MATRIX_A 0x9908b0df
#define MT_UPPER_MASK 0x80000000
#define MT_LOWER_MASK 0x7fffffff
#define MT_DEFAULT_SEED 5489

typedef struct {
    u32 mt[MT_N];
    int mti;
    u32 mag01[2];
    MATHCRC16Table crcTable;
} GFLStdTables;

// pokebw2's sGFLStd
static GFLStdTables *data_021469f8;


void initTableArea(HeapID heapId) {
    data_021469f8 = GFL_HeapAllocate(heapId, sizeof(GFLStdTables), FALSE, "gf_standard.c", 49);
    data_021469f8->mti = MT_N + 1;
    data_021469f8->mag01[0] = 0;
    data_021469f8->mag01[1] = MT_MATRIX_A;
    MATHi_CRC16InitTable(&data_021469f8->crcTable, 0x1021);
}

s32 GFL_STD_MemCmp(const void *a, const void *b, u32 size) {
    const u8 *pa = a;
    const u8 *pb = b;
    u32 n = size;

    while (n-- != 0) {
        if (*pa != *pb) {
            return *pa - *pb;
        }
        pa++;
        pb++;
    }
    return 0;
}

u32 GFL_STD_StrLen(const char *str) {
    u32 len = 0;

    for (;;) {
        if (*(const u8 *)str == '\0') {
            return len;
        }
        str++;
        len++;
    }
}

int _STD_CompareNString(const char *a, const char *b) {
    return STD_CompareString(a, b);
}



void GFL_RandomUpdateMT(u32 seed) {
    data_021469f8->mt[0] = seed;
    for (data_021469f8->mti = 1; data_021469f8->mti < MT_N; data_021469f8->mti++) {
        data_021469f8->mt[data_021469f8->mti] = 1812433253
                                        * (data_021469f8->mt[data_021469f8->mti - 1] ^ (data_021469f8->mt[data_021469f8->mti - 1] >> 30))
                                    + data_021469f8->mti;
    }
}

u32 GFL_RandomMT(void) {
    u32 y;

    if (data_021469f8->mti >= MT_N) {
        int kk;

        if (data_021469f8->mti == MT_N + 1) {
            GFL_RandomUpdateMT(MT_DEFAULT_SEED);
        }
        for (kk = 0; kk < MT_N - MT_M; kk++) {
            y = (data_021469f8->mt[kk] & MT_UPPER_MASK) | (data_021469f8->mt[kk + 1] & MT_LOWER_MASK);
            data_021469f8->mt[kk] = data_021469f8->mt[kk + MT_M] ^ (y >> 1) ^ data_021469f8->mag01[y & 1];
        }
        for (; kk < MT_N - 1; kk++) {
            y = (data_021469f8->mt[kk] & MT_UPPER_MASK) | (data_021469f8->mt[kk + 1] & MT_LOWER_MASK);
            data_021469f8->mt[kk] = data_021469f8->mt[kk + (MT_M - MT_N)] ^ (y >> 1) ^ data_021469f8->mag01[y & 1];
        }
        y = (data_021469f8->mt[MT_N - 1] & MT_UPPER_MASK) | (data_021469f8->mt[0] & MT_LOWER_MASK);
        data_021469f8->mt[MT_N - 1] = data_021469f8->mt[MT_M - 1] ^ (y >> 1) ^ data_021469f8->mag01[y & 1];
        data_021469f8->mti = 0;
    }
    y = data_021469f8->mt[data_021469f8->mti++];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= y >> 18;
    return y;
}

u16 getCRC16(const void *data, u32 size) {
    if (size < 2) {
        u8 bytes[2];
        u8 byte = *(const u8 *)data;

        bytes[0] = byte;
        bytes[1] = byte;
        return MATH_CalcCRC16CCITT(&data_021469f8->crcTable, bytes, 2);
    }
    return MATH_CalcCRC16CCITT(&data_021469f8->crcTable, data, size);
}

void buildSeed(MATHRandContext32 *context) {
    MATHSHA1Context sha1;
    u32 entropy[8];
    u8 digest[MATH_SHA1_DIGEST_SIZE];
    u64 seed;

    // The digest's buffer is hashed before it holds the digest, for whatever the stack held
    OS_GetLowEntropyData(entropy);
    MATH_SHA1Init(&sha1);
    MATH_SHA1Update(&sha1, digest, sizeof(digest));
    MATH_SHA1Update(&sha1, entropy, sizeof(entropy));
    MATH_SHA1GetHash(&sha1, digest);
    MI_CpuCopy8(digest, &seed, sizeof(seed));
    MATH_InitRand32(context, seed);
}



u32 addUpDataStream(const u8 *data, u32 size) {
    u32 sum = 0;
    u32 i;

    for (i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum;
}

void decryptData(void *data, u32 size, u32 seed) {
    u16 *words = data;
    u32 i;

    for (i = 0; i < size / 2; i++) {
        words[i] ^= func_0203f388(&seed);
    }
}

void _decryptData(void *data, u32 size, u32 seed) {
    decryptData(data, size, seed);
}

u16 func_0203f388(u32 *seed) {
    *seed = *seed * 1103515245 + 24691;
    return (u16)(*seed >> 16);
}
