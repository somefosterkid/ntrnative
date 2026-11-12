// Black's code at main 0x0207e3d0-0x0207fd34. No name is known: the module and address.

#include "types.h"
#include "stddef.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "twl/mi.h"

extern u32 GXi_DmaId;

#pragma thumb off

// Black's extended palette upload state, at data_02150e10: NitroSDK's gx_load.c statics (sBGExtPltt and the
// others), one object as -ipa file pools them: the banks taken from the extended palettes while they load, and the
// LCDC address the main engine's BG and OBJ ones are written at
typedef struct {
    GXVRamSubBGExtPltt subBGExtPltt;
    u8 *objExtPlttLCDCBlk;
    GXVRamOBJExtPltt objExtPltt;
    // What the BG extended palette's first slot is at in its LCDC bank (slot 2 is at the start of bank G)
    u32 bgExtPlttLCDCOffset;
    u8 *bgExtPlttLCDCBlk;
    GXVRamBGExtPltt bgExtPltt;
    GXVRamSubOBJExtPltt subOBJExtPltt;
} GfxExtPltUpload;

GfxExtPltUpload data_02150e10;

void * G2S_GetBG0ScrPtr(void);
void * G2S_GetBG1ScrPtr(void);
void * G2S_GetBG2ScrPtr(void);
void * G2S_GetBG3ScrPtr(void);

void GX_LoadBGPltt(const void *src, u32 offset, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | ((uintptr_t)HW_BG_PLTT + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_BG_PLTT + offset, size);
            } else {
                MIi_CpuCopy16(src, (u8 *)HW_BG_PLTT + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, (u8 *)HW_BG_PLTT + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, (u8 *)HW_BG_PLTT + offset, size);
    }
}

void GXS_LoadBGPltt(const void *src, u32 offset, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | ((uintptr_t)HW_DB_BG_PLTT + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_DB_BG_PLTT + offset, size);
            } else {
                MIi_CpuCopy16(src, (u8 *)HW_DB_BG_PLTT + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, (u8 *)HW_DB_BG_PLTT + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, (u8 *)HW_DB_BG_PLTT + offset, size);
    }
}

void GX_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | ((uintptr_t)HW_OBJ_PLTT + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_OBJ_PLTT + offset, size);
            } else {
                MIi_CpuCopy16(src, (u8 *)HW_OBJ_PLTT + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, (u8 *)HW_OBJ_PLTT + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, (u8 *)HW_OBJ_PLTT + offset, size);
    }
}

void GXS_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | ((uintptr_t)HW_DB_OBJ_PLTT + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_DB_OBJ_PLTT + offset, size);
            } else {
                MIi_CpuCopy16(src, (u8 *)HW_DB_OBJ_PLTT + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, (u8 *)HW_DB_OBJ_PLTT + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, (u8 *)HW_DB_OBJ_PLTT + offset, size);
    }
}


void GX_LoadOAM(const void *src, u32 offset, u32 size) {
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_OAM + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)HW_OAM + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, (u8 *)HW_OAM + offset, size);
}
#pragma thumb on

#pragma thumb off
void GXS_LoadOAM(const void *src, u32 offset, u32 size) {
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_DB_OAM + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)HW_DB_OAM + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, (u8 *)HW_DB_OAM + offset, size);
}
#pragma thumb on

#pragma thumb off
void GX_LoadOBJ(const void *src, u32 offset, u32 size) {
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_OBJ_VRAM + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)HW_OBJ_VRAM + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, (u8 *)HW_OBJ_VRAM + offset, size);
}
#pragma thumb on

#pragma thumb off
void GXS_LoadOBJ(const void *src, u32 offset, u32 size) {
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)HW_DB_OBJ_VRAM + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)HW_DB_OBJ_VRAM + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, (u8 *)HW_DB_OBJ_VRAM + offset, size);
}

void GX_LoadBG0Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2_GetBG0ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}

void GXS_LoadBG0Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2S_GetBG0ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}

void GX_LoadBG1Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2_GetBG1ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}

void GXS_LoadBG1Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2S_GetBG1ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}

void GX_LoadBG2Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2_GetBG2ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}

void GXS_LoadBG2Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2S_GetBG2ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}

void GX_LoadBG3Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2_GetBG3ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}

