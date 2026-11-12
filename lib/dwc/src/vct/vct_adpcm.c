// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/vct/vct_adpcm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "vct_internal.h"
#include "vct_adpcm.h"
#include "types.h"

// Adaptive differential PCM for the voice chat, at 4, 3 and 2 bits a sample. Each codec keeps a predicted sample and
// an index into the table of step sizes, and the code of a sample says how far from the prediction the sample was
// and which way, to the precision of the step.

// pokebw2's sIndexAdjust2
static const s8 data_ov013_021b3a00[4] = {-1, 1, -1, 1};
// pokebw2's sIndexAdjust3
static const s8 data_ov013_021b3a04[8] = {-1, -1, 1, 2, -1, -1, 1, 2};
// pokebw2's sIndexAdjust4
static const s8 data_ov013_021b3a0c[16] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};
// pokebw2's sVctAdpcmStepTable
static const s16 data_ov013_021b3a1c[89] = {
    7,     8,     9,     10,    11,    12,    13,    14,    16,    17,    19,    21,    23,    25,    28,    31,
    34,    37,    41,    45,    50,    55,    60,    66,    73,    80,    88,    97,    107,   118,   130,   143,
    157,   173,   190,   209,   230,   253,   279,   307,   337,   371,   408,   449,   494,   544,   598,   658,
    724,   796,   876,   963,   1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066,  2272,  2499,  2749,  3024,
    3327,  3660,  4026,  4428,  4871,  5358,  5894,  6484,  7132,  7845,  8630,  9493,  10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767,
};

static void func_ov013_021af340(const u8 *src, s16 *dst, u32 count, VCTAdpcmState *state);
static void func_ov013_021af494(u8 *dst, const s16 *src, u32 count, VCTAdpcmState *state);
static void func_ov013_021af628(const u8 *src, s16 *dst, u32 count, VCTAdpcmState *state);
static void func_ov013_021afabc(u8 *dst, s16 *src, u32 count, VCTAdpcmState *state);
static void func_ov013_021affe4(const u8 *src, s16 *dst, u32 count, VCTAdpcmState *state);
static void func_ov013_021b01fc(u8 *dst, s16 *src, u32 count, VCTAdpcmState *state);

// Moves the step index by the adjustment of a code, within the table
#define ADVANCE_STEP_INDEX(index, adjust) \
    do {                                  \
        (index) += (adjust);              \
        if ((index) < 0) {                \
            (index) = 0;                  \
        } else if ((index) > 88) {        \
            (index) = 88;                 \
        }                                 \
    } while (0)

static inline s16 DecodeSample4(VCTAdpcmState *state, int code)
{
    int sample = state->predictor;
    int index = state->stepIndex;
    int step = data_ov013_021b3a1c[index];
    int diff = step >> 3;

    if (code & 4) {
        diff += step;
    }
    if (code & 2) {
        diff += step >> 1;
    }
    if (code & 1) {
        diff += step >> 2;
    }
    if (code & 8) {
        sample -= diff;
        if (sample < -0x8000) {
            sample = -0x8000;
        }
    } else {
        sample += diff;
        if (sample > 0x7fff) {
            sample = 0x7fff;
        }
    }
    index = index + data_ov013_021b3a0c[code];
    if (index < 0) {
        index = 0;
    } else if (index > 88) {
        index = 88;
    }
    state->predictor = sample;
    state->stepIndex = index;
    return sample;
}

static inline s16 DecodeSample3(VCTAdpcmState *state, int code)
{
    int sample = state->predictor;
    int index = state->stepIndex;
    int step = data_ov013_021b3a1c[index];
    int diff = step >> 2;

    if (code & 2) {
        diff += step;
    }
    if (code & 1) {
        diff += step >> 1;
    }
    if (code & 4) {
        sample -= diff;
        if (sample < -0x8000) {
            sample = -0x8000;
        }
    } else {
        sample = sample + diff;
        if (sample > 0x7fff) {
            sample = 0x7fff;
        }
    }
    index = index + data_ov013_021b3a04[code];
    if (index < 0) {
        index = 0;
    } else if (index > 88) {
        index = 88;
    }
    state->predictor = sample;
    state->stepIndex = index;
    return sample;
}

