// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov143/monolith_power_select.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Entralink monolith's pass power list on the bottom screen: a list of the pass powers the Entree's levels unlock,
// scrolled with the stylus or the keys, from which one is received into the three slots of the C-Gear, giving another
// back when they are full. The ROM gives no name for this file; monolith_power_select.c is a guess from what it does,
// and its asserts call its work `mpw`

#include "types.h"
#include "app/monolith/monolith_tool.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "field/pass_power_check.h"
#include "save/high_link.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/app_menu_common.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_oam.h"
#include "system/bmp_winframe.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"

// The BGs of the message window, of the frame and of the list
#define MONOLITH_POWER_BG_TEXT 4
#define MONOLITH_POWER_BG_FRAME 5
#define MONOLITH_POWER_BG_LIST 6

#define PASS_POWER_COUNT 64

// What CheckPassPowerUnlocked returns for a pass power that isn't shown
#define PASS_POWER_LOCKED 2

// The rows of the list on the screen, each 32 pixels tall
#define MONOLITH_POWER_ROW_COUNT 7
#define MONOLITH_POWER_ROW_HEIGHT 32

// No row
#define MONOLITH_POWER_NONE 0xffff

// How the cursor was last moved: by dragging the list, by the keys or by a tap
enum {
    MONOLITH_POWER_CURSOR_DRAG,
    MONOLITH_POWER_CURSOR_KEYS,
    MONOLITH_POWER_CURSOR_TAP,
};

// Set while receiving a pass power with no slot free, so that one is given back
#define MONOLITH_POWER_FLAG_FULL 1

enum {
    MONOLITH_POWER_SEQ_START,
    MONOLITH_POWER_SEQ_SHOW_HELP,
    MONOLITH_POWER_SEQ_WAIT_HELP,
    MONOLITH_POWER_SEQ_WAIT_RELEASE,
    MONOLITH_POWER_SEQ_INPUT,
    MONOLITH_POWER_SEQ_ASK,
    MONOLITH_POWER_SEQ_WAIT_ASK,
    MONOLITH_POWER_SEQ_YES_NO,
    MONOLITH_POWER_SEQ_RECEIVE,
    MONOLITH_POWER_SEQ_RECEIVED,
    MONOLITH_POWER_SEQ_WAIT_RECEIVED,
    MONOLITH_POWER_SEQ_WAIT_FADE_OUT,
    MONOLITH_POWER_SEQ_PLAY_JINGLE,
    MONOLITH_POWER_SEQ_WAIT_JINGLE,
    MONOLITH_POWER_SEQ_WAIT_FADE_IN,
    MONOLITH_POWER_SEQ_UNUSED_SAVE,
    MONOLITH_POWER_SEQ_WAIT_TOUCH_RELEASE,
    MONOLITH_POWER_SEQ_RETURN,
    MONOLITH_POWER_SEQ_FULL,
    MONOLITH_POWER_SEQ_WAIT_FULL,
    MONOLITH_POWER_SEQ_PICK_RETURNED,
    MONOLITH_POWER_SEQ_EXCHANGE,
    MONOLITH_POWER_SEQ_EXCHANGED,
};

typedef struct {
    TCB *vblankTask;
    u32 barChar;
    MonolithTextActor receiveButton;
    ClActor *arrowUp;
    ClActor *arrowDown;
    MsgData *msgPowerNames;
    GFLBitmap *listBitmaps[50];
    u32 ids[50];
    u32 states[50];
    u8 count;
    u8 drawnCount;
    BmpWin *windows[2];
    PrintWindow titlePrint;
    StrBuf *msgStr;
    PrintStream *print_stream;
    MonolithReturnButton returnButton;
    BmpOamActor *rowActors[7];
    GFLBitmap *rowBitmaps[7];
    s32 scroll;
    u32 dragFrames;
    s32 velocity;
    s32 keyScroll;
    u32 lastY;
    u32 tapY;
    BOOL rowsChanged;
    int cursor;
    int prevCursor;
    int prevCursor2;
    int selected;
    BOOL cursorChanged;
    u32 cursorBy;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *app_menu_work;
} MonolithPowerWork;

