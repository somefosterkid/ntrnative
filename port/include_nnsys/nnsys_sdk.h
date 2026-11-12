// The NitroSDK names our lib/nnsys headers use (pokebw2's lib/nitro spellings) that libntr spells otherwise or lacks.
// Included by port/include_nnsys/nnsys.h before the five NitroSystem headers, so the game's files see them too.
// Each one is the DS definition (lib/nitro/include/nitro/*.h), restated: none changes a layout libntr has.
#ifndef BWPORT_NNSYS_SDK_H
#define BWPORT_NNSYS_SDK_H

#include <nitro.h>

// lib/nitro fx.h: a 2D affine matrix, its last row the translation (NitroSystem's g2d)
typedef struct {
    fx32 _00;
    fx32 _01;
    fx32 _10;
    fx32 _11;
    fx32 _20;
    fx32 _21;
} MtxFx32;

// lib/nitro gx.h: OAM attribute 0's shape bits (libntr: GX_OAM_ATTR01_SHAPE_MASK, the same bits of attr01)
#ifndef GX_OAM_ATTR0_SHAPE_MASK
#define GX_OAM_ATTR0_SHAPE_MASK 0x0000c000
#endif

// lib/nitro gx.h: a fill through the game's GX DMA channel, or the CPU when there is none
static inline void GXi_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size) {
    if (dmaNo > MI_DMA_MAX_NUM) {
        dmaNo = GX_DMA_NOT_USE;
    }
    if (dmaNo != GX_DMA_NOT_USE) {
        MIi_DmaFill32(dmaNo, dest, data, size, TRUE);
    } else {
        MIi_CpuClear32(data, dest, size);
    }
}

// lib/nitro snd.h: a channel's volume byte and data shift of SND_CalcChannelVolume's result
#ifndef SND_CHANNEL_VOLUME
#define SND_CHANNEL_VOLUME(v) ((u8)(v))
#endif
#ifndef SND_CHANNEL_DATASHIFT
#define SND_CHANNEL_DATASHIFT(v) ((v) >> 8)
#endif

// lib/nitro snd.h: a sequence file (SSEQ): the file and block headers, then the offset of its data (no pointers)
typedef struct SNDSequenceData {
    SNDBinaryFileHeader fileHeader;
    SNDBinaryBlockHeader blockHeader;
    u32 baseOffset;
    u32 data[];
} SNDSequenceData;

#endif
