// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pms_input_view.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pms_input_view.h"
#include "types.h"
#include "app/pms_input.h"
#include "app/pmsiv_category.h"
#include "app/pmsiv_edit.h"
#include "app/pmsiv_menu.h"
#include "app/pmsiv_wordwin.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "system/gf_font.h"
#include "system/pmsi_param.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "nnsys/gfd.h"

// The phrase input's screens: the input sends commands, each of which runs as a task until the screens' parts have
// finished moving, and the parts share the graphics and the message window set up here. The names are ours, guessed

// How many commands can run at once
#define PMSIV_COMMAND_MAX 4

// The message file of the phrase input's texts
#define PMSIV_MSG_FILE TEXT_BANK_PMS_INPUT

struct PMSInputView {
    TCB *mainTask;
    TCB *vintrTask;
    TCB *cmdTask[PMSIV_COMMAND_MAX];
    const PMSInputWork *mwk;
    const PMSInputData *dwk;
    ClActUnit *actUnit;
    // The OBJ graphics, for each screen
    PMSIVObjRes objRes[2];
    PMSIVObjRes objRes2[2];
    PMSIVEdit *edit;
    PMSIVCategory *category;
    PMSIVWordWin *wordWin;
    void *unk60;
    PMSIVMenu *menu;
    // The two colors of the edit area's text
    GXRgb colors[2];
    BmpWin *msgWin;
    BOOL msgPrinting;
    // 1 while the cursor is on the buttons
    u8 status;
    int *keyMode;
    PrintQueue *printQueue;
    Font *font;
    BOOL lowerScreenChanged;
    BOOL lowerScreenCategories;
    ArcTool *arc;
};

typedef struct {
    PMSInputView *vwk;
    const PMSInputWork *mwk;
    const PMSInputData *dwk;
    u32 cmd;
    int storePos;
    int seq;
} PMSIVCommandWork;

static void func_ov110_021b9bac(TCB *tcb, void *data);
static void func_ov110_021b9bd0(TCB *tcb, void *data);
static void func_ov110_021b9cbc(PMSIVCommandWork *cwk);
static void func_ov110_021b9ce4(TCB *tcb, void *data);
static void func_ov110_021b9f34(PMSIVCommandWork *cwk, ArcTool *arc);
static void func_ov110_021b9fb4(TCB *tcb, void *data);
static void func_ov110_021ba070(PMSIVCommandWork *cwk);
static void func_ov110_021ba150(TCB *tcb, void *data);
static void func_ov110_021ba1a0(TCB *tcb, void *data);
static void func_ov110_021ba1d8(TCB *tcb, void *data);
static void func_ov110_021ba1f0(TCB *tcb, void *data);
static void func_ov110_021ba208(TCB *tcb, void *data);
static void func_ov110_021ba220(TCB *tcb, void *data);
static void func_ov110_021ba258(TCB *tcb, void *data);
static void func_ov110_021ba290(TCB *tcb, void *data);
static void func_ov110_021ba2d8(TCB *tcb, void *data);
void func_ov110_021ba4cc(TCB *tcb, void *data);
static void func_ov110_021ba5c4(TCB *tcb, void *data);
static void func_ov110_021ba6bc(TCB *tcb, void *data);
static void func_ov110_021ba7cc(TCB *tcb, void *data);
static void func_ov110_021ba8c4(TCB *tcb, void *data);
static void func_ov110_021ba958(TCB *tcb, void *data);
static void func_ov110_021ba974(TCB *tcb, void *data);
static void func_ov110_021ba998(TCB *tcb, void *data);
static void func_ov110_021ba9b4(TCB *tcb, void *data);
static void func_ov110_021ba9d0(TCB *tcb, void *data);
static void func_ov110_021baa0c(TCB *tcb, void *data);
static void func_ov110_021baa30(TCB *tcb, void *data);
static void func_ov110_021baa3c(TCB *tcb, void *data);
static void func_ov110_021baa5c(TCB *tcb, void *data);
static void func_ov110_021baa68(TCB *tcb, void *data);
static void func_ov110_021baa74(TCB *tcb, void *data);
static void func_ov110_021baa80(TCB *tcb, void *data);
static void func_ov110_021baa8c(TCB *tcb, void *data);
static void func_ov110_021baaf0(TCB *tcb, void *data);
static void func_ov110_021bab0c(TCB *tcb, void *data);
static void func_ov110_021bab70(TCB *tcb, void *data);
static void func_ov110_021babac(TCB *tcb, void *data);
static void func_ov110_021ba398(TCB *tcb, void *data);
static void func_ov110_021ba3f0(TCB *tcb, void *data);
static void func_ov110_021bad68(PMSIVCommandWork *cwk, u32 msg);
static void func_ov110_021bade8(PMSInputView *vwk);

// pokebw2's sPMSIViewVRAMConfig
static const BGSysVRAMConfig data_ov110_021c3714 = {
    GX_VRAM_BG_128_B, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A,  GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K,
};