static BOOL func_ov071_021b8ee8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021b9070(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov071_021b94cc(GameProc *proc, u32 *state, void *param, void *work);
void func_ov071_021b9560(void);
static void func_ov071_021b95cc(void);
void func_ov071_021b95fc(MonolithPowerWork *mpw, MonolithWork *wk);
static void func_ov071_021b9694(MonolithPowerWork *mpw);
void func_ov071_021b96a4(MonolithPowerWork *mpw, MonolithWork *wk);
static void func_ov071_021b9734(MonolithPowerWork *mpw);
void func_ov071_021b9750(MonolithScreenParam *screen, MonolithPowerWork *mpw);
void func_ov071_021b9798(MonolithPowerWork *mpw, MonolithWork *wk, u32 msgId);
void func_ov071_021b6c48(MonolithScreenParam *screen, int flag);
BOOL func_ov071_021b9c7c(MonolithScreenParam *screen, MonolithPowerWork *mpw);
void func_ov071_021ba004(MonolithPowerWork *mpw);
void func_ov071_021b9c30(MonolithPowerWork *mpw);
BOOL func_ov071_021ba0cc(MonolithPowerWork *mpw, s32 delta);
void func_ov071_021ba01c(MonolithScreenParam *screen, MonolithPowerWork *mpw);
AppTaskMenu *func_ov071_021b7194(MonolithWork *wk, AppTaskMenuRes *res, HeapID heapId);
void func_ov071_021b9818(MonolithPowerWork *mpw, MonolithWork *wk, u32 msgId);
BOOL func_ov071_021b98b4(MonolithWork *wk, MonolithPowerWork *mpw);
void func_ov071_021b98c4(MonolithPowerWork *mpw);
void func_ov071_021b992c(MonolithPowerWork *mpw);
void func_ov071_021b9970(MonolithScreenParam *screen, MonolithPowerWork *mpw, MonolithWork *wk);
static void func_ov071_021b9a18(MonolithPowerWork *mpw);
BOOL func_ov071_021b9a40(MonolithPowerWork *mpw, MonolithWork *wk);
void func_ov071_021ba11c(MonolithPowerWork *mpw);
void func_ov071_021ba140(MonolithPowerWork *mpw);
void func_ov071_021ba1b0(TCB *tcb, void *data);
BOOL func_ov071_021b9f2c(MonolithPowerWork *mpw, int index, u32 value);
int func_ov071_021ba1dc(MonolithPowerWork *mpw, u32 y);
BOOL func_020355c4(void);
BOOL func_0201c6c4(PrintQueue *queue, GFLBitmap *bitmap);
void func_ov071_021b9ae4(MonolithPowerWork *mpw, MonolithWork *wk);
static void func_ov071_021b9b3c(MonolithPowerWork *mpw);
void func_ov071_021b9b64(MonolithScreenParam *screen, MonolithPowerWork *mpw);
static void func_ov071_021b9ba0(MonolithPowerWork *mpw);
void func_ov071_021b9bac(MonolithWork *wk, MonolithPowerWork *mpw);
void func_ov071_021b9bb8(MonolithScreenParam *screen, MonolithPowerWork *mpw);
static void func_ov071_021b9bc8(MonolithPowerWork *mpw);
void func_ov071_021b9bd8(MonolithPowerWork *mpw);
void func_ov071_021b9be8(MonolithScreenParam *screen, MonolithPowerWork *mpw);
static void func_ov071_021b9c1c(MonolithPowerWork *mpw);

// The text colors of a pass power that can be received and of one that can't (pokebw2's sMonolithPowerListColors)
static const u16 data_ov071_021baaf8[2] = { 0x3c40, 0x820 };

// pokebw2's sMonolithPowerTouchRects
static const TouchRect data_ov071_021baafc[] = {
    { 172, 188, 232, 248 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's MONOLITH_POWER_SELECT_PROC_FUNCTIONS
const GameProcFunctions data_ov071_021bab04 = {
    func_ov071_021b8ee8,
    func_ov071_021b9070,
    func_ov071_021b94cc,
};

// The setups of MONOLITH_POWER_BG_TEXT, _FRAME and _LIST
static const BGSetup data_ov071_021bab28[3] = {
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x0c000, 0x8000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
};

// pokebw2's MonolithPower_CreateBGs

static BOOL func_ov071_021b8ee8(GameProc *proc, u32 *state, void *param, void *work)
{
    MonolithScreenParam *screen = param;
    MonolithPowerWork *mpw = work;

    switch (*state)
    {
    case 0:
        mpw = GFL_ProcInitSubsystem(proc, sizeof(MonolithPowerWork), HEAPID_MONOLITH);
        MI_CpuFill8(mpw, 0, sizeof(MonolithPowerWork));
        mpw->scroll = 0x800;
        mpw->selected = MONOLITH_POWER_NONE;
        mpw->prevCursor = MONOLITH_POWER_NONE;
        mpw->prevCursor2 = MONOLITH_POWER_NONE;
        func_ov071_021b6c54(screen);
        mpw->msgPowerNames = GFL_MsgSysLoadData(0, 2, 0xa0, HEAPID_MONOLITH);
        func_ov071_021b9560();
        func_ov071_021b95fc(mpw, screen->work);
        func_ov071_021b96a4(mpw, screen->work);
        func_ov071_021ba11c(mpw);
        func_ov071_021b9bb8(screen, mpw);
        func_ov071_021b9ae4(mpw, screen->work);
        func_ov071_021b9b64(screen, mpw);
        func_ov071_021b9be8(screen, mpw);
        func_ov071_021b9750(screen, mpw);
        mpw->menuRes = func_ov071_021b7174(screen->work, 4, HEAPID_MONOLITH);
        screen->state->unk_01 = 0x30;
        mpw->vblankTask = GFL_VBlankTCBAdd(func_ov071_021ba1b0, mpw, 3);
        func_ov071_021b9970(screen, mpw, screen->work);
        func_ov071_021b992c(mpw);
        (*state)++;
        break;
    case 1:
        if (func_ov071_021b9a40(mpw, screen->work) == TRUE)
        {
            func_ov071_021ba140(mpw);
            mpw->rowsChanged = TRUE;
            if (func_020355c4() == FALSE)
                func_ov071_021b9f2c(mpw, 1, 0);
            else
                func_ov071_021b9f2c(mpw, MONOLITH_POWER_NONE, 0);
            (*state)++;
        }
        break;
    case 2:
    {
        BOOL busy = FALSE;

        PrintWindow_Flush(&mpw->titlePrint, screen->work->printQueue);
        if (PrintWindow_IsPrinted(&mpw->titlePrint) == FALSE)
            busy = TRUE;
        if (busy == FALSE)
        {
            GFL_BGSysSetBGEnabled(MONOLITH_POWER_BG_TEXT, 1);
            GFL_BGSysSetBGEnabled(MONOLITH_POWER_BG_FRAME, 1);
            GFL_BGSysSetBGEnabled(MONOLITH_POWER_BG_LIST, 1);
            GFL_BGSysSetBGEnabledB(0x10, 1);
            return TRUE;
        }
        break;
    }
    }
    return FALSE;
}

static BOOL func_ov071_021b9070(GameProc *proc, u32 *state, void *param, void *work)
{
    MonolithScreenParam *screen = param;
    MonolithPowerWork *mpw = work;
    IntrudeComm *comm;

    comm = func_ov010_02158344(GSYS_GetGameCommSystem(screen->param->gsys));
    func_ov071_021b6c7c(screen, 1);
    func_ov071_021b6c7c(screen, 3);
    func_ov071_021b9bd8(mpw);
    func_ov071_021b9bac(screen->work, mpw);
    PrintWindow_Flush(&mpw->titlePrint, screen->work->printQueue);

    if (screen->exiting == TRUE && *state == MONOLITH_POWER_SEQ_INPUT) {
        return TRUE;
    }

    switch (*state) {
    case MONOLITH_POWER_SEQ_START:
        *state = MONOLITH_POWER_SEQ_SHOW_HELP;
        break;
    case MONOLITH_POWER_SEQ_SHOW_HELP:
        func_ov071_021b9798(mpw, screen->work, 0x25);
        (*state)++;
        break;
    case MONOLITH_POWER_SEQ_WAIT_HELP:
        if (func_ov071_021b98b4(screen->work, mpw) == TRUE) {
            if (func_02035c60() == FALSE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) == 0) {
                break;
            }
            func_ov071_021b98c4(mpw);
            GFL_SndSEPlay(0x547);
            if (func_02035c60() == FALSE) {
                func_ov071_021b9f2c(mpw, 1, MONOLITH_POWER_CURSOR_DRAG);
            } else {
                func_ov071_021b9f2c(mpw, MONOLITH_POWER_NONE, MONOLITH_POWER_CURSOR_DRAG);
            }
            (*state)++;
        }
        break;
    case MONOLITH_POWER_SEQ_WAIT_RELEASE:
        if (func_02035c38() == FALSE && (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) == 0) {
            *state = MONOLITH_POWER_SEQ_INPUT;
        }
        break;
    case MONOLITH_POWER_SEQ_INPUT: {
        u32 keys = GCTX_HIDGetPressedKeys();
        s32 hit = func_02035c08(data_ov071_021baafc);

        if (hit == 0 || (keys & PAD_BUTTON_B) != 0) {
            func_ov071_021b7108(&mpw->returnButton);
            if (hit == 0) {
                func_020355d8(TRUE);
            } else {
                func_020355d8(FALSE);
            }
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            *state = MONOLITH_POWER_SEQ_RETURN;
            break;
        }
        func_ov071_021b9c7c(screen, mpw);
        func_ov071_021ba004(mpw);
        if (func_ov071_021ba0cc(mpw, mpw->velocity + mpw->keyScroll) == TRUE) {
            func_ov071_021ba11c(mpw);
            func_ov071_021ba140(mpw);
            mpw->rowsChanged = TRUE;
        }
        func_ov071_021ba01c(screen, mpw);
        if (mpw->cursor == MONOLITH_POWER_NONE) {
            screen->state->unk_01 = HIGH_LINK_POWER_NONE;
            func_ov071_021b6b58(&mpw->receiveButton, FALSE);
        } else {
            screen->state->unk_01 = mpw->ids[mpw->cursor];
            screen->state->unk_02 = mpw->states[mpw->cursor];
            if (mpw->states[mpw->cursor] == 0) {
                func_ov071_021b6b58(&mpw->receiveButton, TRUE);
            } else {
                func_ov071_021b6b58(&mpw->receiveButton, FALSE);
            }
        }
        if (mpw->selected != MONOLITH_POWER_NONE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            *state = MONOLITH_POWER_SEQ_ASK;
        }
        break;
    }
    case MONOLITH_POWER_SEQ_ASK:
        if (func_ov071_021b6c74(screen, 1) == 2 || func_ov071_021b6c74(screen, 3) == 2) {
            break;
        }
        func_ov071_021b9798(mpw, screen->work, 0x20);
        (*state)++;
        break;
    case MONOLITH_POWER_SEQ_WAIT_ASK:
        if (func_ov071_021b98b4(screen->work, mpw) == TRUE) {
            GFL_ASSERT(mpw->app_menu_work == NULL);
            mpw->app_menu_work = func_ov071_021b7194(screen->work, mpw->menuRes, HEAPID_MONOLITH);
            (*state)++;
        }
        break;
    case MONOLITH_POWER_SEQ_YES_NO: {
        BOOL yes;

        if (func_ov071_021b7204(screen->work, 4, mpw->app_menu_work, &yes) == TRUE) {
            func_ov071_021b71fc(mpw->app_menu_work);
            mpw->app_menu_work = NULL;
            func_ov071_021b98c4(mpw);
            if (yes == TRUE) {
                *state = MONOLITH_POWER_SEQ_RECEIVE;
            } else {
                mpw->selected = MONOLITH_POWER_NONE;
                func_ov071_021b6c30(screen, TRUE, 1);
                func_ov071_021b6b98(screen, &mpw->receiveButton, 1, 0xff, 3);
                *state = MONOLITH_POWER_SEQ_INPUT;
            }
        }
        break;
    }
    case MONOLITH_POWER_SEQ_RECEIVE:
        if (func_ov071_021b6c74(screen, 1) == 2 || func_ov071_021b6c74(screen, 3) == 2) {
            break;
        }
        loadPassPowerToStrbuf(screen->work->wordSet, 0, mpw->ids[mpw->selected]);
        func_ov071_021b9818(mpw, screen->work, 0x23);
        *state = MONOLITH_POWER_SEQ_RECEIVED;
        break;
    case MONOLITH_POWER_SEQ_RECEIVED:
        if (comm != NULL) {
            if (func_ov011_02174660(screen->param->netId) == TRUE) {
                *state = MONOLITH_POWER_SEQ_WAIT_RECEIVED;
            }
        } else {
            *state = MONOLITH_POWER_SEQ_WAIT_RECEIVED;
        }
        break;
    case MONOLITH_POWER_SEQ_WAIT_RECEIVED:
        if (func_ov071_021b98b4(screen->work, mpw) == TRUE) {
            GFL_SndBGMFadeOut(6);
            (*state)++;
        }
        break;
    case MONOLITH_POWER_SEQ_WAIT_FADE_OUT:
        if (GFL_SndBGMIsFading() == FALSE) {
            GFL_SndBGMSetPaused(TRUE);
            GFL_SndBGMPush();
            (*state)++;
        }
        break;
    case MONOLITH_POWER_SEQ_PLAY_JINGLE:
        GFL_SndBGMPlay(0x531, 0xffff);
        (*state)++;
        break;
    case MONOLITH_POWER_SEQ_WAIT_JINGLE:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            GFL_SndBGMPop();
            GFL_SndBGMSetPaused(FALSE);
            GFL_SndBGMFadeIn(6);
            (*state)++;
        }
        break;
    case MONOLITH_POWER_SEQ_WAIT_FADE_IN:
        if (GFL_SndBGMIsFading() == FALSE) {
            *state = MONOLITH_POWER_SEQ_UNUSED_SAVE;
        }
        break;
    case MONOLITH_POWER_SEQ_UNUSED_SAVE: {
        GameCommSys *commSys;

        if (func_02035c60() == FALSE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) == 0) {
            break;
        }
        func_ov071_021b98c4(mpw);
        func_0200bf08(getHighLinkBlockAddress(GameData_GetSaveControl(GSYS_GetGameData(screen->param->gsys))),
                      mpw->ids[mpw->selected]);
        commSys = GSYS_GetGameCommSystem(screen->param->gsys);
        func_020266ac(commSys, func_02012b58(GSYS_GetGameData(screen->param->gsys)), screen->param->netId);
        mpw->selected = MONOLITH_POWER_NONE;
        func_ov071_021b6c30(screen, TRUE, 1);
        func_ov071_021b6b98(screen, &mpw->receiveButton, 1, 0xff, 3);
        if (func_02035c60() != FALSE) {
            *state = MONOLITH_POWER_SEQ_WAIT_TOUCH_RELEASE;
        } else {
            *state = MONOLITH_POWER_SEQ_RETURN;
        }
        break;
    }
    case MONOLITH_POWER_SEQ_WAIT_TOUCH_RELEASE:
        if (func_02035c38() == FALSE) {
            *state = MONOLITH_POWER_SEQ_RETURN;
        }
        break;
    case MONOLITH_POWER_SEQ_RETURN:
        if (func_ov071_021b6c74(screen, 1) == 2 || func_ov071_021b6c74(screen, 3) == 2) {
            break;
        }
        if (func_ov071_021b7114(&mpw->returnButton) != FALSE) {
            break;
        }
        screen->next = 0;
        return TRUE;
    }
    func_ov071_021b9c30(mpw);
    return FALSE;
}


