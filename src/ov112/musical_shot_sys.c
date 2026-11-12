// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/musical_shot_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_shot_info.h"
#include "app/musical/mus_shot_photo.h"
#include "app/musical/musical_shot_sys.h"
#include "constants/species.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/rtc.h"
#include "save/save_control.h"
#include "system/wipe.h"
#include "gfl/rtc_cache.h"
#include "save/musical_savedata.h"

#define HEAPID_MUSICAL_SHOT 0x4a
extern u32 OVERLAY_211_ID[];
extern u32 OVERLAY_210_ID[];

// Overlay 209's musical_shot_sys.c: the proc that shows a musical's photo, with its information on the touch screen

typedef struct {
    HeapID heapId;
    TCB *vblankTcb;
    MusicalShotParam *param;
    MusShotPhoto *photo;
    MusShotInfo *info;
} MusicalShotSys;

static BOOL func_ov112_021f36c0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov112_021f3a08(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov112_021f3970(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov112_021f3a4c(TCB *tcb, void *data);
static void func_ov112_021f3a54(MusicalShotSys *sys);
static void func_ov112_021f3ae0(MusicalShotSys *sys);

// pokebw2's MUSICAL_SHOT_PROC_FUNCTIONS
GameProcFunctions data_ov112_021f8360 = {
    func_ov112_021f36c0,
    func_ov112_021f3a08,
    func_ov112_021f3970,
};

static const BGSysLCDConfig data_ov112_021f815c = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

// The VRAM banks, and the value that follows them in the data (nothing refers to it; it keeps the object's order)
static const struct {
    BGSysVRAMConfig config;
    fx32 unk_30;
} data_ov112_021f816c = {
    {
        GX_VRAM_BG_128_D,    GX_VRAM_BGEXTPLTT_23_G,  GX_VRAM_SUB_BG_32_H,        GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_16_F,    GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_012_ABC, GX_VRAM_TEXPLTT_0123_E,  GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
    },
    FX32_CONST(51.2),
};

// pokebw2's MusicalShotSys_Init
static BOOL func_ov112_021f36c0(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalShotParam *shotParam = param;
    MusicalShotSys *sys;
    MusicalShot *shot;
    RTCDate date;
    u8 i, j;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MUSICAL_SHOT, 0x80000);
    sys = GFL_ProcInitSubsystem(proc, sizeof(MusicalShotSys), HEAPID_MUSICAL_SHOT);
    sys->heapId = HEAPID_MUSICAL_SHOT;
    if (shotParam == NULL) {
        GFL_OvlLoad(OVERLAY_ID(132));
        sys->param = GFL_HeapAllocate(HEAPID_MUSICAL_SHOT, sizeof(MusicalShotParam), FALSE, "musical_shot_sys.c", 110);
        sys->param->shot = GFL_HeapAllocate(HEAPID_MUSICAL_SHOT, sizeof(MusicalShot), TRUE, "musical_shot_sys.c", 111);
        sys->param->loadComm = FALSE;
        shot = sys->param->shot;
        RTC_GetCachedDate(&date);
        shot->unk0_0 = 3;
        shot->tops = 2;
        shot->year = date.year;
        shot->month = date.month;
        shot->day = date.day;
        // The default title
        shot->title[0] = 0x30dd;
        shot->title[1] = 0x30b1;
        shot->title[2] = 0x30c3;
        shot->title[3] = 0x30bf;
        shot->title[4] = 0x30fc;
        shot->title[5] = 0x30ea;
        shot->title[6] = 0x30e2;
        shot->title[7] = 0x30f3;
        shot->title[8] = 0x30b9;
        shot->title[9] = 0x30bf;
        shot->title[10] = 0x30fc;
        shot->title[11] = 0x30ea;
        shot->title[12] = GFL_StrBufGetTerminator();
        shot->pokes[0].species = SPECIES_RESHIRAM;
        shot->pokes[1].species = SPECIES_RESHIRAM;
        shot->pokes[2].species = SPECIES_RESHIRAM;
        shot->pokes[3].species = SPECIES_RESHIRAM;
        for (i = 0; i < 4; i++) {
            shot->pokes[i].name[0] = 0x30c8;
            shot->pokes[i].name[1] = 0x30ec;
            shot->pokes[i].name[2] = 0x30fc;
            shot->pokes[i].name[3] = 0x30ca;
            shot->pokes[i].name[4] = 0xff11 + i;
            shot->pokes[i].name[5] = 0;
            for (j = 0; j < 8; j++) {
                shot->pokes[i].equips[j].itemId = 0xff;
                shot->pokes[i].equips[j].unk2 = 0;
                shot->pokes[i].equips[j].unk4 = 10;
            }
            shot->pokes[i].equips[0].itemId = 11;
            shot->pokes[i].equips[0].unk2 = 0;
            shot->pokes[i].equips[0].unk4 = 7;
            shot->pokes[i].equips[1].itemId = 11;
            shot->pokes[i].equips[1].unk2 = 0;
            shot->pokes[i].equips[1].unk4 = 8;
            shot->pokes[i].equips[2].itemId = 11;
            shot->pokes[i].equips[2].unk2 = 0;
            shot->pokes[i].equips[2].unk4 = 6;
            shot->pokes[i].equips[3].itemId = 11;
            shot->pokes[i].equips[3].unk2 = 0;
            shot->pokes[i].equips[3].unk4 = 4;
            shot->pokes[i].equips[4].itemId = 11;
            shot->pokes[i].equips[4].unk2 = 0;
            shot->pokes[i].equips[4].unk4 = 2;
            shot->pokes[i].equips[5].itemId = 11;
            shot->pokes[i].equips[5].unk2 = 0;
            shot->pokes[i].equips[5].unk4 = 1;
            shot->pokes[i].equips[6].itemId = 11;
            shot->pokes[i].equips[6].unk2 = 0;
            shot->pokes[i].equips[6].unk4 = 0;
        }
        if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_R) {
            sys->param->askSave = TRUE;
        } else {
            sys->param->askSave = FALSE;
        }
        sys->param->save = NULL;
    } else {
        sys->param = shotParam;
    }
    if (sys->param->loadComm == TRUE) {
        GFL_OvlLoad(OVERLAY_ID(132));
    }
    sys->vblankTcb = GFL_VBlankTCBAdd(func_ov112_021f3a4c, sys, 0x40);
    func_ov112_021f3a54(sys);
    sys->photo = func_ov112_021f3af0(sys->param->shot, sys->heapId);
    sys->info =
        func_ov112_021f4284(sys->param->shot, sys->param->save, sys->param->askSave, sys->param->comm, sys->heapId);
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, sys->heapId);
    return TRUE;
}