// pokebw2's PMSIView_Create
PMSInputView *func_ov110_021b9a50(const PMSInputWork *mwk, const PMSInputData *dwk) {
    PMSInputView *vwk;
    int i;

    vwk = GFL_HeapAllocate(HEAPID_PMS_INPUT, sizeof(PMSInputView), FALSE, "pms_input_view.c", 223);
    if (vwk) {
        vwk->menu = NULL;
        vwk->printQueue = NULL;
        vwk->mwk = mwk;
        vwk->dwk = dwk;
        vwk->keyMode = func_ov110_021bf6d4(mwk);
        ClActSys_Create(&data_020a1448, &data_ov110_021c3714, HEAPID_PMS_INPUT);
        vwk->actUnit = func_0204b100(128, 0, HEAPID_PMS_INPUT);
        GFL_BGSysCreate(HEAPID_PMS_INPUT);
        BmpWin_InitAllocator(HEAPID_PMS_INPUT);
        GX_SetMasterBrightness(-16);
        GXS_SetMasterBrightness(-16);
        vwk->mainTask = GFL_TCBMgrAddTask(func_ov110_021bf7d0(mwk), func_ov110_021b9bac, vwk, 2);
        vwk->vintrTask = func_ov110_021b9ba4(func_ov110_021b9bd0, vwk, 1);
        for (i = 0; i < PMSIV_COMMAND_MAX; i++) {
            vwk->cmdTask[i] = NULL;
        }
        vwk->msgPrinting = FALSE;
        vwk->status = 0;
        vwk->lowerScreenChanged = FALSE;
        vwk->lowerScreenCategories = FALSE;
        vwk->arc = GFL_ArcSysCreateFileHandle(ARCID_PMSI, HEAPID_PMS_INPUT);
    }
    return vwk;
}

// pokebw2's PMSIView_Delete
void func_ov110_021b9b20(PMSInputView *vwk) {
    int i;

    if (vwk) {
        GFL_ArcToolFree(vwk->arc);
        for (i = 0; i < PMSIV_COMMAND_MAX; i++) {
            if (vwk->cmdTask[i]) {
                GFL_TCBRemove(vwk->cmdTask[i]);
            }
        }
        GFL_TCBRemove(vwk->mainTask);
        GFL_TCBRemove(vwk->vintrTask);
        func_ov110_021bca30(vwk->menu);
        func_0204b1cc(vwk->actUnit);
        for (i = 0; i < 2; i++) {
            func_0204ada4(vwk->objRes[i].palette);
            func_0204a8d4(vwk->objRes[i].chars);
            func_0204afd8(vwk->objRes[i].cellAnims);
            func_0204ada4(vwk->objRes2[i].palette);
            func_0204a8d4(vwk->objRes2[i].chars);
            func_0204afd8(vwk->objRes2[i].cellAnims);
        }
        func_0204a5ac();
        BmpWin_FreeAllocator();
        GFL_BGSysFree();
        GFL_HeapFree(vwk);
    }
}

// pokebw2's PMSIView_AddVTask
TCB *func_ov110_021b9ba4(TCBFunc func, void *wk, u32 priority) {
    return GFL_VBlankTCBAdd(func, wk, priority);
}

// pokebw2's PMSIView_MainTask
static void func_ov110_021b9bac(TCB *tcb, void *data) {
    PMSInputView *vwk = data;

    func_0204a600();
    if (vwk->menu != NULL) {
        func_ov110_021bca84(vwk->menu);
    }
    if (vwk->printQueue != NULL) {
        func_0201c4e4(vwk->printQueue);
        func_ov110_021bade8(vwk);
    }
}

// pokebw2's PMSIView_VintrTask
static void func_ov110_021b9bd0(TCB *tcb, void *data) {
    PMSInputView *vwk = data;

    if (vwk->lowerScreenChanged) {
        vwk->lowerScreenChanged = FALSE;
    }
    GFL_BGSysUpdate();
    func_0204a648();
}

// pokebw2's PMSIView_SetCommand
void func_ov110_021b9bec(PMSInputView *vwk, int cmd) {
    TCBFunc funcs[PMSIV_CMD_COUNT] = {
        func_ov110_021b9ce4,
        func_ov110_021b9fb4,
        func_ov110_021ba150,
        func_ov110_021ba1a0,
        func_ov110_021ba1d8,
        func_ov110_021ba1f0,
        func_ov110_021ba208,
        func_ov110_021ba220,
        func_ov110_021ba258,
        func_ov110_021ba290,
        func_ov110_021ba2d8,
        func_ov110_021ba4cc,
        func_ov110_021ba5c4,
        func_ov110_021ba6bc,
        func_ov110_021ba7cc,
        func_ov110_021ba8c4,
        func_ov110_021ba958,
        func_ov110_021ba974,
        func_ov110_021ba998,
        func_ov110_021ba9b4,
        func_ov110_021ba9d0,
        func_ov110_021baa0c,
        func_ov110_021baa30,
        func_ov110_021baa3c,
        func_ov110_021ba398,
        func_ov110_021ba3f0,
        func_ov110_021baa5c,
        func_ov110_021baa68,
        func_ov110_021baa74,
        func_ov110_021baa80,
        func_ov110_021baa8c,
        func_ov110_021baaf0,
        func_ov110_021bab0c,
        func_ov110_021bab70,
        func_ov110_021babac,
    };

    if (cmd < PMSIV_CMD_COUNT) {
        PMSIVCommandWork *cwk =
            GFL_HeapAllocate(HEAPID_PMS_INPUT, sizeof(PMSIVCommandWork), FALSE, "pms_input_view.c", 493);
        if (cwk) {
            int i;

            cwk->seq = 0;
            cwk->cmd = cmd;
            cwk->vwk = vwk;
            cwk->mwk = vwk->mwk;
            cwk->dwk = vwk->dwk;
            for (i = 0; i < PMSIV_COMMAND_MAX; i++) {
                if (vwk->cmdTask[i] == NULL) {
                    cwk->storePos = i;
                    vwk->cmdTask[i] = GFL_TCBMgrAddTask(func_ov110_021bf7d0(vwk->mwk), funcs[cmd], cwk, 1);
                    break;
                }
            }
        }
    }
}

