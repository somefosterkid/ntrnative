// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov162/delete_save.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/title.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "save/save_control.h"
#include "save/save_outside.h"
#include "system/app_keycursor.h"
#include "system/bmp_menu.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wipe.h"

#define HEAPID_SAVEDATA_DELETE 0x81

// Deleting the save data, from the title screen with Up, Select and B: two questions, then every save block is
// cleared and the game restarts

enum {
    STATE_INIT,
    STATE_WAIT_WIPE_IN,
    STATE_PRINT_QUESTION,
    STATE_ASK,
    STATE_PRINT_WARNING,
    STATE_ASK_AGAIN,
    STATE_DELETE,
    STATE_WAIT_WIPE_OUT,
    STATE_END,
};

// The messages of system message file 4
enum {
    MSG_DELETE_ALL,
    MSG_NO_WAY_TO_RECOVER,
    MSG_DELETING,
};

// The save blocks that deleting clears
#define SAVE_BLOCK_COUNT 21

typedef struct {
    TCB *vblankTask;
    Font *font;
    MsgData *msgData;
    u8 langId;
    StrBuf *strbuf;
    PrintStream *printStream;
    TCBExManager *tcbManager;
    BmpWin *window;
    BmpMenu *dialog;
    KeyCursor *keyCursor;
    // Whether the message was already told to go on past its pause
    BOOL continued;
} DeleteSaveWork;

static BOOL func_ov088_021bcccc(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov088_021bccf4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov088_021bcd64(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov088_021bcd88(DeleteSaveWork *wk, u32 *state);
static void func_ov088_021bcdb4(DeleteSaveWork *wk, u32 *state);
static void func_ov088_021bcdd0(DeleteSaveWork *wk, u32 *state);
static void func_ov088_021bcdec(DeleteSaveWork *wk, u32 *state);
static void func_ov088_021bce20(DeleteSaveWork *wk, u32 *state);
static void func_ov088_021bce3c(DeleteSaveWork *wk, u32 *state);
void func_ov088_021bce80(DeleteSaveWork *wk, u32 *state);
static void func_ov088_021bcf18(DeleteSaveWork *wk, u32 *state);
static void func_ov088_021bcf44(void);
static void func_ov088_021bcf6c(void);
static void func_ov088_021bcfd4(void);
static void func_ov088_021bcfec(DeleteSaveWork *wk);
static void func_ov088_021bd04c(DeleteSaveWork *wk);
static void func_ov088_021bd078(DeleteSaveWork *wk);
static void func_ov088_021bd0c8(DeleteSaveWork *wk);
static void func_ov088_021bd0f4(DeleteSaveWork *wk);
static void func_ov088_021bd10c(DeleteSaveWork *wk);
static void func_ov088_021bd118(DeleteSaveWork *wk, u32 messageId);
static BOOL func_ov088_021bd188(DeleteSaveWork *wk);
static void func_ov088_021bd1ec(DeleteSaveWork *wk);
static void func_ov088_021bd21c(void);
static void func_ov088_021bd23c(void);

// pokebw2's DELETE_SAVE_PROC_FUNCTIONS
// pokebw2's DeleteSave_Exit
// pokebw2's DeleteSave_Init
// pokebw2's DeleteSave_Main
const GameProcFunctions data_ov088_021bd974 = { func_ov088_021bcccc, func_ov088_021bccf4, func_ov088_021bcd64 };

// Stepped at VBlank while the save is deleted, as the VBlank tasks do not run then
// pokebw2's sWaitIcon
static WaitIcon *data_ov088_021bda20;

static BOOL func_ov088_021bcccc(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_SAVEDATA_DELETE, 0x30000);
    MI_CpuFill8(GFL_ProcInitSubsystem(proc, sizeof(DeleteSaveWork), HEAPID_SAVEDATA_DELETE), 0,
               sizeof(DeleteSaveWork));
    return TRUE;
}

static BOOL func_ov088_021bccf4(GameProc *proc, u32 *state, void *param, void *work) {
    switch (*state) {
    case STATE_INIT:
        func_ov088_021bcd88(work, state);
        break;
    case STATE_WAIT_WIPE_IN:
        func_ov088_021bcdb4(work, state);
        break;
    case STATE_PRINT_QUESTION:
        func_ov088_021bcdd0(work, state);
        break;
    case STATE_ASK:
        func_ov088_021bcdec(work, state);
        break;
    case STATE_PRINT_WARNING:
        func_ov088_021bce20(work, state);
        break;
    case STATE_ASK_AGAIN:
        func_ov088_021bce3c(work, state);
        break;
    case STATE_DELETE:
        func_ov088_021bce80(work, state);
        break;
    case STATE_WAIT_WIPE_OUT:
        func_ov088_021bcf18(work, state);
        break;
    case STATE_END:
        return TRUE;
    }
    return FALSE;
}

// Goes back to the title screen
static BOOL func_ov088_021bcd64(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_SAVEDATA_DELETE);
    GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd574, NULL);
    return TRUE;
}