BOOL func_ov071_021b94cc(GameProc *proc, u32 *state, void *param, void *work)
{
    MonolithScreenParam *screen = param;
    MonolithPowerWork *mpw = work;

    if (func_0201c6b4(screen->work->printQueue) == FALSE)
        return FALSE;
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_BG3);
    GFL_TCBRemove(mpw->vblankTask);
    if (mpw->print_stream != NULL)
        func_0201ce48(mpw->print_stream);
    if (mpw->msgStr != NULL)
        GFL_StrBufFree(mpw->msgStr);
    func_ov071_021b9a18(mpw);
    if (mpw->app_menu_work != NULL)
        func_ov071_021b71fc(mpw->app_menu_work);
    func_ov071_021b718c(mpw->menuRes);
    func_ov071_021b9ba0(mpw);
    func_ov071_021b9b3c(mpw);
    func_ov071_021b9bc8(mpw);
    func_ov071_021b9c1c(mpw);
    func_ov071_021b9694(mpw);
    func_ov071_021b9734(mpw);
    func_ov071_021b95cc();
    GFL_MsgDataFree(mpw->msgPowerNames);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

void func_ov071_021b9560(void) {
    GFL_BGSysCreateBG(MONOLITH_POWER_BG_TEXT, &data_ov071_021bab28[0], 0);
    GFL_BGSysCreateBG(MONOLITH_POWER_BG_FRAME, &data_ov071_021bab28[1], 0);
    GFL_BGSysCreateBG(MONOLITH_POWER_BG_LIST, &data_ov071_021bab28[2], 0);
    GFL_BGSysFillScrArea(MONOLITH_POWER_BG_TEXT, 0, 0, 0, 32, 32, 17);
    GFL_BGSysFillScrArea(MONOLITH_POWER_BG_FRAME, 0, 0, 0, 32, 32, 17);
    GFL_BGSysFillScrArea(MONOLITH_POWER_BG_LIST, 0, 0, 0, 32, 32, 17);
}

