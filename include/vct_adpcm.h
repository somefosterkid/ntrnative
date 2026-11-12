#ifndef VCT_ADPCM_H
#define VCT_ADPCM_H

// lib/dwc/src/vct/vct_adpcm.c: decodes one frame of ADPCM samples into 16-bit samples
void func_ov013_021b043c(u8 *data, s16 *samples, int size, int codec);

struct VCTAdpcmState;

// lib/dwc/src/vct/vct_adpcm.c: encodes one frame of samples as ADPCM
void func_ov013_021b04a8(u8 *packet, const s16 *src, int count, struct VCTAdpcmState *state, int bits);

#endif // VCT_ADPCM_H