// pokebw2's DeleteSave_Start
static void func_ov088_021bcd88(DeleteSaveWork *wk, u32 *state) {
    func_ov088_021bcf44();
    func_ov088_021bcf6c();
    func_ov088_021bcfec(wk);
    func_ov088_021bd078(wk);
    func_ov088_021bd0f4(wk);
    func_ov088_021bd21c();
    *state = STATE_WAIT_WIPE_IN;
}

// pokebw2's DeleteSave_WaitWipeIn
static void func_ov088_021bcdb4(DeleteSaveWork *wk, u32 *state) {
    if (GFL_WipeIsFinished() == TRUE) {
        func_ov088_021bd118(wk, MSG_DELETE_ALL);
        *state = STATE_PRINT_QUESTION;
    }
}

// pokebw2's DeleteSave_PrintQuestion
static void func_ov088_021bcdd0(DeleteSaveWork *wk, u32 *state) {
    if (func_ov088_021bd188(wk) == FALSE) {
        func_ov088_021bd1ec(wk);
        *state = STATE_ASK;
    }
}

// pokebw2's DeleteSave_Ask
static void func_ov088_021bcdec(DeleteSaveWork *wk, u32 *state) {
    switch (func_0201febc(wk->dialog)) {
    case 0:
        func_ov088_021bd118(wk, MSG_NO_WAY_TO_RECOVER);
        *state = STATE_PRINT_WARNING;
        break;
    case BMPMENU_CANCEL:
        func_ov088_021bd23c();
        *state = STATE_WAIT_WIPE_OUT;
        break;
    }
}

// pokebw2's DeleteSave_PrintWarning
static void func_ov088_021bce20(DeleteSaveWork *wk, u32 *state) {
    if (func_ov088_021bd188(wk) == FALSE) {
        func_ov088_021bd1ec(wk);
        *state = STATE_ASK_AGAIN;
    }
}

// pokebw2's DeleteSave_AskAgain
static void func_ov088_021bce3c(DeleteSaveWork *wk, u32 *state) {
    switch (func_0201febc(wk->dialog)) {
    case 0:
        func_ov088_021bd118(wk, MSG_DELETING);
        *state = STATE_DELETE;
        break;
    case BMPMENU_CANCEL:
        func_ov088_021bd23c();
        *state = STATE_WAIT_WIPE_OUT;
        break;
    }
}

// pokebw2's DeleteSave_VBlank
void func_ov088_021bce70(void *data) {
    func_0202f0b0(data_ov088_021bda20);
}


void func_ov228_021dff18(HeapID heapId);
void func_02007230(SaveControl *save);
BOOL func_02007400(SaveControl *save, u32 extra_id, u32 heapId);
void func_ov088_021bce80(DeleteSaveWork *wk, u32 *state) {
    SaveControl *save;
    u32 i;

    if (func_ov088_021bd188(wk) == FALSE) {
        data_ov088_021bda20 = func_0202ee30(NULL, wk->window, 15, 16, HEAPID_SAVEDATA_DELETE);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(wk->window));
        GFL_VBlankSetCallback(func_ov088_021bce70, NULL);
        save = SaveControl_GetInstance();
        GFL_OvlLoad(OVERLAY_ID(228));
        func_ov228_021dff18(HEAPID_SAVEDATA_DELETE);
        GFL_OvlUnload(OVERLAY_ID(228));
        func_02007230(save);
        for (i = 0; i < 9; i++) {
            if (func_0200725c(save, i, HEAPID_SAVEDATA_DELETE) == TRUE) {
                func_02007400(save, i, HEAPID_SAVEDATA_DELETE);
            }
            freeIntermediateSaveExtraBlksAfterLoad(save, i);
        }
        GFL_VBlankResetCallback();
        OS_ResetSystem(0);
    }
}


// pokebw2's DeleteSave_WaitWipeOut
static void func_ov088_021bcf18(DeleteSaveWork *wk, u32 *state) {
    if (GFL_WipeIsFinished() == TRUE) {
        func_ov088_021bd10c(wk);
        func_ov088_021bd0c8(wk);
        func_ov088_021bd04c(wk);
        func_ov088_021bcfd4();
        *state = STATE_END;
    }
}

// pokebw2's DeleteSave_InitVRAM
static void func_ov088_021bcf44(void) {
    BGSysVRAMConfig vramConfig = {
        GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_NONE, GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
    };

    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&vramConfig);
}