// pokebw2's MonolithPower_ReleaseBGs
static void func_ov071_021b95cc(void) {
    GFL_BGSysSetBGEnabled(MONOLITH_POWER_BG_TEXT, FALSE);
    GFL_BGSysSetBGEnabled(MONOLITH_POWER_BG_FRAME, FALSE);
    GFL_BGSysSetBGEnabled(MONOLITH_POWER_BG_LIST, FALSE);
    GFL_BGSysReleaseBG(MONOLITH_POWER_BG_TEXT);
    GFL_BGSysReleaseBG(MONOLITH_POWER_BG_FRAME);
    GFL_BGSysReleaseBG(MONOLITH_POWER_BG_LIST);
}

// pokebw2's MonolithPower_LoadGraphics
void func_ov071_021b95fc(MonolithPowerWork *mpw, MonolithWork *wk) {
    ArcTool *menuArc;

    loadBGScrToVramByFileNoReserveNegAlign(wk->arc, 7, MONOLITH_POWER_BG_LIST, 0, FALSE, TRUE, HEAPID_MONOLITH);
    GFL_BGSysLoadArcNCGRStatic(wk->arc, 14, MONOLITH_POWER_BG_FRAME, 0, FALSE, TRUE, HEAPID_MONOLITH);
    loadBGScrToVramByFileNoReserveNegAlign(wk->arc, 9, MONOLITH_POWER_BG_FRAME, 0, FALSE, TRUE, HEAPID_MONOLITH);
    menuArc = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, HEAPID_MONOLITH);
    mpw->barChar = GFL_BGSysAllocChar(MONOLITH_POWER_BG_FRAME, 0x80, TRUE);
    GFL_BGSysLoadArcNCGRStatic(menuArc, 28, MONOLITH_POWER_BG_FRAME, mpw->barChar, 0x80, FALSE, HEAPID_MONOLITH);
    func_02027634(menuArc, MONOLITH_POWER_BG_FRAME, HEAPID_MONOLITH, mpw->barChar, 12);
    GFL_ArcToolFree(menuArc);
    GFL_BGSysLoadScr(MONOLITH_POWER_BG_FRAME);
    GFL_BGSysLoadScr(MONOLITH_POWER_BG_LIST);
}

