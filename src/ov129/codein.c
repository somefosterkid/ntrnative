// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov205/codein.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/codein.h"
#include "app/codein_pv.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "system/codein_param.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "nitro/mi.h"

// The code input screen's process. The ROM has no name for this file; "codein.c" is a guess from pokeplatinum, whose
// code input screen has the same three files. Our names

// The archive of the screen's graphics
#define ARCID_CODEIN 59
#define FRIEND_CODE_LENGTH 12

static BOOL func_ov129_021d85c0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov129_021d8714(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov129_021d872c(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov129_021d8804(void);
static void func_ov129_021d893c(TCB *tcb, void *data);

// pokebw2's CODEIN_PROC_FUNCTIONS
// pokebw2's CodeIn_ProcExit
// pokebw2's CodeIn_ProcInit
// pokebw2's CodeIn_ProcMain
const GameProcFunctions data_ov129_021da62c = { func_ov129_021d85c0, func_ov129_021d8714, func_ov129_021d872c };

// pokebw2's sCodeInVRAMConfig
static BGSysVRAMConfig data_ov129_021da800 = {
    GX_VRAM_BG_128_A,
    GX_VRAM_BGEXTPLTT_NONE,
    GX_VRAM_H,
    GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,
    GX_VRAM_OBJEXTPLTT_NONE,
    GX_VRAM_SUB_OBJ_16_I,
    GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_B | GX_VRAM_C,
    GX_VRAM_F | GX_VRAM_G,
    GX_OBJVRAMMODE_CHAR_1D_64K,
    GX_OBJVRAMMODE_CHAR_1D_32K,
};

static BOOL func_ov129_021d85c0(GameProc *proc, u32 *state, void *param, void *work) {
    CodeInWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_CODEIN, 0x40000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(CodeInWork), HEAPID_CODEIN);
    MI_CpuFill8(wk, 0, sizeof(CodeInWork));
    if (param == NULL) {
        int blocks[CODEIN_BLOCK_COUNT];
        CodeInParam *friendCode;

        blocks[0] = 4;
        blocks[1] = 4;
        blocks[2] = 4;
        friendCode = func_0202e4ec(HEAPID_CODEIN, 0, 0, FRIEND_CODE_LENGTH, blocks);
        wk->param = *friendCode;
        GFL_HeapFree(friendCode);
    } else {
        wk->param = *(CodeInParam *)param;
    }
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    wk->arc = GFL_ArcSysCreateFileHandle(ARCID_CODEIN, HEAPID_CODEIN);
    GFL_BGSysCreate(HEAPID_CODEIN);
    BmpWin_InitAllocator(HEAPID_CODEIN);
    wk->fade = func_02021518(HEAPID_CODEIN);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_CODEIN);
    func_02021ee4(wk->fade, TRUE);
    func_0202155c(wk->fade, 0, 0x200, HEAPID_CODEIN);
    func_ov129_021d8804();
    func_ov129_021d9838(wk, &data_ov129_021da800);
    func_ov129_021d89c4(wk);
    func_ov129_021d9490(wk);
    G2_SetBlendAlpha(0, GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2, 15, 7);
    G2S_SetBlendAlpha(0, GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, 7, 8);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov129_021d893c, wk, 16);
    func_0203d34c(TRUE, HEAPID_CODEIN);
    return TRUE;
}

static BOOL func_ov129_021d8714(GameProc *proc, u32 *state, void *param, void *work) {
    BOOL done = func_ov129_021d8f78(work);

    func_0204a600();
    if (done) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov129_021d872c(GameProc *proc, u32 *state, void *param, void *work) {
    CodeInWork *wk = work;

    GFL_TCBRemove(wk->vblankTask);
    GFL_FontFree(wk->font);
    func_ov129_021d99f0(wk);
    BmpWin_FreeAllocator();
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysFree();
    func_020215a0(wk->fade, 0);
    func_02021540(wk->fade);
    GFL_ArcToolFree(wk->arc);
    func_02051c6c(wk->buttons);
    // The screen's own parameter was freed without its string
    if (param == NULL) {
        GFL_StrBufFree(wk->param.str);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_CODEIN);
    return TRUE;
}

// pokebw2's CodeIn_InitBG
static void func_ov129_021d8804(void) {
    GFL_BGSysDisableAllA();
    GFL_BGSysSetVRAMBanks(&data_ov129_021da800);
    MIi_CpuClear32(0, (void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    {
        BGSetup setups[] = {
#include "asset/codein_setups_021d8804.inc"
        };

        GFL_BGSysCreateBG(1, &setups[0], BGMODE_TEXT);
        GFL_BGSysCreateBG(2, &setups[1], BGMODE_TEXT);
        GFL_BGSysCreateBG(3, &setups[2], BGMODE_TEXT);
        GFL_BGSysClearScr(1);
        GFL_BGSysClearScr(2);
        GFL_BGSysClearScr(3);
    }
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, FALSE);
    {
        BGSetup setups[] = {
#include "asset/codein_setups_021d8804_sub.inc"
        };

        GFL_BGSysCreateBG(4, &setups[0], BGMODE_TEXT);
        GFL_BGSysCreateBG(5, &setups[1], BGMODE_TEXT);
        GFL_BGSysCreateBG(6, &setups[2], BGMODE_TEXT);
        GFL_BGSysCreateBG(7, &setups[3], BGMODE_TEXT);
        GFL_BGSysClearScr(4);
        GFL_BGSysClearScr(5);
        GFL_BGSysClearScr(6);
        GFL_BGSysClearScr(7);
    }
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's CodeIn_VBlank
static void func_ov129_021d893c(TCB *tcb, void *data) {
    CodeInWork *wk = data;

    func_02021d50(wk->fade);
    func_0204a648();
}
