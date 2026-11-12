// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov080/no_gear.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "field/no_gear.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "nitro/gx.h"
#include "save/player_info.h"
#include "system/game_data.h"
#include "system/game_system.h"

typedef void (*NoGearFunc)(NoGearWork *work);

struct NoGearWork {
    // Called each frame, once set
    NoGearFunc func;
    u32 unk_04;
    u16 heapId;
    u32 unk_0c;
    u32 unk_10;
    // The characters of BG 6, as GFL_BGSysLoadArcNCGRDynamic returns them
    u32 chars;
    GameSystem *gsys;
    u32 unk_1c;
};

static void func_ov030_021efec0(NoGearWork *work, NoGearFunc func, int line);
static void func_ov030_021efec4(NoGearWork *work, NoGearFunc func, int line);
static void func_ov030_021efecc(NoGearWork *work);
static void func_ov030_021eff54(void);
void func_ov030_021f0058(NoGearWork *work);
static void func_ov030_021f007c(NoGearWork *work);
static void func_ov030_021f00c4(NoGearWork *work);

// The palette and the characters, by the player's gender
static const u32 data_ov030_021f0134[3] = { 13, 6, 13 };
static const u32 data_ov030_021f0140[3] = { 14, 7, 14 };

static const BGSetup data_ov030_021f014c = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, 0x6000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

static const BGSetup data_ov030_021f016c = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, 0x6000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

static const BGSetup data_ov030_021f018c = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, 0x6000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

// The line is unused
// pokebw2's NoGear_SetFuncCore
static void func_ov030_021efec0(NoGearWork *work, NoGearFunc func, int line) {
    work->func = func;
}

// pokebw2's NoGear_SetFunc
static void func_ov030_021efec4(NoGearWork *work, NoGearFunc func, int line) {
    func_ov030_021efec0(work, func, line);
}

// pokebw2's NoGear_LoadGraphics
static void func_ov030_021efecc(NoGearWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x1e, work->heapId);
    u32 gender = getTrainerGender(GetGameDataPlayerInfo(GSYS_GetGameData(work->gsys)));

    GFL_G2DIOLoadArcNCLRDefault(arc, data_ov030_021f0134[gender], 4, 0, 0, work->heapId);
    work->chars = GFL_BGSysLoadArcNCGRDynamic(arc, data_ov030_021f0140[gender], 6, 0, FALSE, work->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 3, 6, 0, (u16)work->chars, 0, FALSE, work->heapId);
    GFL_ArcToolFree(arc);
    func_0203d34c(FALSE, work->heapId);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's NoGear_SetupBGs
static void func_ov030_021eff54(void) {
    int bg;

    for (bg = 4; bg <= 7; bg++) {
        GFL_BGSysSetBGEnabled(bg, FALSE);
    }

    {
        BGSetup setup = data_ov030_021f014c;

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysSetBGPriority(6, 3);
        GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = data_ov030_021f016c;

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(5, TRUE);
        GFL_BGSysSetBGPriority(5, 1);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(5);
    }
    {
        BGSetup setup = data_ov030_021f018c;

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysSetBGPriority(4, 2);
        GFL_BGSysFillScrArea(4, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(4);
    }
}

// pokebw2's NoGear_Init
void func_ov030_021f0058(NoGearWork *work) {
    func_ov030_021eff54();
    func_ov030_021efecc(work);
    func_ov030_021efec4(work, func_ov030_021f00c4, 329);
}

// pokebw2's NoGear_Release
static void func_ov030_021f007c(NoGearWork *work) {
    GFL_BGSysFreeCharMemory(6, (u16)work->chars, (u16)(work->chars >> 16));
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
}

// pokebw2's NoGear_Idle
static void func_ov030_021f00c4(NoGearWork *work) {
}

#define HEAPID_FIELDMAP 0x15
NoGearWork *func_ov030_021f00c8(void *saveData, FieldSubscreen *subscreen, GameSystem *gsys) {
    NoGearWork *work = GFL_HeapAllocate(HEAPID_FIELDMAP, sizeof(NoGearWork), TRUE, "no_gear.c", 378);

    work->heapId = HEAPID_FIELDMAP;
    work->gsys = gsys;
    func_ov030_021f0058(work);
    return work;
}

void func_ov030_021f00f8(NoGearWork *work) {
    if (work->func != NULL) {
        work->func(work);
    }
    // The connection state is fetched but not used
    func_0201058c(func_0201283c(GSYS_GetGameData(work->gsys)));
}

void func_ov030_021f0114(NoGearWork *work) {
}

void func_ov030_021f0118(NoGearWork *work) {
    func_ov030_021f007c(work);
    G2S_BlendNone();
    GFL_HeapFree(work);
}