// pokebw2's MonolithPower_FreeBarChar
static void func_ov071_021b9694(MonolithPowerWork *mpw) {
    GFL_BGSysFreeCharMemory(MONOLITH_POWER_BG_FRAME, mpw->barChar, 0x80);
}

void func_ov071_021b96a4(MonolithPowerWork *mpw, MonolithWork *wk) {
    LoadSysMsgBoxBGChar(MONOLITH_POWER_BG_TEXT, 1, 0, HEAPID_MONOLITH);
    func_020215ec(wk->fade, ARCID_WINFRAME, GetSysMsgBoxPaletteDatID(0), HEAPID_MONOLITH, 1, 0x20, 11 * 0x10, 0);
    mpw->windows[0] = BmpWin_CreateDynamic(MONOLITH_POWER_BG_TEXT, 1, 19, 30, 4, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(mpw->windows[0]), 0xff);
    mpw->windows[1] = BmpWin_CreateDynamic(MONOLITH_POWER_BG_TEXT, 0, 0, 32, 3, 13, TRUE);
    BmpWin_FlushMap(mpw->windows[1]);
    GFL_BGSysLoadScr(MONOLITH_POWER_BG_TEXT);
    mpw->titlePrint.window = mpw->windows[1];
    mpw->titlePrint.flushPending = 0;
}


// pokebw2's MonolithPower_FreeWindows
static void func_ov071_021b9734(MonolithPowerWork *mpw) {
    int i;

    for (i = 0; i < 2; i++) {
        BmpWin_Free(mpw->windows[i]);
    }
}

// pokebw2's MonolithPower_DrawTitle
void func_ov071_021b9750(MonolithScreenParam *screen, MonolithPowerWork *mpw)
{
    StrBuf *str;
    MonolithWork *work;
    PrintQueue *queue;

    str = GFL_MsgDataLoadStrbufNew(screen->work->msgMonolith, 34);
    work = screen->work;
    queue = work->printQueue;
    func_0201c6fc(queue, BmpWin_GetBitmap(mpw->titlePrint.window), 24, 4, str, work->font);
    mpw->titlePrint.flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov071_021b9798(MonolithPowerWork *mpw, MonolithWork *wk, u32 msgId)
{
    GFL_ASSERT(mpw->print_stream == NULL);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    mpw->msgStr = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, msgId);
    mpw->print_stream = func_0201cce4(mpw->windows[0], 0, 0, mpw->msgStr, wk->font, func_02012ffc(), wk->tcbMgr, 10, HEAPID_MONOLITH, 15);
    BmpWin_FlushMap(mpw->windows[0]);
    BmpWin_DrawFrame(mpw->windows[0], TRUE, 1, 11);
}

void func_ov071_021b9818(MonolithPowerWork *mpw, MonolithWork *wk, u32 msgId)
{
    StrBuf *str;

    GFL_ASSERT(mpw->print_stream == NULL);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    str = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, msgId);
    mpw->msgStr = GFL_StrBufCreate(256, HEAPID_MONOLITH);
    GFL_WordSetFormatStrbuf(wk->wordSet, mpw->msgStr, str);
    GFL_StrBufFree(str);
    mpw->print_stream = func_0201cce4(mpw->windows[0], 0, 0, mpw->msgStr, wk->font, func_02012ffc(), wk->tcbMgr, 10, HEAPID_MONOLITH, 15);
    BmpWin_FlushMap(mpw->windows[0]);
    BmpWin_DrawFrame(mpw->windows[0], TRUE, 1, 11);
}


// pokebw2's MonolithPower_UpdateMessage
BOOL func_ov071_021b98b4(MonolithWork *wk, MonolithPowerWork *mpw) {
    return func_02028484(&wk->printsys, mpw->print_stream);
}