static inline s16 DecodeSample2(VCTAdpcmState *state, int code)
{
    int sample = state->predictor;
    int index = state->stepIndex;
    int step = data_ov013_021b3a1c[index];
    int diff;

    if (!(code & 1)) {
        step = 0;
    }
    diff = step;
    if (code & 2) {
        sample -= diff;
        if (sample < -0x8000) {
            sample = -0x8000;
        }
    } else {
        sample += diff;
        if (sample > 0x7fff) {
            sample = 0x7fff;
        }
    }
    index = index + data_ov013_021b3a00[code];
    if (index < 0) {
        index = 0;
    } else if (index > 88) {
        index = 88;
    }
    state->predictor = sample;
    state->stepIndex = index;
    return sample;
}

static inline u8 EncodeSample4(VCTAdpcmState *state, int sample)
{
    int step;
    int index;
    int diff = sample - state->predictor;
    int code;

    index = state->stepIndex;
    if (diff < 0) {
        code = 8;
        diff = -diff;
    } else {
        code = 0;
    }
    step = data_ov013_021b3a1c[index];
    if (diff >= step) {
        code |= 4;
        diff -= step;
    }
    if (diff >= step >> 1) {
        code |= 2;
        diff -= step >> 1;
    }
    if (diff >= step >> 2) {
        code |= 1;
    }
    diff = step >> 3;
    if (code & 4) {
        diff += step;
    }
    if (code & 2) {
        diff += step >> 1;
    }
    if (code & 1) {
        diff += step >> 2;
    }
    if (code & 8) {
        diff = -diff;
    }
    sample = state->predictor + diff;
    if (sample < -0x8000) {
        sample = -0x8000;
    }
    if (sample > 0x7fff) {
        sample = 0x7fff;
    }
    ADVANCE_STEP_INDEX(index, data_ov013_021b3a0c[code]);
    state->predictor = sample;
    state->stepIndex = index;
    return code;
}

static inline u8 EncodeSample3(VCTAdpcmState *state, int sample)
{
    int step;
    int index;
    int diff = sample - state->predictor;
    int code;

    index = state->stepIndex;
    if (diff < 0) {
        code = 4;
        diff = -diff;
    } else {
        code = 0;
    }
    step = data_ov013_021b3a1c[index];
    if (diff >= step) {
        code |= 2;
        diff -= step;
    }
    if (diff >= step >> 1) {
        code |= 1;
    }
    diff = step >> 2;
    if (code & 2) {
        diff += step;
    }
    if (code & 1) {
        diff += step >> 1;
    }
    if (code & 4) {
        diff = -diff;
    }
    sample = state->predictor + diff;
    if (sample < -0x8000) {
        sample = -0x8000;
    }
    if (sample > 0x7fff) {
        sample = 0x7fff;
    }
    ADVANCE_STEP_INDEX(index, data_ov013_021b3a04[code]);
    state->predictor = sample;
    state->stepIndex = index;
    return code;
}

static inline u8 EncodeSample2(VCTAdpcmState *state, int sample)
{
    int step;
    int index;
    int diff = sample - state->predictor;
    int code;

    index = state->stepIndex;
    if (diff < 0) {
        code = 2;
        diff = -diff;
    } else {
        code = 0;
    }
    step = data_ov013_021b3a1c[index];
    if (diff >= step) {
        code |= 1;
    }
    if (!(code & 1)) {
        step = 0;
    }
    diff = step;
    if (code & 2) {
        diff = -diff;
    }
    sample = state->predictor + diff;
    if (sample < -0x8000) {
        sample = -0x8000;
    }
    if (sample > 0x7fff) {
        sample = 0x7fff;
    }
    ADVANCE_STEP_INDEX(index, data_ov013_021b3a00[code]);
    state->predictor = sample;
    state->stepIndex = index;
    return code;
}

// Decodes count bytes of 4-bit codes, two samples to a byte, low nibble first
static void func_ov013_021af340(const u8 *src, s16 *dst, u32 count, VCTAdpcmState *state)
{
    u32 i;

    for (i = 0; i < count; i++) {
        *dst++ = DecodeSample4(state, *src & 0xf);
        *dst++ = DecodeSample4(state, (*src >> 4) & 0xf);
        src++;
    }
}

// Encodes count samples to 4-bit codes
static void func_ov013_021af494(u8 *dst, const s16 *src, u32 count, VCTAdpcmState *state)
{
    u32 i;

    for (i = 0; i < count >> 1; i++) {
        u8 code = EncodeSample4(state, *src++);
        code |= EncodeSample4(state, *src++) << 4;
        *dst++ = code;
    }
}

