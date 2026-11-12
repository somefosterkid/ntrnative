// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ima_adpcm.h.
#ifndef BWPORT_APP_COMM_TVT_IMA_ADPCM_H
#define BWPORT_APP_COMM_TVT_IMA_ADPCM_H

#include "types.h"

// IMA ADPCM for the video chat's voice. Each direction keeps its own state, which the reset functions clear

// pokebw2's Adpcm_ResetEncoder and Adpcm_ResetDecoder
void func_ov172_021b69e0(void);
void func_ov172_021b69f0(void);
// Encodes size bytes of 16-bit samples into dst, and returns the size of the encoded data (pokebw2's Adpcm_Encode)
u32 func_ov172_021b6a00(const s16 *src, u32 size, u8 *dst);
// Decodes size bytes of encoded data into dst, and returns the size of the samples (pokebw2's Adpcm_Decode)
u32 func_ov172_021b6a18(const s8 *src, u32 size, s16 *dst);

#endif // BWPORT_APP_COMM_TVT_IMA_ADPCM_H