void func_ov071_021b98c4(MonolithPowerWork *mpw) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(mpw->windows[0]);

    GFL_ASSERT(mpw->print_stream != NULL);
    func_0201ce48(mpw->print_stream);
    mpw->print_stream = NULL;
    GFL_StrBufFree(mpw->msgStr);
    mpw->msgStr = NULL;
    func_0201f7a8(mpw->windows[0], FALSE);
    GFL_BitmapFill(bitmap, 0xff);
    BmpWin_FlushChar(mpw->windows[0]);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
}


// Clears the list's background below its last row, when the list is shorter than the screen
// pokebw2's MonolithPower_ClearUnusedRows
void func_ov071_021b992c(MonolithPowerWork *mpw) {
    int y;

    if (mpw->count - 2 < MONOLITH_POWER_ROW_COUNT) {
        y = (mpw->count - 1) * MONOLITH_POWER_ROW_HEIGHT / 8;
        GFL_BGSysFillScrArea(MONOLITH_POWER_BG_LIST, 0, 0, y, 32, 32 - y, 17);
        GFL_BGSysLoadScr(MONOLITH_POWER_BG_LIST);
    }
}

void func_ov071_021b9970(MonolithScreenParam *screen, MonolithPowerWork *mpw, MonolithWork *wk)
{
    enum { POWER_FIRST = 0 } i;
    int count;
    PassPowerLevel *levels;
    u32 unlocked;

    i = POWER_FIRST;
    GSYS_GetGameData(screen->param->gsys);
    levels = func_ov071_021b6f40(screen);
    count = 0;
    mpw->ids[count] = 0;
    mpw->states[count] = 1;
    mpw->listBitmaps[count] = GFL_BitmapCreate(23, 2, 0x20, HEAPID_MONOLITH);
    count++;
    for (; i < 0x30; i++)
    {
        unlocked = func_ov010_0216fe2c((const PassPowerData *)wk->unk_34, i, levels, screen->param->highLink.unk_0C);
        if (unlocked != 2)
        {
            mpw->ids[count] = i;
            mpw->states[count] = unlocked;
            mpw->listBitmaps[count] = GFL_BitmapCreate(23, 2, 0x20, HEAPID_MONOLITH);
            count++;
        }
    }
    mpw->ids[count] = 0;
    mpw->states[count] = 1;
    mpw->listBitmaps[count] = GFL_BitmapCreate(23, 2, 0x20, HEAPID_MONOLITH);
    mpw->count = count + 1;
}


// pokebw2's MonolithPower_FreeList
static void func_ov071_021b9a18(MonolithPowerWork *mpw) {
    int i;

    for (i = 0; i < mpw->count; i++) {
        GFL_BitmapFree(mpw->listBitmaps[i]);
    }
}

// Draws the names of the list into their bitmaps, at most 100 rows a frame; returns whether all are drawn
// pokebw2's MonolithPower_DrawList
BOOL func_ov071_021b9a40(MonolithPowerWork *mpw, MonolithWork *wk) {
    int drawn = 0;
    StrBuf *str = GFL_StrBufCreate(32, HEAPID_MONOLITH);
    int i;

    for (i = mpw->drawnCount; i < mpw->count; i++) {
        if (i >= 1 && i < mpw->count - 1) {
            GFL_MsgDataLoadStrbuf(mpw->msgPowerNames, mpw->ids[i], str);
            GFL_TextRendererDrawToBitmapEx(mpw->listBitmaps[i], 0, 0, str, wk->font,
                                           data_ov071_021baaf8[mpw->states[i]]);
        }
        if (++drawn >= 3) {
            break;
        }
    }
    GFL_StrBufFree(str);
    mpw->drawnCount = i;
    if (mpw->drawnCount >= mpw->count) {
        return TRUE;
    }
    return FALSE;
}

void func_ov071_021b9ae4(MonolithPowerWork *mpw, MonolithWork *wk)
{
    BmpOamActorSetup setup = { NULL, 32, 0, 0, 0, 10, 3, 1, CLACT_VRAM_SUB };
    int i;

    setup.palette = wk->actorRes[1].fontPalette;
    for (i = 0; i < 7; i++)
    {
        mpw->rowBitmaps[i] = GFL_BitmapCreate(23, 2, 0x20, HEAPID_MONOLITH);
        setup.bitmap = mpw->rowBitmaps[i];
        mpw->rowActors[i] = func_020255a0(wk->bmpOam, &setup);
    }
}


// pokebw2's MonolithPower_DeleteRowActors
static void func_ov071_021b9b3c(MonolithPowerWork *mpw) {
    int i;

    for (i = 0; i < MONOLITH_POWER_ROW_COUNT; i++) {
        func_0202570c(mpw->rowActors[i]);
        GFL_BitmapFree(mpw->rowBitmaps[i]);
    }
}

// pokebw2's MonolithPower_CreateReceiveButton
void func_ov071_021b9b64(MonolithScreenParam *screen, MonolithPowerWork *mpw) {
    func_ov071_021b6a0c(screen->work, &mpw->receiveButton, 1, MONOLITH_TEXT_NARROW, 64, 180, 36, NULL);
    if (func_020355c4() == TRUE) {
        func_ov071_021b6b58(&mpw->receiveButton, FALSE);
    }
}

// pokebw2's MonolithPower_DeleteReceiveButton
static void func_ov071_021b9ba0(MonolithPowerWork *mpw) {
    func_ov071_021b6b38(&mpw->receiveButton);
}

// pokebw2's MonolithPower_UpdateReceiveButton
void func_ov071_021b9bac(MonolithWork *wk, MonolithPowerWork *mpw) {
    func_ov071_021b6b70(wk, &mpw->receiveButton);
}

