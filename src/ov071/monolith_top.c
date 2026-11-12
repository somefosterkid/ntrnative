// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov143/monolith_top.c, Black 2/White 2's version
// of this file in overlay 143, adapted to Black's. The names are Black's dsd names and swan's; pokebw2's guesses are in
// the comments.
#include "types.h"
#include "app/monolith/monolith_main.h"
#include "app/monolith/monolith_tool.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "field/intrude_comm.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "save/event_work.h"
#include "system/app_printsys_common.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"

// The Entralink monolith's menu on the bottom screen: its items, which differ with how far the story is, and the
// return button. Unlike Black 2's, it shows only the items that can be picked, and the first one can show a message
// instead of its screen. The ROM gives no name for this file; monolith_top.c is pokebw2's guess for Black 2's

// The menu's BG, for its message window
#define MONOLITH_TOP_BG 5

// The most items, and the value of picked for the return button
#define MONOLITH_TOP_ITEM_MAX 3
#define MONOLITH_TOP_RETURN 3

// The panel whose palette marks the cursor's item
#define MONOLITH_TOP_PANEL 3

// The flag that sets the menu's items
#define MONOLITH_TOP_FLAG 0x980

// The steps of the menu's main proc
enum {
    MONOLITH_TOP_SEQ_INPUT,
    MONOLITH_TOP_SEQ_PICKED,
    MONOLITH_TOP_SEQ_WAIT,
    MONOLITH_TOP_SEQ_MESSAGE,
};

typedef struct {
    MonolithTextActor texts[MONOLITH_TOP_ITEM_MAX];
    s8 cursor;
    // The item picked, or MONOLITH_TOP_RETURN
    s8 picked;
    // Which items the menu shows, an index of the tables below
    u8 mode;
    // Set while the player uses the touch screen, so that the next key press only shows the cursor
    u8 touching;
    MonolithReturnButton returnButton;
    BmpWin *msgWindow;
    PrintStream *printStream;
    StrBuf *msgStr;
} MonolithTopWork;