// pokebw2's MusicalShotSys_Exit
static BOOL func_ov112_021f3970(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalShotSys *sys = work;

    if (GFL_WipeIsFinished() == FALSE) {
        return FALSE;
    }
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    G2_SetBlendAlpha(0, 0, 31, 31);
    func_ov112_021f4300(sys->info);
    func_ov112_021f3bf0(sys->photo);
    func_ov112_021f3ae0(sys);
    GFL_TCBRemove(sys->vblankTcb);
    if (sys->param->loadComm == TRUE) {
        GFL_OvlUnload(OVERLAY_ID(132));
    }
    if (param == NULL) {
        GFL_HeapFree(sys->param->shot);
        GFL_HeapFree(sys->param);
        GFL_OvlUnload(OVERLAY_ID(132));
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_MUSICAL_SHOT);
    return TRUE;
}

// pokebw2's MusicalShotSys_Main
static BOOL func_ov112_021f3a08(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalShotSys *sys = work;

    func_ov112_021f3c50(sys->photo);
    func_ov112_021f4350(sys->info);
    func_0204a600();
    if (func_ov112_021f44e8(sys->info) == TRUE) {
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, sys->heapId);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MusicalShotSys_VBlank
static void func_ov112_021f3a4c(TCB *tcb, void *data) {
    func_0204a648();
}

// pokebw2's MusicalShotSys_InitGraphics
static void func_ov112_021f3a54(MusicalShotSys *sys) {
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, WIPE_COLOR_BLACK);
    func_0202223c(1, WIPE_COLOR_BLACK);
    func_02022224(0);
    func_02022224(1);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetVRAMBanks(&data_ov112_021f816c.config);
    GFL_BGSysCreate(sys->heapId);
    BmpWin_InitAllocator(sys->heapId);
    GFL_BGSysSetLCDConfig(&data_ov112_021f815c);
    ClActSys_Create(&data_020a1448, &data_ov112_021f816c.config, sys->heapId);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's MusicalShotSys_ExitGraphics
static void func_ov112_021f3ae0(MusicalShotSys *sys) {
    func_0204a5ac();
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