// pokebw2's MonolithPower_CreateReturnButton
void func_ov071_021b9bb8(MonolithScreenParam *screen, MonolithPowerWork *mpw) {
    func_ov071_021b7070(screen->work, &mpw->returnButton);
}

// pokebw2's MonolithPower_DeleteReturnButton
static void func_ov071_021b9bc8(MonolithPowerWork *mpw) {
    func_ov071_021b70b8(&mpw->returnButton);
}

// pokebw2's MonolithPower_UpdateReturnButton
void func_ov071_021b9bd8(MonolithPowerWork *mpw) {
    func_ov071_021b70c4(&mpw->returnButton);
}

// pokebw2's MonolithPower_CreateArrows
void func_ov071_021b9be8(MonolithScreenParam *screen, MonolithPowerWork *mpw) {
    mpw->arrowUp = func_ov071_021b7118(screen->work, 128, 28, 9);
    mpw->arrowDown = func_ov071_021b7118(screen->work, 128, 164, 10);
    func_0204b3dc(mpw->arrowUp, FALSE);
    func_0204b3dc(mpw->arrowDown, FALSE);
}

// pokebw2's MonolithPower_DeleteArrows
static void func_ov071_021b9c1c(MonolithPowerWork *mpw) {
    func_ov071_021b7168(mpw->arrowUp);
    func_ov071_021b7168(mpw->arrowDown);
}

void func_ov071_021b9c30(MonolithPowerWork *mpw)
{
    if (mpw->scroll <= 0x800) {
        func_0204b3dc(mpw->arrowUp, FALSE);
    } else {
        func_0204b3dc(mpw->arrowUp, TRUE);
    }
    if ((mpw->scroll >> 8) >= mpw->count * 32 - 200) {
        func_0204b3dc(mpw->arrowDown, FALSE);
    } else {
        func_0204b3dc(mpw->arrowDown, TRUE);
    }
    func_ov071_021b7170(mpw->arrowUp);
    func_ov071_021b7170(mpw->arrowDown);
}