static BOOL func_ov071_021b7e60(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021b7ee0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021b8150(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov071_021b81a0(MonolithScreenParam *screen, MonolithTopWork *mtw);
static void func_ov071_021b8270(MonolithTopWork *mtw);
static void func_ov071_021b82a4(MonolithScreenParam *screen, MonolithTopWork *mtw);
static void func_ov071_021b82e4(MonolithScreenParam *screen, MonolithTopWork *mtw);
static void func_ov071_021b82f0(MonolithTopWork *mtw);
static void func_ov071_021b82fc(MonolithTopWork *mtw);
static void func_ov071_021b8308(void);
static void func_ov071_021b833c(void);
static void func_ov071_021b8350(MonolithTopWork *mtw, MonolithWork *wk);
static void func_ov071_021b83c0(MonolithTopWork *mtw);
static void func_ov071_021b83cc(MonolithTopWork *mtw, MonolithWork *wk, u32 msgId);
static BOOL func_ov071_021b8434(MonolithWork *wk, MonolithTopWork *mtw);
static void func_ov071_021b8440(MonolithTopWork *mtw);

// The tables are declared in an order that gives the ROM's .rodata layout, which MWCC sorts by size

// What each item and the return button set picked to in each mode
static const u32 data_ov071_021ba9c8[][MONOLITH_TOP_ITEM_MAX + 1] = {
    { 1, 2, MONOLITH_TOP_RETURN, 0 },
    { 0, 1, 2, MONOLITH_TOP_RETURN },
    { 0, 2, MONOLITH_TOP_RETURN, 0 },
};

// The items and the return button in each mode
static const TouchRect data_ov071_021ba9f8[][MONOLITH_TOP_ITEM_MAX + 2] = {
    { { 52, 68, 56, 200 }, { 108, 124, 56, 200 }, { 172, 188, 232, 248 }, { TOUCH_RECT_END, 0, 0, 0 } },
    { { 36, 52, 56, 200 },
      { 76, 92, 56, 200 },
      { 116, 132, 56, 200 },
      { 172, 188, 232, 248 },
      { TOUCH_RECT_END, 0, 0, 0 } },
    { { 52, 68, 56, 200 }, { 108, 124, 56, 200 }, { 172, 188, 232, 248 }, { TOUCH_RECT_END, 0, 0, 0 } },
};

// The number of items in each mode
static const u32 data_ov071_021ba978[] = { 2, 3, 2 };

// The y of each item in each mode
static const u32 data_ov071_021ba9a4[][MONOLITH_TOP_ITEM_MAX] = {
    { 60, 116, 0 },
    { 44, 84, 124 },
    { 60, 116, 0 },
};

// pokebw2's sMonolithTopBGSetup
static const BGSetup data_ov071_021ba984 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x2000,
    GX_BG_CHARBASE_0x0c000,
    0x8000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's MONOLITH_TOP_PROC_FUNCTIONS
const GameProcFunctions data_ov071_021baa34 = { func_ov071_021b7e60, func_ov071_021b7ee0, func_ov071_021b8150 };

// pokebw2's MonolithTop_Init
static BOOL func_ov071_021b7e60(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    GameData *gameData = GSYS_GetGameData(screen->param->gsys);
    MonolithTopWork *mtw = GFL_ProcInitSubsystem(proc, sizeof(MonolithTopWork), HEAPID_MONOLITH);

    MI_CpuFill8(mtw, 0, sizeof(MonolithTopWork));
    if (EventWork_FlagGet(GameData_GetEventWork(gameData), MONOLITH_TOP_FLAG) == FALSE) {
        mtw->mode = 2;
    } else if (screen->param->unk_3D == 1) {
        mtw->mode = 1;
    } else {
        mtw->mode = 0;
    }
    func_ov071_021b8308();
    func_ov071_021b8350(mtw, screen->work);
    func_ov071_021b81a0(screen, mtw);
    func_ov071_021b82e4(screen, mtw);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    return TRUE;
}

// pokebw2's MonolithTop_Main, with MonolithTop_Input in it
static BOOL func_ov071_021b7ee0(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithTopWork *mtw = work;
    IntrudeComm *intrude = func_ov010_02158344(GSYS_GetGameCommSystem(screen->param->gsys));

    func_ov071_021b82a4(screen, mtw);
    func_ov071_021b82fc(mtw);
    if (screen->exiting == TRUE) {
        return TRUE;
    }
    switch (*state) {
    case MONOLITH_TOP_SEQ_INPUT: {
        int hit = func_02035c08(data_ov071_021ba9f8[mtw->mode]);
        u32 typed;
        int pressed;

        if (hit != TOUCH_RECT_NONE) {
            mtw->cursor = hit;
            mtw->picked = data_ov071_021ba9c8[mtw->mode][hit];
            func_020355d8(TRUE);
            mtw->touching = FALSE;
            (*state)++;
            break;
        }
        typed = GCTX_HIDGetTypedKeys();
        pressed = GCTX_HIDGetPressedKeys();
        if (pressed > 0 && mtw->touching == TRUE) {
            func_020355d8(FALSE);
            mtw->touching = FALSE;
            func_ov071_021b6b98(screen, mtw->texts, data_ov071_021ba978[mtw->mode], mtw->cursor, MONOLITH_TOP_PANEL);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        } else if (pressed & PAD_BUTTON_A) {
            mtw->picked = data_ov071_021ba9c8[mtw->mode][mtw->cursor];
            (*state)++;
        } else if (pressed & PAD_BUTTON_B) {
            mtw->picked = MONOLITH_TOP_RETURN;
            (*state)++;
        } else if (typed & PAD_KEY_DOWN) {
            mtw->cursor++;
            if (mtw->cursor >= data_ov071_021ba978[mtw->mode]) {
                mtw->cursor = 0;
            }
            func_ov071_021b6b98(screen, mtw->texts, data_ov071_021ba978[mtw->mode], mtw->cursor, MONOLITH_TOP_PANEL);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        } else if (typed & PAD_KEY_UP) {
            mtw->cursor--;
            if (mtw->cursor < 0) {
                mtw->cursor = data_ov071_021ba978[mtw->mode] - 1;
            }
            func_ov071_021b6b98(screen, mtw->texts, data_ov071_021ba978[mtw->mode], mtw->cursor, MONOLITH_TOP_PANEL);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        }
        break;
    }
    case MONOLITH_TOP_SEQ_PICKED:
        if (mtw->picked == 0) {
            if (screen->param->unk_3D == 0) {
                (*state)--;
                break;
            }
            if (intrude != NULL && func_ov011_02175b14(&intrude->unk_1AC) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                func_ov071_021b83cc(mtw, screen->work, 4);
                *state = MONOLITH_TOP_SEQ_MESSAGE;
                break;
            }
        }
        if (mtw->picked != MONOLITH_TOP_RETURN) {
            func_ov071_021b6bec(screen, mtw->texts, data_ov071_021ba978[mtw->mode], mtw->cursor, MONOLITH_TOP_PANEL);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            func_ov071_021b7108(&mtw->returnButton);
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        }
        (*state)++;
        break;
    case MONOLITH_TOP_SEQ_WAIT:
        if (mtw->picked != MONOLITH_TOP_RETURN) {
            if (func_ov071_021b6c74(screen, MONOLITH_TOP_PANEL) != PANEL_MODE_FLASH) {
                screen->next = mtw->picked + 1;
                return TRUE;
            }
        } else if (func_ov071_021b7114(&mtw->returnButton) == FALSE) {
            screen->next = mtw->picked + 1;
            return TRUE;
        }
        break;
    case MONOLITH_TOP_SEQ_MESSAGE:
        if (func_ov071_021b8434(screen->work, mtw) == TRUE &&
            (func_02035c60() || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)))) {
            func_ov071_021b8440(mtw);
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            *state = MONOLITH_TOP_SEQ_INPUT;
        }
        break;
    }
    return FALSE;
}