void GXS_LoadBG3Scr(const void *src, u32 offset, u32 size) {
    u8 *scr = G2S_GetBG3ScrPtr();

    if (GXi_DmaId != GX_DMA_NOT_USE && size > 0x1c) {
        if (GXi_DmaId > 3) {
            if ((((uintptr_t)src | (uintptr_t)(scr + offset) | size) & 3) == 0) {
                MI_NDmaCopy(GXi_DmaId - 4, src, scr + offset, size);
            } else {
                MIi_CpuCopy16(src, scr + offset, size);
            }
        } else {
            MIi_DmaCopy16(GXi_DmaId, src, scr + offset, size, 1);
        }
    } else {
        MIi_CpuCopy16(src, scr + offset, size);
    }
}




#pragma thumb on

#pragma thumb off
void GX_LoadBG0Char(const void *src, u32 offset, u32 size) {
    void *bg0CharPtr;

    bg0CharPtr = G2_GetBG0CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg0CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg0CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg0CharPtr + offset), size);
}
#pragma thumb on

#pragma thumb off
void GXS_LoadBG0Char(const void *src, u32 offset, u32 size) {
    void *bg0CharPtr;

    bg0CharPtr = G2S_GetBG0CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg0CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg0CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg0CharPtr + offset), size);
}
#pragma thumb on

#pragma thumb off
void GX_LoadBG1Char(const void *src, u32 offset, u32 size) {
    void *bg1CharPtr;

    bg1CharPtr = G2_GetBG1CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg1CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg1CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg1CharPtr + offset), size);
}
#pragma thumb on

#pragma thumb off
void GXS_LoadBG1Char(const void *src, u32 offset, u32 size) {
    void *bg1CharPtr;

    bg1CharPtr = G2S_GetBG1CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg1CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg1CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg1CharPtr + offset), size);
}
#pragma thumb on

#pragma thumb off
void GX_LoadBG2Char(const void *src, u32 offset, u32 size) {
    void *bg2CharPtr;

    bg2CharPtr = G2_GetBG2CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg2CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg2CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg2CharPtr + offset), size);
}
#pragma thumb on

#pragma thumb off
void GXS_LoadBG2Char(const void *src, u32 offset, u32 size) {
    void *bg2CharPtr;

    bg2CharPtr = G2S_GetBG2CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg2CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg2CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg2CharPtr + offset), size);
}
#pragma thumb on

#pragma thumb off
void GX_LoadBG3Char(const void *src, u32 offset, u32 size) {
    void *bg3CharPtr;

    bg3CharPtr = G2_GetBG3CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg3CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg3CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg3CharPtr + offset), size);
}
#pragma thumb on

#pragma thumb off
void GXS_LoadBG3Char(const void *src, u32 offset, u32 size) {
    void *bg3CharPtr;

    bg3CharPtr = G2S_GetBG3CharPtr();
    if ((GXi_DmaId != GX_DMA_NOT_USE) && (0x30 < size)) {
        if (3 < GXi_DmaId) {
            MI_NDmaCopy(GXi_DmaId - 4, src, (u8 *)bg3CharPtr + offset, size);
            return;
        }
        MIi_DmaCopy32(GXi_DmaId, src, (u8 *)bg3CharPtr + offset, size, 1);
        return;
    }
    MIi_CpuCopy32(src, ((u8 *)bg3CharPtr + offset), size);
}

void GX_BeginLoadBGExtPltt(void) {
    GXVRamBGExtPltt bank = GX_ResetBankForBGExtPltt();

    data_02150e10.bgExtPltt = bank;
    switch (bank) {
    case GX_VRAM_BGEXTPLTT_NONE:
        break;
    case GX_VRAM_BGEXTPLTT_0123_E:
        data_02150e10.bgExtPlttLCDCBlk = (u8 *)HW_LCDC_VRAM_E;
        data_02150e10.bgExtPlttLCDCOffset = 0;
        break;
    case GX_VRAM_BGEXTPLTT_23_G:
        data_02150e10.bgExtPlttLCDCBlk = (u8 *)HW_LCDC_VRAM_G;
        data_02150e10.bgExtPlttLCDCOffset = 0x4000;
        break;
    case GX_VRAM_BGEXTPLTT_01_F:
    case GX_VRAM_BGEXTPLTT_0123_FG:
        data_02150e10.bgExtPlttLCDCBlk = (u8 *)HW_LCDC_VRAM_F;
        data_02150e10.bgExtPlttLCDCOffset = 0;
        break;
    }
}

