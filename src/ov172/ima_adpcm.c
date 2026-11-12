// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/ima_adpcm.c, which is the same code in
// Black 2/White 2's overlay 257. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "app/comm_tvt/ima_adpcm.h"
#include "types.h"

// IMA ADPCM, which packs the voice of a video chat from 16-bit samples into 4 bits each: the encoder for the
// microphone's samples and the decoder for the other side's. The ROM gives no name for this file: ima_adpcm.c is
// pokebw2's

typedef struct {
    s16 predicted;
    s8 index;
} AdpcmState;

static void func_ov172_021b6a30(const s16 *src, u8 *dst, int count, AdpcmState *state);
static void func_ov172_021b6a88(const s8 *src, s16 *dst, int count, AdpcmState *state);
static s8 func_ov172_021b6adc(s16 sample, AdpcmState *state);
static s16 func_ov172_021b6b68(u8 code, AdpcmState *state);

// pokebw2's sIndexTable
static const s8 data_ov172_021c4d10[16] = {
#include "asset/ima_adpcm_data_ov172_021c4d10.inc"
};

// pokebw2's sStepTable
static const int data_ov172_021c4d20[89] = {
#include "asset/ima_adpcm_data_ov172_021c4d20.inc"
};

// The decoder's state, then the encoder's
enum {
    ADPCM_DECODE,
    ADPCM_ENCODE,
};

// pokebw2's sAdpcm
static AdpcmState data_ov172_021c88e4[2];

// pokebw2's Adpcm_ResetEncoder
void func_ov172_021b69e0(void) {
    data_ov172_021c88e4[ADPCM_ENCODE].predicted = 0;
    data_ov172_021c88e4[ADPCM_ENCODE].index = 0;
}

// pokebw2's Adpcm_ResetDecoder
void func_ov172_021b69f0(void) {
    data_ov172_021c88e4[ADPCM_DECODE].predicted = 0;
    data_ov172_021c88e4[ADPCM_DECODE].index = 0;
}

// pokebw2's Adpcm_Encode
u32 func_ov172_021b6a00(const s16 *src, u32 size, u8 *dst) {
    func_ov172_021b6a30(src, dst, size / 2, &data_ov172_021c88e4[ADPCM_ENCODE]);
    return (size + 3) / 4;
}

// pokebw2's Adpcm_Decode
u32 func_ov172_021b6a18(const s8 *src, u32 size, s16 *dst) {
    func_ov172_021b6a88(src, dst, size * 2, &data_ov172_021c88e4[ADPCM_DECODE]);
    return size * 4;
}

// pokebw2's Adpcm_EncodeSamples
static void func_ov172_021b6a30(const s16 *src, u8 *dst, int count, AdpcmState *state) {
    int i;
    int pos = 0;
    u8 low = 0;
    BOOL haveLow = FALSE;

    for (i = 0; i < count; i++) {
        u8 code = func_ov172_021b6adc(src[i], state);

        if (!haveLow) {
            low = code;
            haveLow = TRUE;
        } else {
            dst[pos] = low + (u8)(code << 4);
            haveLow = FALSE;
            low = 0;
            pos++;
        }
    }
    if (haveLow == TRUE) {
        dst[pos] = low;
    }
}

// pokebw2's Adpcm_DecodeSamples
static void func_ov172_021b6a88(const s8 *src, s16 *dst, int count, AdpcmState *state) {
    int i;

    for (i = 0; i < count / 2; i++) {
        s8 byte = src[i];

        dst[i * 2] = func_ov172_021b6b68(byte & 0xf, state);
        dst[i * 2 + 1] = func_ov172_021b6b68((byte & 0xf0) >> 4, state);
    }
}

// pokebw2's Adpcm_EncodeSample
static s8 func_ov172_021b6adc(s16 sample, AdpcmState *state) {
    int code;
    int diff;
    int delta;
    int step;
    int predicted;
    int index;

    predicted = state->predicted;
    index = state->index;
    step = data_ov172_021c4d20[index];

    diff = sample - predicted;
    if (diff >= 0) {
        code = 0;
    } else {
        code = 8;
        diff = -diff;
    }

    delta = step >> 3;
    if (diff >= step) {
        code |= 4;
        diff -= step;
        delta += step;
    }
    step >>= 1;
    if (diff >= step) {
        code |= 2;
        diff -= step;
        delta += step;
    }
    step >>= 1;
    if (diff >= step) {
        code |= 1;
        delta += step;
    }

    if (code & 8) {
        predicted -= delta;
    } else {
        predicted += delta;
    }
    if (predicted > 32767) {
        predicted = 32767;
    } else if (predicted < -32767) {
        predicted = -32767;
    }

    index += data_ov172_021c4d10[code];
    if (index < 0) {
        index = 0;
    }
    if (index > 88) {
        index = 88;
    }

    state->predicted = predicted;
    state->index = index;
    return code & 0xf;
}

// pokebw2's Adpcm_DecodeSample
static s16 func_ov172_021b6b68(u8 code, AdpcmState *state) {
    int step;
    int predicted;
    int index;
    int delta;

    predicted = state->predicted;
    index = state->index;
    step = data_ov172_021c4d20[index];

    delta = step >> 3;
    if (code & 4) {
        delta += step;
    }
    if (code & 2) {
        delta += step >> 1;
    }
    if (code & 1) {
        delta += step >> 2;
    }

    if (code & 8) {
        predicted -= delta;
        if (predicted < -32767) {
            predicted = -32767;
        }
    } else {
        predicted += delta;
        if (predicted > 32767) {
            predicted = 32767;
        }
    }

    index += data_ov172_021c4d10[code];
    if (index < 0) {
        index = 0;
    } else if (index > 88) {
        index = 88;
    }

    state->predicted = predicted;
    state->index = index;
    return predicted;
}