// pokebw2's MonolithTop_Exit
static BOOL func_ov071_021b8150(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithTopWork *mtw = work;

    if (func_0201c6b4(screen->work->printQueue) == FALSE) {
        return FALSE;
    }
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_BG3);
    if (mtw->printStream != NULL) {
        func_0201ce48(mtw->printStream);
    }
    if (mtw->msgStr != NULL) {
        GFL_StrBufFree(mtw->msgStr);
    }
    func_ov071_021b8270(mtw);
    func_ov071_021b82f0(mtw);
    func_ov071_021b83c0(mtw);
    func_ov071_021b833c();
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's MonolithTop_CreateTexts
static void func_ov071_021b81a0(MonolithScreenParam *screen, MonolithTopWork *mtw) {
    u32 i;

    func_ov071_021b6c54(screen);
    for (i = 0; i < data_ov071_021ba978[mtw->mode]; i++) {
        if (mtw->mode == 1) {
            func_ov071_021b6a0c(screen->work, &mtw->texts[i], 1, MONOLITH_TEXT_MEDIUM, 128,
                                data_ov071_021ba9a4[mtw->mode][i], i + 1, NULL);
        } else if (mtw->mode == 0) {
            func_ov071_021b6a0c(screen->work, &mtw->texts[i], 1, MONOLITH_TEXT_MEDIUM, 128,
                                data_ov071_021ba9a4[mtw->mode][i], i + 2, NULL);
        } else if (mtw->mode == 2) {
            func_ov071_021b6a0c(screen->work, &mtw->texts[i], 1, MONOLITH_TEXT_MEDIUM, 128,
                                data_ov071_021ba9a4[mtw->mode][i], i == 0 ? 1 : 3, NULL);
        }
    }
    if (func_020355c4() == FALSE) {
        func_ov071_021b6b98(screen, mtw->texts, data_ov071_021ba978[mtw->mode], 0, MONOLITH_TOP_PANEL);
        mtw->touching = FALSE;
    } else {
        mtw->touching = TRUE;
    }
}

// pokebw2's MonolithTop_DeleteTexts
static void func_ov071_021b8270(MonolithTopWork *mtw) {
    u32 i;

    for (i = 0; i < data_ov071_021ba978[mtw->mode]; i++) {
        func_ov071_021b6b38(&mtw->texts[i]);
    }
}

