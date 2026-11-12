// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov301/zukan_open.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_common.h"
#include "app/zukan_open.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/wipe.h"

// The Pokédex's opening

// The extra save block of the Pokédex skin
#define SKIN_BLOCK 7
// The size of the skin's characters, and of its palette
#define SKIN_CHAR_SIZE 0x6000
#define SKIN_PALETTE_SIZE 0x200

// The BGs. 1, 2, 3 and 5 are the cover's halves and 7 is its background
#define BG_COVER_BACK 1
#define BG_COVER_FRONT 2
#define BG_COVER_TOP 3
#define BG_COVER_SUB 5
#define BG_SUB_TOP 7

// The frames that the opening waits for the player
#define WAIT_FRAMES 300

// The steps of the opening, which the table of steps follows
enum {
    STEP_SETUP,
    STEP_END,
    STEP_WAIT_WIPE_IN,
    STEP_WAIT_WIPE_OUT,
    STEP_WAIT_INPUT,
    STEP_OPEN,
    STEP_DONE,
};

// The parts of STEP_OPEN, in the frames they take
enum {
    OPEN_SLIDE,
    OPEN_ZOOM_START,
    OPEN_ZOOM,
    OPEN_WIPE,
};

typedef struct {
    ZukanOpenParam *param;
    TCB *vblankTask;
    u16 timer;
    u8 frame;
    u8 part;
    u32 step;
} ZukanOpenWork;