BOOL func_ov071_021b9c7c(MonolithScreenParam *screen, MonolithPowerWork *mpw)
{
    BOOL decided = FALSE;
    int index = MONOLITH_POWER_NONE;
    s32 rem;
    u32 x, y;
    BOOL touch;
    int row;

    mpw->keyScroll = 0;
    touch = func_02035cb8(&x, &y);
    if (mpw->dragFrames == 0 && (x < 24 || x > 224)) {
        touch = FALSE;
    }
    if (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
        row = ((mpw->scroll >> 8) + 24) / MONOLITH_POWER_ROW_HEIGHT;
        rem = ((mpw->scroll >> 8) + 24) % MONOLITH_POWER_ROW_HEIGHT;
        touch = FALSE;
        mpw->velocity = 0;
        mpw->dragFrames = 0;
        if (mpw->cursor == MONOLITH_POWER_NONE) {
            if (rem != 0) {
                row++;
            }
            if (row < 1) {
                row = 1;
            }
            if (row >= mpw->count - 2) {
                func_ov071_021b9f2c(mpw, mpw->count - 2, MONOLITH_POWER_CURSOR_KEYS);
            } else {
                func_ov071_021b9f2c(mpw, row, MONOLITH_POWER_CURSOR_KEYS);
            }
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return FALSE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            func_ov071_021b9f2c(mpw, mpw->cursor, MONOLITH_POWER_CURSOR_KEYS);
            func_020355d8(touch);
            if (mpw->states[mpw->cursor] == 0) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
            return TRUE;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
            if (rem != 0) {
                row++;
            }
            if (func_ov071_021b9f2c(mpw, mpw->cursor - 1, MONOLITH_POWER_CURSOR_KEYS) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            if (mpw->cursor < row) {
                mpw->keyScroll = -0x2000;
            }
            return FALSE;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
            row = ((mpw->scroll >> 8) + 168) / MONOLITH_POWER_ROW_HEIGHT;
            if (func_ov071_021b9f2c(mpw, mpw->cursor + 1, MONOLITH_POWER_CURSOR_KEYS) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            if (mpw->cursor >= row) {
                mpw->keyScroll = 0x2000;
            }
            return FALSE;
        }
    }
    if (mpw->dragFrames == 0) {
        if (touch == TRUE) {
            if (y > 24 && y < 168) {
                mpw->dragFrames = 1;
                mpw->lastY = y;
                mpw->tapY = y;
                func_ov071_021b9f2c(mpw, MONOLITH_POWER_NONE, MONOLITH_POWER_CURSOR_DRAG);
            } else if (func_02035c60() == TRUE && mpw->cursor != MONOLITH_POWER_NONE && x >= 16 && x <= 112
                       && y >= 172 && y <= 188) {
                decided = TRUE;
                index = mpw->cursor;
                func_020355d8(TRUE);
            }
        } else if (mpw->velocity != 0) {
            if (mpw->velocity > 0) {
                mpw->velocity -= 0xc0;
                if (mpw->velocity < 0) {
                    mpw->velocity = 0;
                }
            } else {
                mpw->velocity += 0xc0;
                if (mpw->velocity > 0) {
                    mpw->velocity = 0;
                }
            }
        }
    } else if (touch == FALSE) {
        mpw->dragFrames = 0;
        if (mpw->tapY != MONOLITH_POWER_NONE && MATH_ABS((s32)(mpw->lastY - mpw->tapY)) < 4) {
            row = func_ov071_021ba1dc(mpw, mpw->lastY);
            if (row != MONOLITH_POWER_NONE) {
                decided = TRUE;
                index = row;
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        }
    } else {
        mpw->velocity = (mpw->lastY - y) << 8;
        mpw->lastY = y;
        mpw->dragFrames++;
        if (mpw->tapY != MONOLITH_POWER_NONE && MATH_ABS((s32)(y - mpw->tapY)) > 4) {
            mpw->tapY = MONOLITH_POWER_NONE;
        }
    }
    if (decided == TRUE) {
        mpw->velocity = 0;
        func_ov071_021b9f2c(mpw, index, MONOLITH_POWER_CURSOR_TAP);
    }
    return decided;
}


// Corrected struct: PrintWindow is 32 bytes smaller in Black than Black 2
// (the fields this function writes land at 0x2F4–0x308 in Black)
BOOL func_ov071_021b9f2c(MonolithPowerWork *mpw, int index, u32 value)
{
    if (index != MONOLITH_POWER_NONE)
    {
        if (index >= mpw->count - 1 || index < 1)
            return FALSE;
    }

    mpw->prevCursor = mpw->cursor;
    mpw->prevCursor2 = mpw->cursor;
    mpw->cursor = index;
    mpw->cursorChanged = TRUE;
    mpw->cursorBy = value;
    return TRUE;
}

void func_ov071_021b9f74(MonolithPowerWork *mpw, int index, u32 palette)
{
    if (index != MONOLITH_POWER_NONE)
    {
        s32 scroll = mpw->scroll >> 8;
        s32 top = scroll / MONOLITH_POWER_ROW_HEIGHT;
        s32 rowOffset = scroll % MONOLITH_POWER_ROW_HEIGHT;
        s32 bgOffset = scroll % 64;
        index -= top;
        if (index >= 0 && index < top + MONOLITH_POWER_ROW_COUNT)
        {
            GFL_BGSysSetScrPaletteNo(MONOLITH_POWER_BG_LIST, 3, ((bgOffset + index * MONOLITH_POWER_ROW_HEIGHT - rowOffset) % 256) / 8, 0x19, 4, palette);
        }
    }
}

void func_ov071_021b9fec(MonolithPowerWork *mpw, int index)
{
    func_ov071_021b9f74(mpw, index, 3);
}

void func_ov071_021b9ff8(MonolithPowerWork *mpw, int index) {
    func_ov071_021b9f74(mpw, index, 0);
}

void func_ov071_021ba004(MonolithPowerWork *mpw)
{
    if (mpw->cursorChanged == 1)
        func_ov071_021b9ff8(mpw, mpw->prevCursor);
}

void func_ov071_021ba01c(MonolithScreenParam *screen, MonolithPowerWork *mpw)
{
    if (mpw->cursorChanged != 1)
        return;
    func_ov071_021b9fec(mpw, mpw->cursor);
    if (mpw->cursor == MONOLITH_POWER_NONE)
        func_ov071_021b6c30(screen, 0, 1);
    else if ((mpw->cursorBy == 2 && mpw->cursor == mpw->prevCursor2) || (mpw->cursorBy == 1 && mpw->cursor == mpw->prevCursor))
    {
        if (mpw->states[mpw->cursor] == 0)
        {
            mpw->selected = mpw->cursor;
            if (mpw->cursorBy == 1)
                func_ov071_021b6c48(screen, 1);
            else
                func_ov071_021b6bec(screen, &mpw->receiveButton, 1, 0, 3);
        }
        else if (mpw->cursorBy == 2)
            func_ov071_021b6c30(screen, 1, 1);
    }
    else
        func_ov071_021b6c30(screen, 1, 1);
    GFL_BGSysQueueScrLoad(6);
    mpw->cursorChanged = 0;
}


BOOL func_ov071_021ba0cc(MonolithPowerWork *mpw, s32 delta)
{
    s32 oldScroll = mpw->scroll;
    s32 height = mpw->count * 32 - 8;
    s32 scroll;

    if (height <= 0xC0) {
        return FALSE;
    }
    scroll = oldScroll + delta;
    mpw->scroll = scroll;
    if (scroll < 0x800) {
        mpw->scroll = 0x800;
    } else if (scroll > (height - 0xC0) << 8) {
        mpw->scroll = (height - 0xC0) << 8;
    }
    if ((mpw->scroll >> 8) != (oldScroll >> 8)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov071_021ba11c(MonolithPowerWork *mpw)
{
    GFL_BGSysMoveBGReq(MONOLITH_POWER_BG_LIST, 3, (mpw->scroll >> 8) % 64);
}

void func_ov071_021ba140(MonolithPowerWork *mpw)
{
    s32 base = mpw->scroll >> 8;
    s32 row = base / 32;
    s32 frac = base % 32;
    int i;

    for (i = 0; i < 7; i++)
    {
        func_0202590c(mpw->rowActors[i], 32, (s16)(8 - frac + i * 32));
        if (mpw->listBitmaps[row + i] != NULL)
            GFL_BitmapCopy(mpw->listBitmaps[row + i], mpw->rowBitmaps[i]);
    }
}


void func_ov071_021ba1b0(TCB *tcb, void *data)
{
    MonolithPowerWork *mpw = data;

    if (mpw->rowsChanged == 1)
    {
        int i;
        for (i = 0; i < 7; i++)
            func_020257d0(mpw->rowActors[i]);
        mpw->rowsChanged = 0;
    }
}

int func_ov071_021ba1dc(MonolithPowerWork *mpw, u32 y)
{
    int row = ((mpw->scroll >> 8) + y) >> 5;
    if (row < mpw->count)
        return row;
    return MONOLITH_POWER_NONE;
}