// The message window on BG 0 over a blue backdrop
// pokebw2's DeleteSave_InitBG
static void func_ov088_021bcf6c(void) {
    GFL_BGSysCreate(HEAPID_SAVEDATA_DELETE);
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,
                          GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    }
    GFL_BGSysResetStdPalette(0, GX_RGB(12, 12, 31));
    GFL_BGSysResetStdPalette(4, GX_RGB(12, 12, 31));
}

// pokebw2's DeleteSave_FreeBG
static void func_ov088_021bcfd4(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

// The messages are read in language 0 while the screen is up
// pokebw2's DeleteSave_InitMsg
static void func_ov088_021bcfec(DeleteSaveWork *wk) {
    wk->langId = GFL_MsgDataGetDefaultLangID();
    func_020461d4(0);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_DELETE_SAVE, HEAPID_SAVEDATA_DELETE);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, HEAPID_SAVEDATA_DELETE);
    wk->strbuf = GFL_StrBufCreate(1024, HEAPID_SAVEDATA_DELETE);
    wk->tcbManager = GFL_TCBExMgrCreate(HEAPID_SAVEDATA_DELETE, HEAPID_SAVEDATA_DELETE, 1, 4);
    wk->keyCursor = func_02028590(15, TRUE, FALSE, HEAPID_SAVEDATA_DELETE);
}

// pokebw2's DeleteSave_FreeMsg
static void func_ov088_021bd04c(DeleteSaveWork *wk) {
    func_02028604(wk->keyCursor);
    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_StrBufFree(wk->strbuf);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgData);
    func_020461d4(wk->langId);
}

// pokebw2's DeleteSave_InitWindow
static void func_ov088_021bd078(DeleteSaveWork *wk) {
    BmpWin_InitAllocator(HEAPID_SAVEDATA_DELETE);
    wk->window = BmpWin_CreateDynamic(0, 1, 19, 30, 4, 15, 1);
    LoadSysMsgBox(0, 1, 14, 0, HEAPID_SAVEDATA_DELETE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 15 * 0x20, 0x20, HEAPID_SAVEDATA_DELETE);
}

// pokebw2's DeleteSave_FreeWindow
static void func_ov088_021bd0c8(DeleteSaveWork *wk) {
    BmpWin_Free(wk->window);
    BmpWin_FreeAllocator();
}

// pokebw2's DeleteSave_VBlankTask
static void func_ov088_021bd0d8(TCB *tcb, void *data) {
    GFL_BGSysUpdate();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's DeleteSave_AddVBlankTask
static void func_ov088_021bd0f4(DeleteSaveWork *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov088_021bd0d8, wk, 0);
}

// pokebw2's DeleteSave_RemoveVBlankTask
static void func_ov088_021bd10c(DeleteSaveWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
}

// pokebw2's DeleteSave_Print
static void func_ov088_021bd118(DeleteSaveWork *wk, u32 messageId) {
    GFL_MsgDataLoadStrbuf(wk->msgData, messageId, wk->strbuf);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->window), 15);
    BmpWin_DrawFrame(wk->window, WINFRAME_TRANSFER_NONE, 1, 14);
    wk->printStream = func_0201cce4(wk->window, 0, 0, wk->strbuf, wk->font, func_02013080(0), wk->tcbManager, 10,
                                    HEAPID_SAVEDATA_DELETE, 15);
    wk->continued = FALSE;
    BmpWin_Transfer(wk->window);
}

// Returns FALSE once the message has ended
// pokebw2's DeleteSave_UpdatePrint
static BOOL func_ov088_021bd188(DeleteSaveWork *wk) {
    GFL_TCBExMgrUpdate(wk->tcbManager);
    func_020286c4(wk->keyCursor, wk->printStream, wk->window);
    switch (func_0201ce30(wk->printStream)) {
    case PRINT_STREAM_RUNNING:
        wk->continued = FALSE;
        break;
    case PRINT_STREAM_PAUSED:
        if (wk->continued == FALSE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            func_0201ce38(wk->printStream);
            wk->continued = TRUE;
        }
        break;
    case PRINT_STREAM_DONE:
        func_0201ce48(wk->printStream);
        wk->continued = FALSE;
        return FALSE;
    }
    return TRUE;
}

// No is chosen at first
// pokebw2's DeleteSave_OpenYesNo
static void func_ov088_021bd1ec(DeleteSaveWork *wk) {
    ConfirmDialogSetup setup;

    setup.bg = 0;
    setup.x = 24;
    setup.y = 13;
    setup.palette = 15;
    setup.unk4 = 0;
    wk->dialog = ShopUI_CreateConfirmDialog(&setup, 1, 14, 1, HEAPID_SAVEDATA_DELETE);
}

// pokebw2's DeleteSave_WipeIn
static void func_ov088_021bd21c(void) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, HEAPID_SAVEDATA_DELETE);
}

// pokebw2's DeleteSave_WipeOut
static void func_ov088_021bd23c(void) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, HEAPID_SAVEDATA_DELETE);
}