BOOL func_ov211_021bc240(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov211_021bc26c(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov211_021bc280(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov211_021bc290(ZukanOpenWork *work);
void func_ov211_021bc2b0(void);
void func_ov211_021bc2d8(TCB *tcb, void *data);
void func_ov211_021bc2f4(ZukanOpenWork *work);
static void func_ov211_021bc30c(ZukanOpenWork *work);
static void func_ov211_021bc318(void);
static void func_ov211_021bc43c(void);
static void func_ov211_021bc474(ZukanOpenWork *work);
void func_ov211_021bc5b0(ZukanOpenWork *work);
u32 func_ov211_021bc670(ZukanOpenWork *work);
static u32 func_ov211_021bc6cc(ZukanOpenWork *work);
static u32 func_ov211_021bc6f8(ZukanOpenWork *work);
static u32 func_ov211_021bc70c(ZukanOpenWork *work);
u32 func_ov211_021bc720(ZukanOpenWork *work);
u32 func_ov211_021bc7a0(ZukanOpenWork *work);
void func_ov211_021bc890(void);
static inline void ZukanOpen_InitSubTopBG(void);
static inline void ZukanOpen_InitCoverSubBG(void);

// pokebw2's ZUKAN_OPEN_PROC_FUNCTIONS
// pokebw2's ZukanOpen_Exit
// pokebw2's ZukanOpen_Init
// pokebw2's ZukanOpen_Main
const GameProcFunctions data_ov211_021bc8bc = { func_ov211_021bc240, func_ov211_021bc26c, func_ov211_021bc280 };

// pokebw2's sSteps
static u32 (*const data_ov211_021bc8d8[])(ZukanOpenWork *work) = {
    func_ov211_021bc670,       func_ov211_021bc6cc,       func_ov211_021bc6f8,
    func_ov211_021bc70c, func_ov211_021bc720, func_ov211_021bc7a0,
};


BOOL func_ov211_021bc240(GameProc *proc, u32 *state, void *param, void *work) {
    ZukanOpenWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_ZUKAN_OPEN, 0x60000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ZukanOpenWork), HEAPID_ZUKAN_OPEN);
    MI_CpuFill8(wk, 0, sizeof(ZukanOpenWork));
    wk->param = param;
    return TRUE;
}

static BOOL func_ov211_021bc26c(GameProc *proc, u32 *state, void *param, void *work) {
    if (func_ov211_021bc290(work) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov211_021bc280(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_ZUKAN_OPEN);
    return TRUE;
}


// pokebw2's ZukanOpen_RunStep
static BOOL func_ov211_021bc290(ZukanOpenWork *work) {
    work->step = data_ov211_021bc8d8[work->step](work);
    if (work->step != STEP_DONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZukanOpen_SetVRAM
void func_ov211_021bc2b0(void) {
    BGSysVRAMConfig config = {
        GX_VRAM_BG_128_A,
        GX_VRAM_E,
        GX_VRAM_SUB_BG_128_C,
        GX_VRAM_SUB_BGEXTPLTT_0123_H,
        GX_VRAM_OBJ_128_B,
        GX_VRAM_OBJEXTPLTT_NONE,
        GX_VRAM_SUB_OBJ_128_D,
        GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_NONE,
        GX_VRAM_TEXPLTT_NONE,
        GX_OBJVRAMMODE_CHAR_1D_128K,
        GX_OBJVRAMMODE_CHAR_1D_128K,
    };

    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&config);
}

// pokebw2's ZukanOpen_VBlank
void func_ov211_021bc2d8(TCB *tcb, void *data) {
    GFL_BGSysUpdate();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's ZukanOpen_AddVBlank
void func_ov211_021bc2f4(ZukanOpenWork *work) {
    work->vblankTask = GFL_VBlankTCBAdd(func_ov211_021bc2d8, work, 0);
}

// pokebw2's ZukanOpen_RemoveVBlank
static void func_ov211_021bc30c(ZukanOpenWork *work) {
    GFL_TCBRemove(work->vblankTask);
}

// pokebw2's ZukanOpen_InitCoverTopBG
static inline void ZukanOpen_InitCoverTopBG(void) {
    BGSetup coverTop = { 0, 0xe8, 0x2000, 0, BGRES_512x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x10000, 0x10000, GX_BG_EXTPLTT_23, 2, GX_BG_AREAOVER_REPEAT, FALSE };
    GFL_BGSysCreateBG(BG_COVER_TOP, &coverTop, BGMODE_EXTENDED);
    GFL_BGSysClearScr(BG_COVER_TOP);
}

// pokebw2's ZukanOpen_InitCoverBackBG
static inline void ZukanOpen_InitCoverBackBG(void) {
    BGSetup coverBack = { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_REPEAT, FALSE };
    GFL_BGSysCreateBG(BG_COVER_BACK, &coverBack, BGMODE_TEXT);
    GFL_BGSysClearCharCore(BG_COVER_BACK, BGSYS_TILE_SIZE_16, 0x7fe0, HEAPID_TAIL(HEAPID_ZUKAN_OPEN));
    GFL_BGSysFillScrAsync(BG_COVER_BACK, 0x7fe0 / BGSYS_TILE_SIZE_16);
}

// pokebw2's ZukanOpen_InitCoverFrontBG
static inline void ZukanOpen_InitCoverFrontBG(void) {
    BGSetup coverFront = { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x10000, 0x10000, GX_BG_EXTPLTT_23, 0, GX_BG_AREAOVER_REPEAT, FALSE };
    GFL_BGSysCreateBG(BG_COVER_FRONT, &coverFront, BGMODE_TEXT);
    GFL_BGSysClearScr(BG_COVER_FRONT);
}

// pokebw2's ZukanOpen_InitBG. Each BG is set up by a one-BG inline, three defined before it and two after, as pokebw2
// has them: that lists their initializers in the order that sorts them as Black's .rodata (tools/data_order.py)
static void func_ov211_021bc318(void) {
    GFL_BGSysCreate(HEAPID_ZUKAN_OPEN);
    BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_3, GX_BGMODE_3, FALSE };
    GFL_BGSysSetLCDConfig(&lcdConfig);
    ZukanOpen_InitCoverTopBG();
    ZukanOpen_InitCoverBackBG();
    ZukanOpen_InitCoverFrontBG();
    ZukanOpen_InitSubTopBG();
    ZukanOpen_InitCoverSubBG();
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3, TRUE);
}

// pokebw2's ZukanOpen_InitSubTopBG
static inline void ZukanOpen_InitSubTopBG(void) {
    BGSetup subTop = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x10000, 0x10000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_REPEAT, FALSE };
    GFL_BGSysCreateBG(BG_SUB_TOP, &subTop, BGMODE_EXTENDED);
    GFL_BGSysClearScr(BG_SUB_TOP);
}

// pokebw2's ZukanOpen_InitCoverSubBG
static inline void ZukanOpen_InitCoverSubBG(void) {
    BGSetup coverSub = { 0, 0x18, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_23, 0, GX_BG_AREAOVER_REPEAT, FALSE };
    GFL_BGSysCreateBG(BG_COVER_SUB, &coverSub, BGMODE_TEXT);
    GFL_BGSysClearScr(BG_COVER_SUB);
}

// pokebw2's ZukanOpen_FreeBG
static void func_ov211_021bc43c(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysReleaseBG(BG_COVER_SUB);
    GFL_BGSysReleaseBG(BG_SUB_TOP);
    GFL_BGSysReleaseBG(BG_COVER_FRONT);
    GFL_BGSysReleaseBG(BG_COVER_BACK);
    GFL_BGSysReleaseBG(BG_COVER_TOP);
    GFL_BGSysFree();
}

static void func_ov211_021bc474(ZukanOpenWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_TAIL(HEAPID_ZUKAN_OPEN));

    if (getTrainerGender(work->param->playerInfo) == GENDER_MALE) {
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x66, PALTYPE_MAIN_BG_EX, 0x4000, 0, HEAPID_ZUKAN_OPEN);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x66, PALTYPE_MAIN_BG_EX, 0x6000, 0, HEAPID_ZUKAN_OPEN);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x66, PALTYPE_SUB_BG_EX, 0x4000, 0, HEAPID_ZUKAN_OPEN);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x66, PALTYPE_SUB_BG_EX, 0x6000, 0, HEAPID_ZUKAN_OPEN);
    } else {
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x67, PALTYPE_MAIN_BG_EX, 0x4000, 0, HEAPID_ZUKAN_OPEN);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x67, PALTYPE_MAIN_BG_EX, 0x6000, 0, HEAPID_ZUKAN_OPEN);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x67, PALTYPE_SUB_BG_EX, 0x4000, 0, HEAPID_ZUKAN_OPEN);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0x67, PALTYPE_SUB_BG_EX, 0x6000, 0, HEAPID_ZUKAN_OPEN);
    }
    GFL_BGSysLoadArcNCGRStatic(arc, 0x69, BG_COVER_FRONT, 0, 0, FALSE, HEAPID_ZUKAN_OPEN);
    GFL_BGSysLoadArcNCGRStatic(arc, 0x69, BG_COVER_SUB, 0, 0, FALSE, HEAPID_ZUKAN_OPEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x6c, BG_COVER_TOP, 0, 0, FALSE, HEAPID_ZUKAN_OPEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x6f, BG_COVER_BACK, 0, 0, FALSE, HEAPID_ZUKAN_OPEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x6e, BG_COVER_FRONT, 0, 0, FALSE, HEAPID_ZUKAN_OPEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x70, BG_SUB_TOP, 0, 0, FALSE, HEAPID_ZUKAN_OPEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x6d, BG_COVER_SUB, 0, 0, FALSE, HEAPID_ZUKAN_OPEN);
    GFL_ArcToolFree(arc);
}