void GX_LoadBGExtPltt(const void *src, u32 offset, u32 size) {
    u8 *dst = data_02150e10.bgExtPlttLCDCBlk + offset - data_02150e10.bgExtPlttLCDCOffset;

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


void GX_EndLoadBGExtPltt(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_WaitNDma(GXi_DmaId - 4);
        } else {
            MI_WaitDma(GXi_DmaId);
        }
    }
    GX_SetBankForBGExtPltt(data_02150e10.bgExtPltt);
    data_02150e10.bgExtPltt = 0;
    data_02150e10.bgExtPlttLCDCBlk = NULL;
    data_02150e10.bgExtPlttLCDCOffset = 0;
}


void GX_BeginLoadOBJExtPltt(void) {
    GXVRamOBJExtPltt bank = GX_ResetBankForOBJExtPltt();

    data_02150e10.objExtPltt = bank;
    if (bank != GX_VRAM_OBJEXTPLTT_NONE) {
        if (bank != GX_VRAM_OBJEXTPLTT_0_F) {
            if (bank == GX_VRAM_OBJEXTPLTT_0_G) {
                data_02150e10.objExtPlttLCDCBlk = (u8 *)HW_LCDC_VRAM_G;
            }
            return;
        }
        data_02150e10.objExtPlttLCDCBlk = (u8 *)HW_LCDC_VRAM_F;
    }
}

void GX_LoadOBJExtPltt(const void *src, u32 offset, u32 size) {
    u8 *dst = data_02150e10.objExtPlttLCDCBlk + offset;

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


void GX_EndLoadOBJExtPltt(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_WaitNDma(GXi_DmaId - 4);
        } else {
            MI_WaitDma(GXi_DmaId);
        }
    }
    GX_SetBankForOBJExtPltt(data_02150e10.objExtPltt);
    data_02150e10.objExtPltt = 0;
    data_02150e10.objExtPlttLCDCBlk = NULL;
}


void GXS_BeginLoadBGExtPltt(void) {
    data_02150e10.subBGExtPltt = GX_ResetBankForSubBGExtPltt();
}

void GXS_LoadBGExtPltt(const void *src, u32 offset, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_NDmaCopyAsync(GXi_DmaId - 4, src, (u8 *)HW_LCDC_VRAM_H + offset, size, 0, 0);
        } else {
            MIi_DmaCopy32Async(GXi_DmaId, src, (u8 *)HW_LCDC_VRAM_H + offset, size, 0, 0, 1);
        }
    } else {
        MIi_CpuCopy32(src, (u8 *)HW_LCDC_VRAM_H + offset, size);
    }
}


void GXS_EndLoadBGExtPltt(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_WaitNDma(GXi_DmaId - 4);
        } else {
            MI_WaitDma(GXi_DmaId);
        }
    }
    GX_SetBankForSubBGExtPltt(data_02150e10.subBGExtPltt);
    data_02150e10.subBGExtPltt = 0;
}


void GXS_BeginLoadOBJExtPltt(void) {
    data_02150e10.subOBJExtPltt = GX_ResetBankForSubOBJExtPltt();
}

void GXS_LoadOBJExtPltt(const void *src, u32 offset, u32 size) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_NDmaCopyAsync(GXi_DmaId - 4, src, (u8 *)HW_LCDC_VRAM_I + offset, size, 0, 0);
        } else {
            MIi_DmaCopy32Async(GXi_DmaId, src, (u8 *)HW_LCDC_VRAM_I + offset, size, 0, 0, 1);
        }
    } else {
        MIi_CpuCopy32(src, (u8 *)HW_LCDC_VRAM_I + offset, size);
    }
}


void GXS_EndLoadOBJExtPltt(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        if (GXi_DmaId > 3) {
            MI_WaitNDma(GXi_DmaId - 4);
        } else {
            MI_WaitDma(GXi_DmaId);
        }
    }
    GX_SetBankForSubOBJExtPltt(data_02150e10.subOBJExtPltt);
    data_02150e10.subOBJExtPltt = 0;
}

#pragma thumb on