// pokebw2's MonolithTop_Update
static void func_ov071_021b82a4(MonolithScreenParam *screen, MonolithTopWork *mtw) {
    u32 i;

    func_ov071_021b6c7c(screen, MONOLITH_TOP_PANEL);
    for (i = 0; i < data_ov071_021ba978[mtw->mode]; i++) {
        func_ov071_021b6b70(screen->work, &mtw->texts[i]);
    }
}

// pokebw2's MonolithTop_CreateReturnButton
static void func_ov071_021b82e4(MonolithScreenParam *screen, MonolithTopWork *mtw) {
    func_ov071_021b7070(screen->work, &mtw->returnButton);
}

// pokebw2's MonolithTop_DeleteReturnButton
static void func_ov071_021b82f0(MonolithTopWork *mtw) {
    func_ov071_021b70b8(&mtw->returnButton);
}

// pokebw2's MonolithTop_UpdateReturnButton
static void func_ov071_021b82fc(MonolithTopWork *mtw) {
    func_ov071_021b70c4(&mtw->returnButton);
}

// pokebw2's MonolithTop_CreateBG
static void func_ov071_021b8308(void) {
    GFL_BGSysCreateBG(MONOLITH_TOP_BG, &data_ov071_021ba984, BGMODE_TEXT);
    GFL_BGSysFillScrArea(MONOLITH_TOP_BG, 0, 0, 0, 32, 32, 17);
    GFL_BGSysSetBGEnabled(MONOLITH_TOP_BG, FALSE);
}

// pokebw2's MonolithTop_ReleaseBG
static void func_ov071_021b833c(void) {
    GFL_BGSysSetBGEnabled(MONOLITH_TOP_BG, FALSE);
    GFL_BGSysReleaseBG(MONOLITH_TOP_BG);
}

// pokebw2's MonolithTop_CreateMessageWindow
static void func_ov071_021b8350(MonolithTopWork *mtw, MonolithWork *wk) {
    LoadSysMsgBoxBGChar(MONOLITH_TOP_BG, 1, 0, HEAPID_MONOLITH);
    func_020215ec(wk->fade, ARCID_WINFRAME, GetSysMsgBoxPaletteDatID(0), HEAPID_MONOLITH, PALFADE_BUFFER_SUB_BG, 0x20,
                  11 * 0x10, 0);
    mtw->msgWindow = BmpWin_CreateDynamic(MONOLITH_TOP_BG, 1, 19, 30, 4, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(mtw->msgWindow), 0xff);
    BmpWin_FlushMap(mtw->msgWindow);
    BmpWin_DrawFrame(mtw->msgWindow, FALSE, 1, 11);
}

// pokebw2's MonolithTop_FreeMessageWindow
static void func_ov071_021b83c0(MonolithTopWork *mtw) {
    BmpWin_Free(mtw->msgWindow);
}

// Prints a message of the monolith's into the message window, after pokebw2's MonolithPower_ShowMessage
static void func_ov071_021b83cc(MonolithTopWork *mtw, MonolithWork *wk, u32 msgId) {
    GFL_ASSERT(mtw->printStream == NULL);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    mtw->msgStr = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, msgId);
    mtw->printStream = func_0201cce4(mtw->msgWindow, 0, 0, mtw->msgStr, wk->font, func_02012ffc(), wk->tcbMgr, 10,
                                     HEAPID_MONOLITH, 15);
    GFL_BGSysSetBGEnabled(MONOLITH_TOP_BG, TRUE);
}

// pokebw2's MonolithTop_UpdateMessage
static BOOL func_ov071_021b8434(MonolithWork *wk, MonolithTopWork *mtw) {
    return func_02028484(&wk->printsys, mtw->printStream);
}

// pokebw2's MonolithTop_ClearMessage
static void func_ov071_021b8440(MonolithTopWork *mtw) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(mtw->msgWindow);

    GFL_ASSERT(mtw->printStream != NULL);
    func_0201ce48(mtw->printStream);
    mtw->printStream = NULL;
    GFL_StrBufFree(mtw->msgStr);
    mtw->msgStr = NULL;
    GFL_BGSysSetBGEnabled(MONOLITH_TOP_BG, FALSE);
    GFL_BitmapFill(bitmap, 0xff);
    BmpWin_FlushChar(mtw->msgWindow);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
}