void func_ov211_021bc5b0(ZukanOpenWork *work) {
    SaveControl *save = GameData_GetSaveControl(work->param->gameData);
    void *block;
    void *characters;
    void *palette;

    if (func_0200725c(save, SKIN_BLOCK, HEAPID_ZUKAN_OPEN | HEAPID_TAIL_BIT) == TRUE) {
        block = getAddressOfExtraSaveBlk(save, SKIN_BLOCK, 0);
        if (block != NULL && func_0200e8d8(block) == TRUE) {
            characters = func_0200e8cc(block);
            palette = func_0200e8d0(block);
            GFL_BGSysLoadChar(BG_COVER_BACK, characters, SKIN_CHAR_SIZE, 0);
            GFL_BGSysUploadStdPalette(BG_COVER_BACK, palette, SKIN_PALETTE_SIZE, 0);
            GFL_BGSysUploadStdPalette(BG_COVER_SUB, palette, SKIN_PALETTE_SIZE, 0);
            DC_FlushRange(palette, SKIN_PALETTE_SIZE);
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(palette, 0x4000, SKIN_PALETTE_SIZE);
            GX_LoadBGExtPltt(palette, 0x6000, SKIN_PALETTE_SIZE);
            GX_EndLoadBGExtPltt();
            GXS_BeginLoadBGExtPltt();
            GXS_LoadBGExtPltt(palette, 0x4000, SKIN_PALETTE_SIZE);
            GXS_LoadBGExtPltt(palette, 0x6000, SKIN_PALETTE_SIZE);
            GXS_EndLoadBGExtPltt();
        }
    }
    freeIntermediateSaveExtraBlksAfterLoad(save, SKIN_BLOCK);
}

u32 func_ov211_021bc670(ZukanOpenWork *work) {
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetDisplayLayout(0);
    func_ov211_021bc2b0();
    func_ov211_021bc318();
    func_ov211_021bc474(work);
    func_ov211_021bc5b0(work);
    func_0203d34c(1, HEAPID_ZUKAN_OPEN);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, 1);
    func_ov211_021bc2f4(work);
    func_ov212_021c93a8(HEAPID_ZUKAN_OPEN);
    return STEP_WAIT_WIPE_IN;
}



