// Black's code at main 0x0207f7a0-0x0207fd34. No name is known: the module and address.

#include "types.h"
#include "stddef.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "twl/mi.h"

extern u32 GXi_DmaId;

#pragma thumb off

// The texture, palette and clear image upload state, at data_02150e2c: NitroSDK's gx_load3d.c statics (sTex and
// the others), one object as -ipa file pools them: the banks taken from the 3D engine while they load and the LCDC
// addresses they are written at
typedef struct {
    GXVRamClearImage clrImg;
    u8 *texLCDCBlk1;
    u8 *texPlttLCDCBlk;
    GXVRamTexPltt texPltt;
    u8 *clrImgLCDCBlk;
    GXVRamTex tex;
    // The second LCDC block of a texture slot that spans two banks, and the size of the first
    u8 *texLCDCBlk2;
    u32 szTexBlk1;
} GfxTexPltUpload;

GfxTexPltUpload data_02150e2c;

const u16 data_020a5c3c[8] = { 0x0000, 0x6880, 0x6890, 0x6880, 0x6894, 0x0000, 0x6890, 0x6880 };

const u16 data_020a5c4c[16][3] = {
#include "asset/main_0207f7a0_data_020a5c4c.inc"
};

void GX_BeginLoadTex(void) {
    u32 bank = GX_ResetBankForTex();

    data_02150e2c.tex = bank;
    data_02150e2c.texLCDCBlk1 = (u8 *)(uintptr_t)(data_020a5c4c[bank][0] << 12);
    data_02150e2c.texLCDCBlk2 = (u8 *)(uintptr_t)(data_020a5c4c[bank][1] << 12);
    data_02150e2c.szTexBlk1 = data_020a5c4c[bank][2] << 12;
}

static inline void gfxCopyFar(const void *src, u8 *dst, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x30) {
        if (GXi_DmaId > 3) {
            MI_NDmaCopy(GXi_DmaId - 4, src, dst, size);
        } else {
            MIi_DmaCopy32(GXi_DmaId, src, dst, size, 1);
        }
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

static inline void gfxCopyNear(const void *src, u8 *dst, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_NDmaCopyAsync(GXi_DmaId - 4, src, dst, size, 0, 0);
        } else {
            MIi_DmaCopy32Async(GXi_DmaId, src, dst, size, 0, 0, 1);
        }
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GX_LoadTex(const void *src, u32 offset, u32 size) {
    u8 *blk2 = data_02150e2c.texLCDCBlk2;
    u8 *dst;

    if (blk2 == NULL) {
        dst = data_02150e2c.texLCDCBlk1 + offset;
    } else if (offset + size < data_02150e2c.szTexBlk1) {
        dst = data_02150e2c.texLCDCBlk1 + offset;
    } else if (offset >= data_02150e2c.szTexBlk1) {
        dst = blk2 + offset - data_02150e2c.szTexBlk1;
    } else {
        u32 size1 = data_02150e2c.szTexBlk1 - offset;

        gfxCopyFar(src, data_02150e2c.texLCDCBlk1 + offset, size1);
        gfxCopyNear((const u8 *)src + size1, blk2, size - size1);
        return;
    }
    gfxCopyNear(src, dst, size);
}


void GX_EndLoadTex(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_WaitNDma(GXi_DmaId - 4);
        } else {
            MI_WaitDma(GXi_DmaId);
        }
    }
    GX_SetBankForTex(data_02150e2c.tex);
    data_02150e2c.szTexBlk1 = 0;
    data_02150e2c.texLCDCBlk2 = NULL;
    data_02150e2c.texLCDCBlk1 = NULL;
    data_02150e2c.tex = 0;
}



void GX_BeginLoadTexPltt(void) {
    s32 bank = GX_ResetBankForTexPltt();

    data_02150e2c.texPltt = bank;
    data_02150e2c.texPlttLCDCBlk = (u8 *)(uintptr_t)(data_020a5c3c[bank >> 4] << 12);
}

void GX_LoadTexPltt(const void *src, u32 offset, u32 size) {
    u8 *dst = data_02150e2c.texPlttLCDCBlk + offset;

    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_NDmaCopyAsync(GXi_DmaId - 4, src, dst, size, 0, 0);
        } else {
            MIi_DmaCopy32Async(GXi_DmaId, src, dst, size, 0, 0, 1);
        }
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}


void GX_EndLoadTexPltt(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_WaitNDma(GXi_DmaId - 4);
        } else {
            MI_WaitDma(GXi_DmaId);
        }
    }
    GX_SetBankForTexPltt(data_02150e2c.texPltt);
    data_02150e2c.texPltt = 0;
    data_02150e2c.texPlttLCDCBlk = NULL;
}

void GX_BeginLoadClearImage(void) {
    u32 bank = GX_ResetBankForClearImage();

    data_02150e2c.clrImg = bank;
    switch (bank) {
    case 2:
    case 3:
        data_02150e2c.clrImgLCDCBlk = (u8 *)HW_LCDC_VRAM_A;
        break;
    case 8:
    case 12:
        data_02150e2c.clrImgLCDCBlk = (u8 *)HW_LCDC_VRAM_C;
        break;
    case 1:
        data_02150e2c.clrImgLCDCBlk = (u8 *)HW_LCDC_VRAM_A - 0x20000;
        break;
    case 4:
        data_02150e2c.clrImgLCDCBlk = (u8 *)HW_LCDC_VRAM_C - 0x20000;
        break;
    }
}

void GX_LoadClearImageColor(const void *src, u32 size) {
    u8 *dst = data_02150e2c.clrImgLCDCBlk;

    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_NDmaCopyAsync(GXi_DmaId - 4, src, dst, size, 0, 0);
        } else {
            MIi_DmaCopy32Async(GXi_DmaId, src, dst, size, 0, 0, 1);
        }
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GX_LoadClearImageDepth(const void *src, u32 size) {
    u8 *dst = data_02150e2c.clrImgLCDCBlk + 0x20000;

    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_NDmaCopyAsync(GXi_DmaId - 4, src, dst, size, 0, 0);
        } else {
            MIi_DmaCopy32Async(GXi_DmaId, src, dst, size, 0, 0, 1);
        }
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GX_EndLoadClearImage(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_WaitNDma(GXi_DmaId - 4);
        } else {
            MI_WaitDma(GXi_DmaId);
        }
    }
    GX_SetBankForClearImage(data_02150e2c.clrImg);
    data_02150e2c.clrImg = 0;
    data_02150e2c.clrImgLCDCBlk = NULL;
}

#pragma thumb on