// pokebw2's PMSIView_WaitCommandAll
BOOL func_ov110_021b9c74(PMSInputView *vwk) {
    int i;

    for (i = 0; i < PMSIV_COMMAND_MAX; i++) {
        if (vwk->cmdTask[i] != NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's PMSIView_WaitCommand
BOOL func_ov110_021b9c90(PMSInputView *vwk, int cmd) {
    int i;

    for (i = 0; i < PMSIV_COMMAND_MAX; i++) {
        if (vwk->cmdTask[i] != NULL) {
            PMSIVCommandWork *cwk = GFL_TCBGetData(vwk->cmdTask[i]);
            if (cwk->cmd == cmd) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

// pokebw2's PMSIView_DeleteCommand
static void func_ov110_021b9cbc(PMSIVCommandWork *cwk) {
    GFL_TCBRemove(cwk->vwk->cmdTask[cwk->storePos]);
    cwk->vwk->cmdTask[cwk->storePos] = NULL;
    GFL_HeapFree(cwk);
}

// pokebw2's PMSIView_CmdInit
static void func_ov110_021b9ce4(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    ArcTool *arc;

    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);

    arc = GFL_ArcSysCreateFileHandle(ARCID_PMSI, HEAPID_PMS_INPUT);
    func_ov110_021ba070(cwk);
    func_ov110_021b9f34(cwk, arc);

    cwk->vwk->edit = func_ov110_021bbc5c(cwk->vwk, cwk->mwk, cwk->dwk);
    func_ov110_021bbd60(cwk->vwk->edit, arc);
    cwk->vwk->category = func_ov110_021bb290(cwk->vwk, cwk->mwk, cwk->dwk);
    func_ov110_021bb328(cwk->vwk->category, arc);
    cwk->vwk->wordWin = func_ov110_021bf844(cwk->vwk, cwk->mwk, cwk->dwk);
    func_ov110_021bf8f4(cwk->vwk->wordWin);

    loadBGScrToVramByFileNoReserveNegAlign(arc, 30, 5, 0, 0, FALSE, HEAPID_PMS_INPUT);
    GFL_BGSysLoadArcNCGRStatic(arc, 18, 5, 0, 0, FALSE, HEAPID_PMS_INPUT);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 6, 0, 0, FALSE, HEAPID_PMS_INPUT);
    GFL_BGSysLoadArcNCGRStatic(arc, 18, 6, 0, 0, FALSE, HEAPID_PMS_INPUT);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_G2DIOLoadArcNCLR(arc, 4, 4, 0x80, 0xa0, 0x20, HEAPID_PMS_INPUT);

    cwk->vwk->colors[0] = GX_RGB(11, 10, 10);
    NNS_GfdRegisterNewVramTransferTask(31, 0xbc, &cwk->vwk->colors[0], sizeof(GXRgb));
    cwk->vwk->colors[1] = GX_RGB(20, 20, 21);
    NNS_GfdRegisterNewVramTransferTask(31, 0xbe, &cwk->vwk->colors[1], sizeof(GXRgb));

    GFL_BGSysLoadArcNCGRStatic(arc, 18, 7, 0, 0, FALSE, HEAPID_PMS_INPUT);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 24, 7, 0, 0, FALSE, HEAPID_PMS_INPUT);
    GFL_BGSysSetScrPaletteNo(7, 0, 0, 32, 32, 5);
    cwk->vwk->msgWin = BmpWin_CreateDynamic(7, 3, 5, 26, 7, 5, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(cwk->vwk->msgWin), 7);
    cwk->vwk->msgPrinting = FALSE;
    if (func_ov110_021bf7c4(cwk->mwk) || func_ov110_021bf6d8(cwk->mwk) != PMSI_MODE_SENTENCE) {
        func_ov110_021bad68(cwk, 1);
    } else {
        func_ov110_021bad68(cwk, 0);
    }
    func_ov110_021bad2c(cwk->vwk, FALSE);

    cwk->vwk->menu = func_ov110_021bc990(cwk->vwk, cwk->mwk, cwk->dwk);
    func_ov110_021bcb50(cwk->vwk->menu);

    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    GX_DispOn();
    GFL_BGSysLoadArcNCGRStatic(arc, 16, 3, 0, 0, FALSE, HEAPID_PMS_INPUT);
    GFL_BGSysLoadArcNCGRStatic(arc, 17, 3, 0x60, 0, FALSE, HEAPID_PMS_INPUT);
    GFL_ArcToolFree(arc);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_SetupObjGraphic
static void func_ov110_021b9f34(PMSIVCommandWork *cwk, ArcTool *arc) {
    PMSInputView *vwk = cwk->vwk;
    int i;

    for (i = 0; i < 2; i++) {
        vwk->objRes[i].palette = func_0204abf0(arc, 7, i, 0, HEAPID_PMS_INPUT);
        vwk->objRes[i].chars = func_0204a6c8(arc, 19, FALSE, i, HEAPID_PMS_INPUT);
        vwk->objRes[i].cellAnims = func_0204af28(arc, 36, 42, HEAPID_PMS_INPUT);
        vwk->objRes2[i].palette = func_0204abf0(arc, 1, i, 0xe0, HEAPID_PMS_INPUT);
        vwk->objRes2[i].chars = func_0204a6c8(arc, 46, FALSE, i, HEAPID_PMS_INPUT);
        vwk->objRes2[i].cellAnims = func_0204af28(arc, 34, 40, HEAPID_PMS_INPUT);
    }
}

// pokebw2's PMSIView_CmdQuit
static void func_ov110_021b9fb4(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    switch (cwk->seq) {
    case 0:
        GFL_WipeSet(0, 0, 0, 0, 5, 1, HEAPID_PMS_INPUT);
        cwk->seq++;
        break;
    case 1:
        if (GFL_WipeIsFinished()) {
            BmpWin_Free(cwk->vwk->msgWin);
            func_ov110_021bbcf0(cwk->vwk->edit);
            func_ov110_021bb2cc(cwk->vwk->category);
            func_ov110_021bf8bc(cwk->vwk->wordWin);
            GFL_FontFree(vwk->font);
            func_0201c6ec(vwk->printQueue);
            func_0201c4c0(vwk->printQueue);
            vwk->printQueue = NULL;
            GFL_BGSysReleaseBG(0);
            GFL_BGSysReleaseBG(1);
            GFL_BGSysReleaseBG(2);
            GFL_BGSysReleaseBG(3);
            GFL_BGSysReleaseBG(4);
            GFL_BGSysReleaseBG(5);
            GFL_BGSysReleaseBG(6);
            GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's sPMSIViewLCDConfig
static const BGSysLCDConfig data_ov110_021c3604 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sPMSIViewBG0Setup
static const BGSetup data_ov110_021c3694 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xd800,
    GX_BG_CHARBASE_0x00000,
    0x8000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sPMSIViewBG1Setup
static const BGSetup data_ov110_021c36b4 = {
    0,
    0,
    0x1000,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xe000,
    GX_BG_CHARBASE_0x10000,
    0x8000,
    GX_BG_EXTPLTT_01,
    1,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sPMSIViewBG2Setup
static const BGSetup data_ov110_021c3674 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xf000,
    GX_BG_CHARBASE_0x18000,
    0x8000,
    GX_BG_EXTPLTT_01,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sPMSIViewBG3Setup
static const BGSetup data_ov110_021c3614 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xf800,
    GX_BG_CHARBASE_0x08000,
    0x5000,
    GX_BG_EXTPLTT_01,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sPMSIViewBG4Setup
static const BGSetup data_ov110_021c36f4 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x0000,
    GX_BG_CHARBASE_0x04000,
    0x8000,
    GX_BG_EXTPLTT_01,
    1,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sPMSIViewBG5Setup
static const BGSetup data_ov110_021c36d4 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x0800,
    GX_BG_CHARBASE_0x0c000,
    0x8000,
    GX_BG_EXTPLTT_01,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sPMSIViewBG6Setup
static const BGSetup data_ov110_021c3654 = {
    0,
    0,
    0x1000,
    0,
    BGRES_256x512,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x1000,
    GX_BG_CHARBASE_0x14000,
    0x8000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sPMSIViewBG7Setup
static const BGSetup data_ov110_021c3634 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x2000,
    GX_BG_CHARBASE_0x1c000,
    0x4000,
    GX_BG_EXTPLTT_01,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's PMSIView_SetupBG
static void func_ov110_021ba070(PMSIVCommandWork *cwk) {
    PMSInputView *vwk = cwk->vwk;
    int i;

    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    GFL_BGSysSetVRAMBanks(&data_ov110_021c3714);
    GFL_BGSysSetLCDConfig(&data_ov110_021c3604);
    GFL_BGSysCreateBG(0, &data_ov110_021c3694, BGMODE_TEXT);
    GFL_BGSysCreateBG(1, &data_ov110_021c36b4, BGMODE_TEXT);
    GFL_BGSysCreateBG(2, &data_ov110_021c3674, BGMODE_TEXT);
    GFL_BGSysCreateBG(3, &data_ov110_021c3614, BGMODE_TEXT);
    GFL_BGSysCreateBG(4, &data_ov110_021c36f4, BGMODE_TEXT);
    GFL_BGSysCreateBG(5, &data_ov110_021c36d4, BGMODE_TEXT);
    GFL_BGSysCreateBG(6, &data_ov110_021c3654, BGMODE_TEXT);
    GFL_BGSysCreateBG(7, &data_ov110_021c3634, BGMODE_TEXT);
    for (i = 0; i <= 7; i++) {
        GFL_BGSysClearScr(i);
        GFL_BGSysSetBGEnabled(i, TRUE);
    }
    vwk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_PMS_INPUT);
    vwk->printQueue = func_0201c450(0x500, HEAPID_PMS_INPUT);
}

// pokebw2's PMSIView_CmdFadeIn
static void func_ov110_021ba150(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;

    switch (cwk->seq) {
    case 0:
        GFL_WipeSet(0, 1, 1, 0, 5, 1, HEAPID_PMS_INPUT);
        cwk->seq++;
        break;
    case 1:
        if (GFL_WipeIsFinished()) {
            cwk->seq++;
        }
        break;
    default:
        func_ov110_021b9cbc(cwk);
        break;
    }
}

// pokebw2's PMSIView_CmdUpdateEditArea
static void func_ov110_021ba1a0(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    vwk->status = 0;
    func_ov110_021bc23c(vwk->edit);
    func_ov110_021bc72c(vwk->edit, func_ov110_021bf714(cwk->mwk));
    func_ov110_021bcca8(vwk->menu);
    func_ov110_021bcd44(vwk->menu);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdChangeKTEditArea
static void func_ov110_021ba1d8(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;

    func_ov110_021bae24(cwk->vwk, cwk->mwk, cwk->dwk);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdChangeKTCategory
static void func_ov110_021ba1f0(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;

    func_ov110_021bae94(cwk->vwk, cwk->mwk, cwk->dwk);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdChangeKTWordWin
static void func_ov110_021ba208(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;

    func_ov110_021baed0(cwk->vwk, cwk->mwk, cwk->dwk);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdEditAreaToButton
static void func_ov110_021ba220(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    vwk->status = 1;
    func_ov110_021bc704(vwk->edit, FALSE);
    func_ov110_021bc724(vwk->edit);
    func_ov110_021bcf7c(vwk->menu, func_ov110_021bf71c(vwk->mwk), TRUE);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdButtonToEditArea
static void func_ov110_021ba258(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    vwk->status = 1;
    func_ov110_021bc704(vwk->edit, FALSE);
    func_ov110_021bc724(vwk->edit);
    func_ov110_021bcf7c(vwk->menu, func_ov110_021bf71c(vwk->mwk), FALSE);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdButtonToEditAreaSelect
static void func_ov110_021ba290(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    vwk->status = 0;
    func_ov110_021bcf7c(vwk->menu, func_ov110_021bf71c(vwk->mwk), FALSE);
    func_ov110_021bc728(vwk->edit);
    func_ov110_021bc704(vwk->edit, TRUE);
    func_ov110_021bc72c(vwk->edit, func_ov110_021bf714(cwk->mwk));
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdEditAreaToCategory
static void func_ov110_021ba2d8(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;
    BOOL flag1, flag2;

    switch (cwk->seq) {
    case 0:
        func_ov110_021bc704(vwk->edit, TRUE);
        func_ov110_021bc72c(vwk->edit, func_ov110_021bf714(cwk->mwk));
        cwk->seq++;
        break;
    case 1:
        func_ov110_021bf7e0(cwk->mwk);
        func_ov110_021bcc58(vwk->menu);
        func_ov110_021bc6ec(vwk->edit);
        func_ov110_021bc724(vwk->edit);
        func_ov110_021bb76c(vwk->category);
        func_ov110_021bbef4(vwk->edit, FALSE);
        if (func_ov110_021bf6dc(cwk->mwk) == 1) {
            func_ov110_021bb958(vwk->category);
        }
        if (func_ov110_021bf6dc(cwk->mwk) == 0) {
            func_ov110_021bad68(cwk, 2);
        } else {
            func_ov110_021bad68(cwk, 4);
        }
        cwk->seq++;
        break;
    case 2:
        flag1 = func_ov110_021bb784(vwk->category);
        flag2 = func_ov110_021bbf54(vwk->edit);
        if (flag1 && flag2) {
            func_ov110_021bb6a0(vwk->category, func_ov110_021bf724(vwk->mwk));
            func_ov110_021bb658(vwk->category, TRUE);
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdChangeCategoryModeDisable
static void func_ov110_021ba398(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    switch (cwk->seq) {
    case 0:
        cwk->seq++;
        // fallthrough
    case 1:
        func_ov110_021bcc58(vwk->menu);
        func_ov110_021bb8d8(vwk->category);
        func_ov110_021bb8b8(vwk->category);
        if (func_ov110_021bf6dc(cwk->mwk) == 1) {
            func_ov110_021bb958(vwk->category);
        }
        cwk->seq++;
        break;
    case 2:
        if (func_ov110_021bb954(vwk->category)) {
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdChangeCategoryModeEnable
static void func_ov110_021ba3f0(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;
    BOOL flag1, flag2;

    switch (cwk->seq) {
    case 0:
        func_ov110_021bcee4(vwk->menu, 1);
        func_ov110_021bb9c0(vwk->category, FALSE);
        cwk->seq++;
        break;
    case 1:
        flag1 = func_ov110_021bbb0c(vwk->category, FALSE);
        flag2 = func_ov110_021bcf38(vwk->menu, 1);
        if (flag1 && flag2) {
            func_ov110_021bcf70(vwk->menu, 1);
            func_ov110_021bf7e0(cwk->mwk);
            func_ov110_021bcc58(vwk->menu);
            func_ov110_021bb658(vwk->category, FALSE);
            func_ov110_021bb8d8(vwk->category);
            func_ov110_021bb8b8(vwk->category);
            if (func_ov110_021bf6dc(cwk->mwk) == 1) {
                func_ov110_021bb958(vwk->category);
            }
            if (func_ov110_021bf6dc(cwk->mwk) == 0) {
                func_ov110_021bad68(cwk, 2);
            } else {
                func_ov110_021bad68(cwk, 4);
            }
            cwk->seq++;
        }
        break;
    case 2:
        if (func_ov110_021bb954(vwk->category)) {
            cwk->seq++;
        }
        break;
    case 3:
        func_ov110_021bb6a0(vwk->category, func_ov110_021bf724(vwk->mwk));
        func_ov110_021bb658(vwk->category, TRUE);
        func_ov110_021b9cbc(cwk);
        break;
    }
}

// pokebw2's PMSIView_CmdCategoryToEditArea
void func_ov110_021ba4cc(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;
    BOOL flag1, flag2;

    switch (cwk->seq) {
    case 0:
        if (func_ov110_021bf6dc(cwk->mwk) == 1) {
            func_ov110_021bcee4(vwk->menu, 0);
            func_ov110_021bb9c0(vwk->category, FALSE);
        } else {
            func_ov110_021bd000(vwk->menu);
        }
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bf6dc(cwk->mwk) == 1) {
            flag1 = func_ov110_021bbb0c(vwk->category, FALSE);
            flag2 = func_ov110_021bcf38(vwk->menu, 0);
        } else {
            flag1 = func_ov110_021bd028(vwk->menu);
            flag2 = TRUE;
        }
        if (flag1 && flag2) {
            cwk->seq++;
        }
        break;
    case 2:
        func_ov110_021bcb50(vwk->menu);
        func_ov110_021bb658(vwk->category, FALSE);
        func_ov110_021bb7a0(vwk->category);
        func_ov110_021bbef4(vwk->edit, TRUE);
        if (func_ov110_021bf7c4(cwk->mwk) || func_ov110_021bf6d8(cwk->mwk) != PMSI_MODE_SENTENCE) {
            func_ov110_021bad68(cwk, 1);
        } else {
            func_ov110_021bad68(cwk, 0);
        }
        cwk->seq++;
        break;
    case 3:
        flag1 = func_ov110_021bb7b8(vwk->category);
        flag2 = func_ov110_021bbf54(vwk->edit);
        if (flag1 && flag2) {
            func_ov110_021bc728(vwk->edit);
            func_ov110_021bc6f8(vwk->edit);
            vwk->status = 0;
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdCategoryToWordWin
static void func_ov110_021ba5c4(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;
    BOOL flag1, flag2;

    switch (cwk->seq) {
    case 0:
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bbc10(vwk->category)) {
            func_ov110_021bb658(vwk->category, FALSE);
            func_ov110_021bcee4(vwk->menu, 3);
            cwk->seq++;
        }
        break;
    case 2:
        if (func_ov110_021bcf38(vwk->menu, 3)) {
            func_ov110_021bb838(vwk->category);
            func_ov110_021bb9c0(vwk->category, FALSE);
            cwk->seq++;
        }
        break;
    case 3:
        flag1 = func_ov110_021bbb0c(vwk->category, FALSE);
        flag2 = func_ov110_021bb854(vwk->category);
        if (flag1 && flag2) {
            func_ov110_021bcc88(vwk->menu);
            func_ov110_021bf9f0(vwk->wordWin);
            func_ov110_021bfaa0(vwk->wordWin);
            if (func_ov110_021bf6dc(cwk->mwk) == 0) {
                func_ov110_021bad68(cwk, 3);
            } else {
                func_ov110_021bad68(cwk, 5);
            }
            cwk->seq++;
        }
        break;
    case 4:
        if (func_ov110_021bfb58(vwk->wordWin)) {
            func_ov110_021bfc64(vwk->wordWin, func_ov110_021bf784(vwk->mwk));
            func_ov110_021bfdb0(vwk->wordWin, func_ov110_021bf7ac(vwk->mwk),
                                func_ov110_021bf7b8(vwk->mwk));
            func_ov110_021bfc20(vwk->wordWin, TRUE);
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdWordWinToCategory
static void func_ov110_021ba6bc(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;
    u32 searchCount = func_ov110_021bf7fc(cwk->mwk);
    BOOL flag1, flag2;

    switch (cwk->seq) {
    case 0:
        func_ov110_021bd000(vwk->menu);
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bd028(vwk->menu)) {
            cwk->seq++;
        }
        break;
    case 2:
        func_ov110_021bcc58(vwk->menu);
        cwk->seq++;
        break;
    case 3:
        func_ov110_021bfc20(vwk->wordWin, FALSE);
        func_ov110_021bfb7c(vwk->wordWin);
        cwk->seq++;
        break;
    case 4:
        if (func_ov110_021bfba0(vwk->wordWin)) {
            func_ov110_021bb8b8(vwk->category);
            cwk->seq++;
        }
        break;
    case 5: {
        BOOL flag = FALSE;

        func_ov110_021bad2c(cwk->vwk, FALSE);
        func_ov110_021bb870(vwk->category);
        if (func_ov110_021bf6dc(cwk->mwk) == 0) {
            func_ov110_021bb9c0(vwk->category, flag);
        } else {
            if (searchCount) {
                flag = TRUE;
            }
            func_ov110_021bb9c0(vwk->category, flag);
        }
        if (func_ov110_021bf6dc(cwk->mwk) == 0) {
            func_ov110_021bad68(cwk, 2);
        } else {
            func_ov110_021bad68(cwk, 4);
        }
        cwk->seq++;
        break;
    }
    case 6:
        flag1 = func_ov110_021bb8ac(vwk->category);
        flag2 = func_ov110_021bbb0c(vwk->category, searchCount ? TRUE : FALSE);
        if (flag1 && flag2) {
            func_ov110_021bb6a0(vwk->category, func_ov110_021bf724(vwk->mwk));
            func_ov110_021bb658(vwk->category, TRUE);
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdWordWinToEditArea
static void func_ov110_021ba7cc(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;
    BOOL flag1, flag2;

    switch (cwk->seq) {
    case 0:
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021c00fc(vwk->wordWin)) {
            func_ov110_021bcb50(vwk->menu);
            func_ov110_021bfc20(vwk->wordWin, FALSE);
            func_ov110_021bfb7c(vwk->wordWin);
            func_ov110_021bbef4(vwk->edit, TRUE);
            cwk->seq++;
        }
        break;
    case 2:
        flag1 = func_ov110_021bfba0(vwk->wordWin);
        flag2 = func_ov110_021bbf54(vwk->edit);
        if (flag1 && flag2) {
            func_ov110_021bb804(vwk->category);
            func_ov110_021bad2c(cwk->vwk, FALSE);
            func_ov110_021bb870(vwk->category);
            if (func_ov110_021bf7c4(cwk->mwk) || func_ov110_021bf6d8(cwk->mwk) != PMSI_MODE_SENTENCE) {
                func_ov110_021bad68(cwk, 1);
            } else {
                func_ov110_021bad68(cwk, 0);
            }
            cwk->seq++;
        }
        break;
    case 3:
        if (func_ov110_021bb8ac(vwk->category)) {
            func_ov110_021bb814(vwk->category);
            cwk->seq++;
        }
        break;
    case 4:
        if (func_ov110_021bb82c(vwk->category)) {
            func_ov110_021bc23c(vwk->edit);
            func_ov110_021bc704(vwk->edit, TRUE);
            func_ov110_021bc728(vwk->edit);
            func_ov110_021bc6f8(vwk->edit);
            vwk->status = 0;
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdWordWinToButton
static void func_ov110_021ba8c4(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    switch (cwk->seq) {
    case 0:
        func_ov110_021bfc20(vwk->wordWin, FALSE);
        func_ov110_021bfb7c(vwk->wordWin);
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bfba0(vwk->wordWin)) {
            func_ov110_021bb804(vwk->category);
            func_ov110_021bad2c(cwk->vwk, FALSE);
            func_ov110_021bb870(vwk->category);
            cwk->seq++;
        }
        break;
    case 2:
        if (func_ov110_021bb8ac(vwk->category)) {
            func_ov110_021bb814(vwk->category);
            cwk->seq++;
        }
        break;
    case 3:
        if (func_ov110_021bb82c(vwk->category)) {
            func_ov110_021bc23c(vwk->edit);
            func_ov110_021bc704(vwk->edit, FALSE);
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdMoveEditAreaCursor
static void func_ov110_021ba958(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    func_ov110_021bc72c(vwk->edit, func_ov110_021bf714(vwk->mwk));
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdMoveButtonCursor
static void func_ov110_021ba974(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    func_ov110_021bcf7c(vwk->menu, func_ov110_021bf71c(vwk->mwk), TRUE);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdMoveCategoryCursor
static void func_ov110_021ba998(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    func_ov110_021bb6a0(vwk->category, func_ov110_021bf724(vwk->mwk));
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdMoveWordWinCursor
static void func_ov110_021ba9b4(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    func_ov110_021bfc64(vwk->wordWin, func_ov110_021bf784(vwk->mwk));
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdScrollWordWin
static void func_ov110_021ba9d0(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    switch (cwk->seq) {
    case 0:
        func_ov110_021bfca4(vwk->wordWin, func_ov110_021bf7a0(vwk->mwk));
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bfd60(vwk->wordWin)) {
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdPushButton
static void func_ov110_021baa0c(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    func_ov110_021bcfbc(vwk->menu, func_ov110_021bf71c(cwk->mwk), TRUE);
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdNop22
static void func_ov110_021baa30(TCB *tcb, void *data) {
    func_ov110_021b9cbc(data);
}

// pokebw2's PMSIView_CmdUpdateEditAreaCursor
static void func_ov110_021baa3c(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    if (vwk->status != 1) {
        func_ov110_021bc6f8(vwk->edit);
    }
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdNop26
static void func_ov110_021baa5c(TCB *tcb, void *data) {
    func_ov110_021b9cbc(data);
}

// pokebw2's PMSIView_CmdNop27
static void func_ov110_021baa68(TCB *tcb, void *data) {
    func_ov110_021b9cbc(data);
}

// pokebw2's PMSIView_CmdNop28
static void func_ov110_021baa74(TCB *tcb, void *data) {
    func_ov110_021b9cbc(data);
}

// pokebw2's PMSIView_CmdNop29
static void func_ov110_021baa80(TCB *tcb, void *data) {
    func_ov110_021b9cbc(data);
}

// pokebw2's PMSIView_CmdChangeCategoryMode
static void func_ov110_021baa8c(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;
    u32 searchCount = func_ov110_021bf7fc(cwk->mwk);

    switch (cwk->seq) {
    case 0:
        func_ov110_021bcd80(vwk->menu);
        func_ov110_021bb958(vwk->category);
        func_ov110_021bb9c0(vwk->category, searchCount ? TRUE : FALSE);
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bbb0c(vwk->category, searchCount ? TRUE : FALSE)) {
            cwk->seq++;
        }
        break;
    case 2:
        func_ov110_021b9cbc(cwk);
        break;
    }
}

// pokebw2's PMSIView_CmdSetWordWinArrows
static void func_ov110_021baaf0(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    func_ov110_021bfa40(vwk->wordWin, func_ov110_021bf7ac(vwk->mwk));
    func_ov110_021b9cbc(cwk);
}

// pokebw2's PMSIView_CmdShowMenu
static void func_ov110_021bab0c(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    switch (cwk->seq) {
    case 0:
        func_ov110_021bcee4(vwk->menu, 2);
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bcf38(vwk->menu, 2)) {
            if (*vwk->keyMode == 0 && func_ov110_021bf724(vwk->mwk) == 0xfd &&
                func_ov110_021bf72c(vwk->mwk) == 0xfd) {
                func_ov110_021bcf7c(vwk->menu, 1, TRUE);
            } else {
                func_ov110_021bcf7c(vwk->menu, 0, FALSE);
            }
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdMoveCategory
static void func_ov110_021bab70(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    switch (cwk->seq) {
    case 0:
        func_ov110_021bbb84(vwk->category, func_ov110_021bf724(vwk->mwk));
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021bbc10(vwk->category)) {
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_CmdMoveWordWin
static void func_ov110_021babac(TCB *tcb, void *data) {
    PMSIVCommandWork *cwk = data;
    PMSInputView *vwk = cwk->vwk;

    switch (cwk->seq) {
    case 0:
        func_ov110_021c00ac(vwk->wordWin, func_ov110_021bf784(vwk->mwk));
        cwk->seq++;
        break;
    case 1:
        if (func_ov110_021c00fc(vwk->wordWin)) {
            func_ov110_021b9cbc(cwk);
        }
        break;
    }
}

// pokebw2's PMSIView_GetSentenceEditPosMax
u32 func_ov110_021babe8(PMSInputView *vwk) {
    return func_ov110_021bc6dc(vwk->edit);
}

// pokebw2's PMSIView_GetSentenceWord
u16 func_ov110_021babf4(PMSInputView *vwk, u32 index) {
    return func_ov110_021bc6e0(vwk->edit, index);
}

// pokebw2's PMSIView_GetSentenceWordArea
void func_ov110_021bac00(PMSInputView *vwk, TouchRect *rect, u8 index) {
    func_ov110_021bc320(vwk->edit, rect, index);
}

// pokebw2's PMSIView_GetMenuButton
int func_ov110_021bac0c(PMSInputView *vwk) {
    if (func_ov110_021bcff0(vwk->menu, func_ov110_021bf71c(vwk->mwk))) {
        return 1;
    }
    return -1;
}

// pokebw2's PMSIView_SetMenuButton
void func_ov110_021bac30(PMSInputView *vwk, u32 which) {
    func_ov110_021bcf18(vwk->menu, which);
}

// pokebw2's PMSIView_GetMenu
PMSIVMenu *func_ov110_021bac3c(PMSInputView *vwk) {
    return vwk->menu;
}

// pokebw2's PMSIView_GetActUnit
ClActUnit *func_ov110_021bac40(PMSInputView *vwk) {
    return vwk->actUnit;
}

// pokebw2's PMSIView_GetFont
Font *func_ov110_021bac44(PMSInputView *vwk) {
    return vwk->font;
}

// pokebw2's PMSIView_GetPrintQueue
PrintQueue *func_ov110_021bac4c(PMSInputView *vwk) {
    return vwk->printQueue;
}

// pokebw2's PMSIView_GetObjRes2
void func_ov110_021bac50(PMSInputView *vwk, PMSIVObjRes *res, u32 lcd) {
    *res = vwk->objRes2[lcd];
}

// pokebw2's PMSIView_GetObjRes
void func_ov110_021bac64(PMSInputView *vwk, PMSIVObjRes *res, u32 lcd, u32 bgPriority) {
    *res = vwk->objRes[lcd];
}

// pokebw2's PMSIView_AddActor
ClActor *func_ov110_021bac78(PMSInputView *vwk, const PMSIVObjRes *res, u32 x, u32 y, u32 priority, int drawArea) {
    ClActorSetup setup;
    ClActor *actor;
    u32 irq;
    u16 surface;

    setup.x = x;
    setup.y = y;
    setup.priority = priority;
    setup.bgPriority = 0;
    setup.sequence = 0;
    if (drawArea == NNS_G2D_VRAM_TYPE_2DSUB) {
        surface = CLACT_SURFACE_SUB;
    } else {
        surface = CLACT_SURFACE_MAIN;
    }
    irq = OS_DisableInterrupts();
    actor = func_0204b294(vwk->actUnit, res->chars, res->palette, res->cellAnims, &setup, surface, HEAPID_PMS_INPUT);
    OS_RestoreInterrupts(irq);
    if (actor) {
        func_0204b92c(actor, TRUE);
        func_0204b954(actor, FX32_ONE);
    }
    return actor;
}

// pokebw2's PMSIView_GetWordWinScrollDir
int func_ov110_021bacdc(PMSInputView *vwk, u32 unused, u32 pos) {
    ClActorPos barPos;

    if (!func_ov110_021bfd94(vwk->wordWin, &barPos)) {
        return 3;
    }
    if (pos < barPos.y) {
        return 1;
    }
    if (pos > barPos.y) {
        return 2;
    }
    if (pos == barPos.y) {
        return 0;
    }
    return 3;
}

// pokebw2's PMSIView_SetWordWinScrollBarY
void func_ov110_021bad14(PMSInputView *vwk, u32 y) {
    func_ov110_021bfe24(vwk->wordWin, y);
}

// pokebw2's PMSIView_GetWordWinScrollBarPos
u32 func_ov110_021bad20(PMSInputView *vwk, u32 count) {
    return func_ov110_021bfe60(vwk->wordWin, count);
}

// pokebw2's PMSIView_SetLowerScreen
void func_ov110_021bad2c(PMSInputView *vwk, BOOL categories) {
    u32 fileId, palOffset;

    if (categories) {
        fileId = 29;
        palOffset = 0x60;
    } else {
        fileId = 28;
        palOffset = 0;
    }
    GFL_G2DIOLoadNSCRAsync(vwk->arc, fileId, 3, 0, palOffset, 0, FALSE, HEAPID_PMS_INPUT);
    vwk->lowerScreenChanged = TRUE;
    vwk->lowerScreenCategories = categories;
}

// pokebw2's PMSIView_PrintMessage
static void func_ov110_021bad68(PMSIVCommandWork *cwk, u32 msg) {
    u32 msgIds[] = { 22, 23, 27, 24, 25, 26 };
    MsgData *msgData;
    StrBuf *str;

    GFL_BitmapFill(BmpWin_GetBitmap(cwk->vwk->msgWin), 7);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PMSIV_MSG_FILE, HEAPID_PMS_INPUT);
    str = GFL_MsgDataLoadStrbufNew(msgData, msgIds[msg]);
    func_0201c724(cwk->vwk->printQueue, BmpWin_GetBitmap(cwk->vwk->msgWin), 2, 4, str, cwk->vwk->font,
                  PRINT_COLOR(14, 15, 7));
    cwk->vwk->msgPrinting = TRUE;
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
}

// pokebw2's PMSIView_FlushMessage
static void func_ov110_021bade8(PMSInputView *vwk) {
    if (vwk->msgPrinting) {
        if (!func_0201c6c4(vwk->printQueue, BmpWin_GetBitmap(vwk->msgWin))) {
            BmpWin_Transfer(vwk->msgWin);
            vwk->msgPrinting = FALSE;
        }
    }
}

// pokebw2's PMSIView_ChangeKTEditArea
void func_ov110_021bae24(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk) {
    if (*vwk->keyMode == 1) {
        func_ov110_021bc704(vwk->edit, FALSE);
        func_ov110_021bcf7c(vwk->menu, func_ov110_021bf71c(vwk->mwk), FALSE);
    } else if (vwk->status == 1) {
        func_ov110_021bc704(vwk->edit, FALSE);
        func_ov110_021bcf7c(vwk->menu, func_ov110_021bf71c(vwk->mwk), TRUE);
    } else {
        func_ov110_021bc704(vwk->edit, TRUE);
        func_ov110_021bcf7c(vwk->menu, func_ov110_021bf71c(vwk->mwk), FALSE);
    }
}

// pokebw2's PMSIView_ChangeKTCategory
void func_ov110_021bae94(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk) {
    func_ov110_021bb658(vwk->category, TRUE);
    if (*vwk->keyMode == 1) {
        if (func_ov110_021bf6dc(mwk) == 1) {
            func_ov110_021bcf7c(vwk->menu, 0, FALSE);
        }
    } else {
        func_ov110_021bb6a0(vwk->category, func_ov110_021bf724(vwk->mwk));
    }
}

// pokebw2's PMSIView_ChangeKTWordWin
void func_ov110_021baed0(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk) {
    func_ov110_021bfc20(vwk->wordWin, TRUE);
}