// pokebw2's ZukanOpen_StepEnd
static u32 func_ov211_021bc6cc(ZukanOpenWork *work) {
    func_ov211_021bc30c(work);
    func_ov211_021bc43c();
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    return STEP_DONE;
}

// pokebw2's ZukanOpen_StepWaitWipeIn
static u32 func_ov211_021bc6f8(ZukanOpenWork *work) {
    if (GFL_WipeIsFinished() == TRUE) {
        return STEP_WAIT_INPUT;
    }
    return STEP_WAIT_WIPE_IN;
}

// pokebw2's ZukanOpen_StepWaitWipeOut
static u32 func_ov211_021bc70c(ZukanOpenWork *work) {
    if (GFL_WipeIsFinished() == TRUE) {
        return STEP_END;
    }
    return STEP_WAIT_WIPE_OUT;
}

u32 func_ov211_021bc720(ZukanOpenWork *work) {
    if (work->timer == WAIT_FRAMES) {
        return STEP_OPEN;
    }
    work->timer++;
    if (func_02035c38() == TRUE) {
        func_020355d8(TRUE);
        return STEP_OPEN;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        func_020355d8(FALSE);
        return STEP_OPEN;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        work->param->result = ZUKAN_OPEN_RESULT_BACK;
        func_ov212_021c93c4(HEAPID_ZUKAN_OPEN);
        return STEP_WAIT_WIPE_OUT;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        func_020355d8(FALSE);
        work->param->result = ZUKAN_OPEN_RESULT_CLOSE;
        func_ov212_021c93c4(HEAPID_ZUKAN_OPEN);
        return STEP_WAIT_WIPE_OUT;
    }
    return STEP_WAIT_INPUT;
}

u32 func_ov211_021bc7a0(ZukanOpenWork *work) {
    switch (work->part) {
    case OPEN_SLIDE:
        if (work->frame == 26) {
            GFL_SndSEPlay(SEQ_SE_SYS_76);
            work->frame = 0;
            work->part++;
        } else {
            GFL_BGSysMoveBGReq(BG_COVER_TOP, BG_MOVE_UP, 8);
            GFL_BGSysMoveBGReq(BG_COVER_BACK, BG_MOVE_UP, 8);
            GFL_BGSysMoveBGReq(BG_COVER_FRONT, BG_MOVE_UP, 8);
            GFL_BGSysMoveBGReq(BG_COVER_SUB, BG_MOVE_UP, 8);
            work->frame++;
        }
        break;
    case OPEN_ZOOM_START:
        if (work->frame == 32) {
            GFL_SndSEPlay(SEQ_SE_SYS_77);
            GFL_BGSysAdjustBGOriginReq(BG_COVER_TOP, BG_CENTER_SET_X, 0x80);
            GFL_BGSysAdjustBGOriginReq(BG_COVER_TOP, BG_CENTER_SET_Y, 0x78);
            GFL_BGSysAdjustBGOriginReq(BG_SUB_TOP, BG_CENTER_SET_X, 0x80);
            GFL_BGSysAdjustBGOriginReq(BG_SUB_TOP, BG_CENTER_SET_Y, 0x60);
            work->frame = 0;
            work->part++;
        } else {
            work->frame++;
        }
        break;
    case OPEN_ZOOM:
        func_ov211_021bc890();
        if (work->frame == 4) {
            GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 4, HEAPID_ZUKAN_OPEN);
            work->frame = 0;
            work->part++;
        } else {
            work->frame++;
        }
        break;
    case OPEN_WIPE:
        func_ov211_021bc890();
        if (GFL_WipeIsFinished() == TRUE) {
            work->param->result = ZUKAN_OPEN_RESULT_DONE;
            return STEP_END;
        }
        break;
    }
    return STEP_OPEN;
}



// Grows the cover and the sub screen's, a little each frame
// pokebw2's ZukanOpen_ZoomCover
void func_ov211_021bc890(void) {
    GFL_BGSysScaleBGReq(BG_COVER_TOP, BG_SCALE_ADD_X, 0x80);
    GFL_BGSysScaleBGReq(BG_COVER_TOP, BG_SCALE_ADD_Y, 0x80);
    GFL_BGSysScaleBGReq(BG_SUB_TOP, BG_SCALE_ADD_X, 0x80);
    GFL_BGSysScaleBGReq(BG_SUB_TOP, BG_SCALE_ADD_Y, 0x80);
}
