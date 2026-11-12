// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/gx_layers.c, which is the same code in
// Black 2/White 2.
#include "types.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "nitro/memorymap.h"
#include "nitro/mi.h"

// The planes shown on each engine, GX_PLANEMASK_*, kept so that one can be shown or hidden without the others
static struct {
    u32 sub;
    u32 main;
} data_02146a48;

void GFL_BGSysInitVRAM(u32 banks) {
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL & (banks ^ 0xffff));
    MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    GX_DisableBankForLCDC();
    MIi_CpuClearFast(0xc0, (void *)HW_OAM, HW_OAM_SIZE);
    MIi_CpuClearFast(0, (void *)HW_BG_PLTT, HW_PLTT_SIZE);
    MIi_CpuClearFast(0xc0, (void *)HW_DB_OAM, HW_OAM_SIZE);
    MIi_CpuClearFast(0, (void *)HW_DB_BG_PLTT, HW_PLTT_SIZE);
}

void GFL_BGSysSetVRAMBanks(const BGSysVRAMConfig *config) {
    GX_ResetBankForBG();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForSubBG();
    GX_ResetBankForSubBGExtPltt();
    GX_ResetBankForOBJ();
    GX_ResetBankForOBJExtPltt();
    GX_ResetBankForSubOBJ();
    GX_ResetBankForSubOBJExtPltt();
    GX_ResetBankForTex();
    GX_ResetBankForTexPltt();
    GX_SetBankForBG(config->bgMain);
    GX_SetBankForBGExtPltt(config->bgExtPaletteMain);
    GX_SetBankForSubBG(config->bgSub);
    GX_SetBankForSubBGExtPltt(config->bgExtPaletteSub);
    GX_SetBankForOBJ(config->objMain);
    GX_SetBankForOBJExtPltt(config->objExtPaletteMain);
    GX_SetBankForSubOBJ(config->objSub);
    GX_SetBankForSubOBJExtPltt(config->objExtPaletteSub);
    GX_SetBankForTex(config->texture);
    GX_SetBankForTexPltt(config->texturePalette);
    GX_SetOBJVRamModeChar(config->objMappingMain);
    GXS_SetOBJVRamModeChar(config->objMappingSub);
}

void GFL_BGSysDisableBGsA(void) {
    data_02146a48.main &= GX_PLANEMASK_OBJ;
}

void GFL_BGSysDisableOBJA(void) {
    data_02146a48.main &= GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3;
}

void GFL_BGSysDisableAllA(void) {
    GFL_BGSysDisableBGsA();
    GFL_BGSysDisableOBJA();
}

void GFL_BGSysSetBGEnabledA(u32 planes, BOOL enabled) {
    if (enabled == TRUE) {
        if (data_02146a48.main & planes) {
            return;
        }
    } else {
        if (!(data_02146a48.main & planes)) {
            return;
        }
    }
    data_02146a48.main ^= planes;
    GX_SetVisiblePlane(data_02146a48.main);
}

void GFL_BGSysSetEnabledBGsA(u32 enabled) {
    data_02146a48.main = enabled;
    GX_SetVisiblePlane(data_02146a48.main);
}

void GFL_BGSysDisableBGsB(void) {
    data_02146a48.sub &= GX_PLANEMASK_OBJ;
}

void GFL_BGSysDisableOBJB(void) {
    data_02146a48.sub &= GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3;
}

void GFL_BGSysDisableAllB(void) {
    GFL_BGSysDisableBGsB();
    GFL_BGSysDisableOBJB();
}

void GFL_BGSysSetBGEnabledB(u32 planes, BOOL enabled) {
    if (enabled == TRUE) {
        if (data_02146a48.sub & planes) {
            return;
        }
    } else {
        if (!(data_02146a48.sub & planes)) {
            return;
        }
    }
    data_02146a48.sub ^= planes;
    GXS_SetVisiblePlane(data_02146a48.sub);
}

void GFL_BGSysSetEnabledBGsB(u32 enabled) {
    data_02146a48.sub = enabled;
    GXS_SetVisiblePlane(data_02146a48.sub);
}

void GFL_BGSysEnableEngines(void) {
    GX_DispOn();
    GXS_DispOn();
}

void GFL_BGSysSetDisplayLayout(u32 layout) {
    GX_SetDispSelect(layout);
}

u32 GFL_BGSysGetEnabledBGsA(void) {
    return data_02146a48.main;
}

u32 GFL_BGSysGetEnabledBGsB(void) {
    return data_02146a48.sub;
}
