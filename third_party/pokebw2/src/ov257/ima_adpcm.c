#include "app/comm_tvt/ima_adpcm.h"
#include "types.h"

// IMA ADPCM, which packs the voice of a video chat from 16-bit samples into 4 bits each: the encoder for the
// microphone's samples and the decoder for the other side's. The ROM gives no name for this file

typedef struct {
    s16 predicted;
    s8 prevIndex;
} AdpcmState;

static void Adpcm_EncodeSamples(const s16 *src, u8 *dst, int count, AdpcmState *state);
static void Adpcm_DecodeSamples(const s8 *src, s16 *dst, int count, AdpcmState *state);
static s8 Adpcm_EncodeSample(s16 sample, AdpcmState *state);
static s16 Adpcm_DecodeSample(u8 code, AdpcmState *state);

static const s8 sIndexTable[16] = {
#include "asset/ima_adpcm_sIndexTable.inc"
};

static const int sStepTable[89] = {
#include "asset/ima_adpcm_sStepTable.inc"
};

// The decoder's state, then the encoder's
enum {
    ADPCM_DECODE,
    ADPCM_ENCODE,
};

static AdpcmState sAdpcm[2];

void Adpcm_ResetEncoder(void) {
    sAdpcm[ADPCM_ENCODE].predicted = 0;
    sAdpcm[ADPCM_ENCODE].prevIndex = 0;
}

void Adpcm_ResetDecoder(void) {
    sAdpcm[ADPCM_DECODE].predicted = 0;
    sAdpcm[ADPCM_DECODE].prevIndex = 0;
}

u32 Adpcm_Encode(const s16 *src, u32 size, u8 *dst) {
    Adpcm_EncodeSamples(src, dst, size / 2, &sAdpcm[ADPCM_ENCODE]);
    return (size + 3) / 4;
}

u32 Adpcm_Decode(const s8 *src, u32 size, s16 *dst) {
    Adpcm_DecodeSamples(src, dst, size * 2, &sAdpcm[ADPCM_DECODE]);
    return size * 4;
}

static void Adpcm_EncodeSamples(const s16 *src, u8 *dst, int count, AdpcmState *state) {
    int i;
    int pos = 0;
    u8 low = 0;
    BOOL haveLow = FALSE;

    for (i = 0; i < count; i++) {
        u8 code = Adpcm_EncodeSample(src[i], state);

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

static void Adpcm_DecodeSamples(const s8 *src, s16 *dst, int count, AdpcmState *state) {
    int i;

    for (i = 0; i < count / 2; i++) {
        s8 byte = src[i];

        dst[i * 2] = Adpcm_DecodeSample(byte & 0xf, state);
        dst[i * 2 + 1] = Adpcm_DecodeSample((byte & 0xf0) >> 4, state);
    }
}

static s8 Adpcm_EncodeSample(s16 sample, AdpcmState *state) {
    int code;
    int diff;
    int delta;
    int step;
    int predicted;
    int index;

    predicted = state->predicted;
    index = state->prevIndex;
    step = sStepTable[index];

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

    index += sIndexTable[code];
    if (index < 0) {
        index = 0;
    }
    if (index > 88) {
        index = 88;
    }

    state->predicted = predicted;
    state->prevIndex = index;
    return code & 0xf;
}

static s16 Adpcm_DecodeSample(u8 code, AdpcmState *state) {
    int step;
    int predicted;
    int index;
    int delta;

    predicted = state->predicted;
    index = state->prevIndex;
    step = sStepTable[index];

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

    index += sIndexTable[code];
    if (index < 0) {
        index = 0;
    } else if (index > 88) {
        index = 88;
    }

    state->predicted = predicted;
    state->prevIndex = index;
    return predicted;
}
