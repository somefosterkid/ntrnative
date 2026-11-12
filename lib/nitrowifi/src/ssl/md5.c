// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/ssl/md5.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "ssl_internal.h"

#include "gfl/std.h"

// SSL's own MD5, the public-domain algorithm from RFC 1321, written with the usual table-driven round loop and the
// no-swap trick of rotating which variable FF/GG/HH/II writes.

// The per-round rotation amounts, repeated every four steps
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define F(b, c, d) ((d) ^ ((b) & ((c) ^ (d))))
#define G(b, c, d) ((c) ^ ((d) & ((b) ^ (c))))
#define H(b, c, d) ((b) ^ (c) ^ (d))
#define I(b, c, d) ((c) ^ ((b) | ~(d)))

#define FF(a, b, c, d, x, t, s) { (a) += F(b, c, d) + (x) + (t); (a) = (b) + ROTL(a, s); }
#define GG(a, b, c, d, x, t, s) { (a) += G(b, c, d) + (x) + (t); (a) = (b) + ROTL(a, s); }
#define HH(a, b, c, d, x, t, s) { (a) += H(b, c, d) + (x) + (t); (a) = (b) + ROTL(a, s); }
#define II(a, b, c, d, x, t, s) { (a) += I(b, c, d) + (x) + (t); (a) = (b) + ROTL(a, s); }

// The 64-byte padding CPSi_md5_result appends to a message before its length
// pokebw2's sMD5Padding
static u8 data_ov009_021a1de8[64] = {
    0x80,
};

// The 64 additive constants, floor(abs(sin(i + 1)) * 2^32)
// pokebw2's sMD5Constants
static u32 data_ov009_021a1e28[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, //
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, //
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, //
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, //
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, //
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, //
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, //
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, //
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, //
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, //
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, //
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, //
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, //
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, //
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, //
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

// Which of the 16 message words each of the 64 steps mixes in: i for round 1, (5i + 1) % 16 for round 2,
// (3i + 5) % 16 for round 3 and (7i) % 16 for round 4
// pokebw2's sMD5WordIndex
static u8 data_ov009_021a1da8[64] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, //
    1,  6,  11, 0,  5,  10, 15, 4,  9,  14, 3,  8,  13, 2,  7,  12, //
    5,  8,  11, 14, 1,  4,  7,  10, 13, 0,  3,  6,  9,  12, 15, 2,  //
    0,  7,  14, 5,  12, 3,  10, 1,  8,  15, 6,  13, 4,  11, 2,  9,
};

// RFC 1321's Encode, which packs an array of 32-bit words into bytes, low byte first: on this little-endian target
// that is a plain copy, through MI_CpuCopy8 (whose argument order is the opposite of this one's)
// pokebw2's MD5Encode
static void char_from_long_md5(u8 *output, const u32 *input, u32 len) {
    MI_CpuCopy8(input, output, len);
}

// RFC 1321's Decode, the opposite of char_from_long_md5: also a plain copy on this little-endian target
// pokebw2's MD5Decode
static void long_from_char_md5(u32 *output, const u8 *input, u32 len) {
    MI_CpuCopy8(input, output, len);
}

// pokebw2's MD5Transform
static void func_ov009_02162ff0(u32 state[4], const u8 block[64]) {
    u32 a = state[0];
    u32 b = state[1];
    u32 c = state[2];
    u32 d = state[3];
    u32 x[16];
    u32 i;
    int j;

    long_from_char_md5(x, block, 64);
    i = 0;

    for (j = 0; j < 4; j++) {
        FF(a, b, c, d, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S11);
        i++;
        FF(d, a, b, c, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S12);
        i++;
        FF(c, d, a, b, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S13);
        i++;
        FF(b, c, d, a, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S14);
        i++;
    }

    for (j = 0; j < 4; j++) {
        GG(a, b, c, d, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S21);
        i++;
        GG(d, a, b, c, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S22);
        i++;
        GG(c, d, a, b, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S23);
        i++;
        GG(b, c, d, a, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S24);
        i++;
    }

    for (j = 0; j < 4; j++) {
        HH(a, b, c, d, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S31);
        i++;
        HH(d, a, b, c, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S32);
        i++;
        HH(c, d, a, b, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S33);
        i++;
        HH(b, c, d, a, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S34);
        i++;
    }

    for (j = 0; j < 4; j++) {
        II(a, b, c, d, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S41);
        i++;
        II(d, a, b, c, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S42);
        i++;
        II(c, d, a, b, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S43);
        i++;
        II(b, c, d, a, x[data_ov009_021a1da8[i]], data_ov009_021a1e28[i], S44);
        i++;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

// pokebw2's MD5Init
void CPSi_md5_init(MD5Context *context) {
    MI_CpuFill8(context, 0, sizeof(MD5Context));
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

// pokebw2's MD5Update
void CPSi_md5_calc(MD5Context *context, const u8 *input, u32 inputLen) {
    u32 index;
    u32 partLen;
    u32 i;

    index = (context->count[0] >> 3) & 0x3f;

    if ((context->count[0] += inputLen << 3) < (inputLen << 3)) {
        context->count[1]++;
    }
    context->count[1] += inputLen >> 29;

    partLen = 64 - index;

    if (inputLen >= partLen) {
        MI_CpuCopy8(input, &context->buffer[index], partLen);
        index = 0;
        func_ov009_02162ff0(context->state, context->buffer);

        for (i = partLen; i + 63 < inputLen; i += 64) {
            func_ov009_02162ff0(context->state, &input[i]);
        }
    } else {
        i = 0;
    }

    MI_CpuCopy8(&input[i], &context->buffer[index], inputLen - i);
}

// pokebw2's MD5Final
void CPSi_md5_result(MD5Context *context, u8 digest[16]) {
    int index;
    int padLen;

    char_from_long_md5(digest, context->count, 8);

    index = (context->count[0] >> 3) & 0x3f;
    padLen = (index < 56) ? (56 - index) : (120 - index);
    CPSi_md5_calc(context, data_ov009_021a1de8, padLen);
    CPSi_md5_calc(context, digest, 8);

    char_from_long_md5(digest, context->state, 16);
}
