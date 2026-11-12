// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov264/cdemo_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/cdemo.h"
#include "gfl/arc_util.h"
#include "gfl/calctool.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "system/wipe.h"
#include "gfl/proc.h"
#include "gfl/tcb.h"

static BOOL func_ov179_021b6100(GameProc *proc, u32 *state, void *param, void *unused);
static BOOL func_ov179_021b6138(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov179_021b614c(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov179_021b6278(TCB *tcb, void *data);

// pokebw2's CDEMO_PROC_FUNCTIONS
// pokebw2's CDemo_Exit
// pokebw2's CDemo_Init
// pokebw2's CDemo_Update
const GameProcFunctions data_ov179_021b68d8 = { func_ov179_021b6100, func_ov179_021b6138, func_ov179_021b614c };
// Demo scripts are arrays of opcode words and their arguments.
// pokebw2's CDEMO_SCRIPT_LOGO
static const u32 data_ov179_021b6920[] = {
#include "asset/cdemo_main_data_ov179_021b6920.inc"
};
// pokebw2's CDEMO_SCRIPT_0
static const u32 data_ov179_021b6964[] = {
#include "asset/cdemo_main_data_ov179_021b6964.inc"
};
// pokebw2's CDEMO_SCRIPT_1
static const u32 data_ov179_021b69b0[] = {
#include "asset/cdemo_main_data_ov179_021b69b0.inc"
};
// pokebw2's CDEMO_SCRIPTS
static const u32 *const data_ov179_021b68e4[] = { data_ov179_021b6964, data_ov179_021b69b0, data_ov179_021b6920 };
// pokebw2's CDEMO_VRAM_CONFIG
static const BGSysVRAMConfig data_ov179_021b68f0 = {
#include "asset/cdemo_main_data_ov179_021b68f0.inc"
};


static BOOL func_ov179_021b6100(GameProc *proc, u32 *state, void *param, void *unused) {
    CDemoWork *work;
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_CDEMO, 0x80000);
    work = GFL_ProcInitSubsystem(proc, sizeof(CDemoWork), HEAPID_CDEMO);
    MI_CpuFill8(work, 0, sizeof(CDemoWork));
    work->param = param;
    work->initialized = FALSE;
    return TRUE;
}

static BOOL func_ov179_021b6138(GameProc *proc, u32 *state, void *param, void *work) {
    return func_ov179_021b6380(work) == FALSE;
}

static BOOL func_ov179_021b614c(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_CDEMO);
    return TRUE;
}

// pokebw2's CDemo_ConfigureVRAM
void func_ov179_021b615c(void) {
    GFL_BGSysSetVRAMBanks(&data_ov179_021b68f0);
}

void func_ov179_021b616c(CDemoWork *work) {
    MtxFx22 matrix;
    void *screen;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BG0_AS_2D);
    G2_SetBG2ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256, GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x00000);
    G2_SetBG3ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256, GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x20000);
    MAT2_SetScaleRot(&matrix, 0, FX32_ONE, FX32_ONE, 0);
    G2_SetBG2Affine(&matrix, 0, 0, 0, 0);
    G2_SetBG3Affine(&matrix, 0, 0, 0, 0);
    screen = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_CDEMO), 0x18000, TRUE, "cdemo_main.c", 91);
    GX_LoadBG2Scr(screen, 0, 0x18000);
    GX_LoadBG3Scr(screen, 0, 0x18000);
    GFL_HeapFree(screen);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
}

// pokebw2's CDemo_DisableGraphics
void func_ov179_021b6220(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
}
// pokebw2's CDemo_InitScript
void func_ov179_021b622c(CDemoWork *work) {
    const u32 *script = data_ov179_021b68e4[work->param->demo];
    work->scriptStart = script;
    work->scriptCursor = script;
}
// pokebw2's CDemo_FreeScript
void func_ov179_021b6248(CDemoWork *work) {
}
// pokebw2's CDemo_StartVBlank
void func_ov179_021b624c(CDemoWork *work) {
    work->vblankTask = GFL_VBlankTCBAdd(func_ov179_021b6278, work, 0);
}
// pokebw2's CDemo_StopVBlank
void func_ov179_021b6268(CDemoWork *work) {
    GFL_TCBRemove(work->vblankTask);
}
// pokebw2's CDemo_VBlankCallback
static void func_ov179_021b6278(TCB *tcb, void *data) {
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void func_ov179_021b6290(CDemoWork *work, int division, int sync, u32 resumeState) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, division, sync, HEAPID_CDEMO);
    work->resumeState = resumeState;
}

void func_ov179_021b62b8(CDemoWork *work, int division, int sync, u32 resumeState) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, division, sync, HEAPID_CDEMO);
    work->resumeState = resumeState;
}

void func_ov179_021b62e0(CDemoWork *work, int division, int sync, u32 resumeState) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_WHITE, division, sync, HEAPID_CDEMO);
    work->resumeState = resumeState;
}

void func_ov179_021b630c(CDemoWork *work, int division, int sync, u32 resumeState) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_WHITE, division, sync, HEAPID_CDEMO);
    work->resumeState = resumeState;
}

void func_ov179_021b6338(CDemoWork *work, u32 index, u32 fileId) {
    NNSG2dPaletteData *palette;
    void *file = GFL_G2DIOReadNCLRArc(work->arc, fileId, &palette, HEAPID_TAIL(HEAPID_CDEMO));
    CDemoPalette *buffer = &work->palettes[index];
    buffer->size = palette->szByte;
    MIi_CpuCopy32(palette->pRawData, buffer->data, buffer->size);
    GFL_HeapFree(file);
}