// Decodes groups of three bytes of 3-bit codes, eight samples to a group, the first sample in the top bits
static void func_ov013_021af628(const u8 *src, s16 *dst, u32 count, VCTAdpcmState *state)
{
    u32 i;

    for (i = 0; i < count / 3; i++) {
        u8 b0 = src[0];
        u8 b1 = src[1];
        u8 b2 = src[2];

        dst[0] = DecodeSample3(state, b0 >> 5);
        dst[1] = DecodeSample3(state, (b0 >> 2) & 7);
        dst[2] = DecodeSample3(state, ((b0 << 1) & 7) | (b1 >> 7));

        dst[3] = DecodeSample3(state, (b1 >> 4) & 7);
        dst[4] = DecodeSample3(state, (b1 >> 1) & 7);
        dst[5] = DecodeSample3(state, ((b1 << 2) & 7) | (b2 >> 6));

        dst[6] = DecodeSample3(state, (b2 >> 3) & 7);
        dst[7] = DecodeSample3(state, b2 & 7);

        src += 3;
        dst += 8;
    }
}

// Encodes count samples to 3-bit codes
static void func_ov013_021afabc(u8 *dst, s16 *src, u32 count, VCTAdpcmState *state)
{
    u32 i;

    for (i = 0; i < count >> 3; i++) {
        u8 code = EncodeSample3(state, *src++) << 5;
        u8 tmp0;
        u8 tmp1;

        code |= EncodeSample3(state, *src++) << 2;
        tmp0 = EncodeSample3(state, *src++);
        *dst++ = code | (tmp0 >> 1);

        code = EncodeSample3(state, *src++) << 4;
        code |= EncodeSample3(state, *src++) << 1;
        tmp1 = EncodeSample3(state, *src++);
        *dst++ = code | (tmp0 << 7) | (tmp1 >> 2);

        code = EncodeSample3(state, *src++) << 3;
        code |= EncodeSample3(state, *src++);
        *dst++ = code | (tmp1 << 6);
    }
}

// Decodes count bytes of 2-bit codes, four samples to a byte, the first in the low bits
static void func_ov013_021affe4(const u8 *src, s16 *dst, u32 count, VCTAdpcmState *state)
{
    u32 i;

    for (i = 0; i < count; i++) {
        *dst++ = DecodeSample2(state, *src & 3);
        *dst++ = DecodeSample2(state, (*src >> 2) & 3);
        *dst++ = DecodeSample2(state, (*src >> 4) & 3);
        *dst++ = DecodeSample2(state, (*src >> 6) & 3);
        src++;
    }
}

// Encodes count samples to 2-bit codes
static void func_ov013_021b01fc(u8 *dst, s16 *src, u32 count, VCTAdpcmState *state)
{
    u32 i;

    for (i = 0; i < count >> 2; i++) {
        u8 code = EncodeSample2(state, *src++);
        code |= EncodeSample2(state, *src++) << 2;
        code |= EncodeSample2(state, *src++) << 4;
        code |= EncodeSample2(state, *src++) << 6;
        *dst++ = code;
    }
}

// Decodes a packet of ADPCM: the state it starts from, then bytes of codes at the given number of bits a sample
void func_ov013_021b043c(u8 *packet, s16 *dst, int size, int bits)
{
    VCTAdpcmState state;

    state.predictor = ((const VCTAdpcmState *)packet)->predictor;
    state.stepIndex = ((const VCTAdpcmState *)packet)->stepIndex;
    switch (bits) {
    case 2:
        func_ov013_021affe4(packet + 4, dst, size - 4, &state);
        break;
    case 3:
        func_ov013_021af628(packet + 4, dst, size - 4, &state);
        break;
    case 4:
        func_ov013_021af340(packet + 4, dst, size - 4, &state);
        break;
    }
}

// Encodes count samples to a packet of ADPCM, which starts with the state before the first sample
void func_ov013_021b04a8(u8 *packet, const s16 *src, int count, VCTAdpcmState *state, int bits)
{
    VCTAdpcmState *header = (VCTAdpcmState *)packet;

    header->predictor = state->predictor;
    header->stepIndex = state->stepIndex;
    header->pad = 0;
    switch (bits) {
    case 2:
        func_ov013_021b01fc(packet + 4, src, count, state);
        break;
    case 3:
        func_ov013_021afabc(packet + 4, src, count, state);
        break;
    case 4:
        func_ov013_021af494(packet + 4, src, count, state);
        break;
    }
}
