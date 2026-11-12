// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pmsiv_menu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pmsiv_menu.h"
#include "types.h"
#include "app/pms_input.h"
#include "app/pms_input_view.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/pmsi_param.h"
#include "nnsys/gfd.h"

// The phrase input's buttons on the lower screen: task menu buttons for the edit area, the categories and the search,
// a row of buttons for the sentence types whose chosen one pulses in color, two arrows, and the return button. The
// names are ours, guessed

// The message file of the phrase input's texts
#define PMSIV_MENU_MSG_FILE TEXT_BANK_PMS_INPUT

// The sentence type buttons, and the button that goes back from the categories
#define PMSIV_MENU_TYPE_BUTTON_COUNT 8
#define PMSIV_MENU_TYPE_BUTTON_BACK 7
// The sentence types that have a button
#define PMSIV_MENU_SENTENCE_TYPE_COUNT 5

// The return button's states
enum {
    RETURN_SEQ_NONE,
    RETURN_SEQ_START,
    RETURN_SEQ_WAIT,
    RETURN_SEQ_END,
};

// The task menu buttons' colors
#define PMSIV_MENU_TEXT_COLOR 0x39e3
#define PMSIV_MENU_BUTTON_PALETTE 8
#define PMSIV_MENU_BUTTON_PALETTE_OFF 14
#define PMSIV_MENU_BUTTON_COLOR_1 0x3545
#define PMSIV_MENU_BUTTON_COLOR_2 0x7b2c

// The colors that pulse on the chosen sentence type's button
#define PMSIV_MENU_PULSE_COLOR_COUNT 5

// Black 2's; twinned code confirms the offsets of vwk, mwk, typeButtonRes, returnPalette, returnChars, returnCellAnims,
// returnButton, returnSeq, msgData, taskMenuRes, items, buttons, typeButtons, arrows, pulseFrom, pulseTo, pulse and
// pulseAngle in Black
struct PMSIVMenu {
    PMSInputView *vwk;
    const PMSInputWork *mwk;
    const PMSInputData *dwk;
    int *keyMode;
    PMSIVObjRes typeButtonRes;
    u32 returnPalette;
    u32 returnChars;
    u32 returnCellAnims;
    ClActor *returnButton;
    int returnSeq;
    MsgData *msgData;
    AppTaskMenuRes *taskMenuRes[3];
    AppTaskMenuItem items[3];
    AppTaskMenuWin *buttons[3];
    ClActor *typeButtons[PMSIV_MENU_TYPE_BUTTON_COUNT];
    ClActor *arrows[2];
    GXRgb pulseFrom[PMSIV_MENU_PULSE_COLOR_COUNT];
    GXRgb pulseTo[PMSIV_MENU_PULSE_COLOR_COUNT];
    GXRgb pulse[PMSIV_MENU_PULSE_COLOR_COUNT];
    int pulseAngle;
};

// Black 2's; twinned code confirms the offsets of y and anim in Black
typedef struct {
    u8 x;
    u8 y;
    u8 anim;
} PMSIVMenuArrow;

static void func_ov110_021bcad8(PMSIVMenu *wk);
static void func_ov110_021bcde0(PMSIVMenu *wk);
static BOOL func_ov110_021bcdfc(PMSIVMenu *wk);
static void func_ov110_021bce24(PMSIVMenu *wk);
static BOOL func_ov110_021bce30(PMSIVMenu *wk);
static void func_ov110_021bce48(PMSIVMenu *wk);
static void func_ov110_021bce54(PMSIVMenu *wk);
static BOOL func_ov110_021bce70(PMSIVMenu *wk);
static void func_ov110_021bcea0(PMSIVMenu *wk);
static BOOL func_ov110_021bcebc(PMSIVMenu *wk);
static void func_ov110_021bd044(PMSIVMenu *wk);
static void func_ov110_021bd0d4(PMSIVMenu *wk);
static void func_ov110_021bd0f8(PMSIVMenu *wk, u32 button, BOOL on);
static void func_ov110_021bd114(PMSIVMenu *wk);
static void func_ov110_021bd174(PMSIVMenu *wk);
static void func_ov110_021bd194(PMSIVMenu *wk);
static void func_ov110_021bd248(PMSIVMenu *wk);
static void func_ov110_021bd264(PMSIVMenu *wk);
static void func_ov110_021bd2f0(PMSIVMenu *wk);
static void func_ov110_021bd344(PMSIVMenu *wk);
static void func_ov110_021bd348(PMSIVMenu *wk);
static void func_ov110_021bd448(PMSIVMenu *wk, u32 button);
static void func_ov110_021bd470(PMSIVMenu *wk, u32 button);

// The button of each sentence type
// pokebw2's sPMSIVMenuTypeButtons
static const u8 data_ov110_021c38d4[PMSIV_MENU_SENTENCE_TYPE_COUNT] = { 1, 2, 3, 4, 5 };

// pokebw2's sPMSIVMenuArrows
static const PMSIVMenuArrow data_ov110_021c38d9[2] = {
    { 8, 20, 17 },
    { 240, 20, 18 },
};

// The animations of each sentence type button, off and on
// pokebw2's sPMSIVMenuTypeButtonAnims
static const u8 data_ov110_021c38df[PMSIV_MENU_TYPE_BUTTON_COUNT][2] = {
    { 0, 1 }, { 2, 3 }, { 4, 5 }, { 6, 7 }, { 8, 9 }, { 10, 11 }, { 12, 13 }, { 14, 15 },
};

// pokebw2's PMSIVMenu_Create
PMSIVMenu *func_ov110_021bc990(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk) {
    PMSIVMenu *wk = GFL_HeapAllocate(HEAPID_PMS_INPUT, sizeof(PMSIVMenu), TRUE, "pmsiv_menu.c", 309);
    int i;

    wk->vwk = vwk;
    wk->mwk = mwk;
    wk->dwk = dwk;
    wk->keyMode = func_ov110_021bf6d4(mwk);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PMSIV_MENU_MSG_FILE, HEAPID_PMS_INPUT);
    for (i = 0; i < 3; i++) {
        wk->taskMenuRes[i] = func_02027f60(0, PMSIV_MENU_BUTTON_PALETTE, func_ov110_021bac44(wk->vwk),
                                                   func_ov110_021bac4c(wk->vwk), HEAPID_PMS_INPUT);
    }
    GFL_BGSysLoadNCLRDefault(ARCID_PMSI, 9, 0, 0x1c0, 0x40, HEAPID_PMS_INPUT);
    func_ov110_021bd194(wk);
    func_ov110_021bd044(wk);
    func_ov110_021bd114(wk);
    func_ov110_021bd2f0(wk);
    return wk;
}

// pokebw2's PMSIVMenu_Delete
void func_ov110_021bca30(PMSIVMenu *wk) {
    int i;

    func_ov110_021bcad8(wk);
    func_0204b3b4(wk->returnButton);
    func_0204ada4(wk->returnPalette);
    func_0204a8d4(wk->returnChars);
    func_0204afd8(wk->returnCellAnims);
    for (i = 0; i < 3; i++) {
        func_02027fd8(wk->taskMenuRes[i]);
    }
    func_ov110_021bd344(wk);
    func_ov110_021bd174(wk);
    func_ov110_021bd0d4(wk);
    GFL_MsgDataFree(wk->msgData);
    GFL_HeapFree(wk);
}

// pokebw2's PMSIVMenu_Main
void func_ov110_021bca84(PMSIVMenu *wk) {
    int i;

    for (i = 0; i < 3; i++) {
        if (wk->buttons[i] != NULL) {
            func_02028178(wk->buttons[i]);
        }
    }
    switch (wk->returnSeq) {
    case RETURN_SEQ_NONE:
        break;
    case RETURN_SEQ_START:
        wk->returnSeq = RETURN_SEQ_WAIT;
        // fallthrough
    case RETURN_SEQ_WAIT:
        if (!func_0204b98c(wk->returnButton)) {
            wk->returnSeq = RETURN_SEQ_END;
        }
        break;
    case RETURN_SEQ_END:
        wk->returnSeq = RETURN_SEQ_NONE;
        break;
    }
    func_ov110_021bd348(wk);
}

// pokebw2's PMSIVMenu_Clear
static void func_ov110_021bcad8(PMSIVMenu *wk) {
    int i;

    for (i = 0; i < 3; i++) {
        if (wk->items[i].str != NULL) {
            GFL_StrBufFree(wk->items[i].str);
            wk->items[i].str = NULL;
        }
        if (wk->buttons[i] != NULL) {
            func_02028148(wk->buttons[i]);
            wk->buttons[i] = NULL;
        }
    }
    for (i = 0; i < PMSIV_MENU_TYPE_BUTTON_COUNT; i++) {
        func_0204b3dc(wk->typeButtons[i], FALSE);
    }
    for (i = 0; i < 2; i++) {
        func_0204b3dc(wk->arrows[i], FALSE);
    }
    GFL_BGSysLoadScr(0);
    func_0204b3dc(wk->returnButton, FALSE);
}

// pokebw2's PMSIVMenu_SetupEditButtons
void func_ov110_021bcb50(PMSIVMenu *wk) {
    int i;

    func_ov110_021bcad8(wk);
    for (i = 0; i < 2; i++) {
        AppTaskMenuItem *item = &wk->items[i];

        wk->items[i].str = GFL_MsgDataLoadStrbufNew(wk->msgData, i + 14);
        wk->items[i].color = PMSIV_MENU_TEXT_COLOR;
        wk->items[i].type = i == 1 ? APP_TASKMENU_ITEM_RETURN : 0;
        wk->buttons[i] = func_02027fec(wk->taskMenuRes[i], item, 23, i * 3 + 18, 9, HEAPID_PMS_INPUT);
        func_020282ac(wk->buttons[i], wk->taskMenuRes[i], PMSIV_MENU_BUTTON_PALETTE,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    }
    func_ov110_021bcd44(wk);
    GFL_BGSysLoadScr(0);
    if (!func_ov110_021bf7c4(wk->mwk) && func_ov110_021bf6d8(wk->mwk) == PMSI_MODE_SENTENCE) {
        func_0204b3dc(wk->typeButtons[0], TRUE);
        func_0204b3dc(wk->typeButtons[1], TRUE);
        func_0204b3dc(wk->typeButtons[2], TRUE);
        func_0204b3dc(wk->typeButtons[3], TRUE);
        func_0204b3dc(wk->typeButtons[4], TRUE);
        func_0204b3dc(wk->typeButtons[5], TRUE);
        func_0204b3dc(wk->typeButtons[6], TRUE);
        for (i = 0; i < 2; i++) {
            func_0204b3dc(wk->arrows[i], TRUE);
        }
    }
    func_ov110_021bcca8(wk);
}

// pokebw2's PMSIVMenu_SetupCategoryButtons
void func_ov110_021bcc58(PMSIVMenu *wk) {
    func_ov110_021bcad8(wk);
    func_0204b3dc(wk->typeButtons[PMSIV_MENU_TYPE_BUTTON_BACK], TRUE);
    if (func_ov110_021bf6dc(wk->mwk) == 0) {
        func_ov110_021bd248(wk);
    } else {
        func_ov110_021bd264(wk);
    }
}

// pokebw2's PMSIVMenu_SetupWordWinButtons
void func_ov110_021bcc88(PMSIVMenu *wk) {
    func_ov110_021bcad8(wk);
    func_0204b3dc(wk->returnButton, TRUE);
    func_0204b878(wk->returnButton, 1);
    wk->returnSeq = RETURN_SEQ_NONE;
}

// pokebw2's PMSIVMenu_UpdateSentenceType
void func_ov110_021bcca8(PMSIVMenu *wk) {
    int prev = -1;
    int cur = -1;
    u32 type = func_ov110_021bf6e4(wk->mwk);
    int i;
    u16 anim;

    for (i = 1; i <= PMSIV_MENU_SENTENCE_TYPE_COUNT; i++) {
        anim = func_0204b898(wk->typeButtons[i]);
        if (anim == data_ov110_021c38df[i][1]) {
            prev = i;
            break;
        }
    }
    for (i = 0; i < PMSIV_MENU_SENTENCE_TYPE_COUNT; i++) {
        if (i == type) {
            cur = data_ov110_021c38d4[i];
            break;
        }
    }
    if (prev >= 0 && prev != cur) {
        func_ov110_021bd470(wk, prev);
    }
    for (i = 0; i < PMSIV_MENU_SENTENCE_TYPE_COUNT; i++) {
        func_ov110_021bd0f8(wk, data_ov110_021c38d4[i], i == type ? TRUE : FALSE);
    }
    if (cur >= 0 && prev != cur) {
        func_ov110_021bd448(wk, cur);
    }
}

// pokebw2's PMSIVMenu_UpdateEditButtons
void func_ov110_021bcd44(PMSIVMenu *wk) {
    if (func_ov110_021bf824(wk->mwk)) {
        func_020282ac(wk->buttons[0], wk->taskMenuRes[0], PMSIV_MENU_BUTTON_PALETTE,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    } else {
        func_020282ac(wk->buttons[0], wk->taskMenuRes[0], PMSIV_MENU_BUTTON_PALETTE_OFF,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    }
}

// pokebw2's PMSIVMenu_UpdateSearchButtons
void func_ov110_021bcd80(PMSIVMenu *wk) {
    if (func_ov110_021bf7fc(wk->mwk)) {
        func_020282ac(wk->buttons[0], wk->taskMenuRes[0], PMSIV_MENU_BUTTON_PALETTE,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    } else {
        func_020282ac(wk->buttons[0], wk->taskMenuRes[0], PMSIV_MENU_BUTTON_PALETTE_OFF,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    }
    if (func_ov110_021bf808(wk->mwk)) {
        func_020282ac(wk->buttons[1], wk->taskMenuRes[1], PMSIV_MENU_BUTTON_PALETTE,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    } else {
        func_020282ac(wk->buttons[1], wk->taskMenuRes[1], PMSIV_MENU_BUTTON_PALETTE_OFF,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    }
}

// pokebw2's PMSIVMenu_StartModeButton
static void func_ov110_021bcde0(PMSIVMenu *wk) {
    if (func_ov110_021bf6dc(wk->mwk) == 1) {
        func_ov110_021bcfbc(wk, 2, TRUE);
    }
}

// pokebw2's PMSIVMenu_WaitModeButton
static BOOL func_ov110_021bcdfc(PMSIVMenu *wk) {
    if (func_ov110_021bf6dc(wk->mwk) == 1) {
        if (func_ov110_021bcff0(wk, 2)) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

// pokebw2's PMSIVMenu_StartBackButton
static void func_ov110_021bce24(PMSIVMenu *wk) {
    func_ov110_021bd0f8(wk, PMSIV_MENU_TYPE_BUTTON_BACK, TRUE);
}

// pokebw2's PMSIVMenu_WaitBackButton
static BOOL func_ov110_021bce30(PMSIVMenu *wk) {
    if (!func_0204b98c(wk->typeButtons[PMSIV_MENU_TYPE_BUTTON_BACK])) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSIVMenu_EndBackButton
static void func_ov110_021bce48(PMSIVMenu *wk) {
    func_ov110_021bd0f8(wk, PMSIV_MENU_TYPE_BUTTON_BACK, FALSE);
}

// pokebw2's PMSIVMenu_StartSearchButton
static void func_ov110_021bce54(PMSIVMenu *wk) {
    if (func_ov110_021bf6dc(wk->mwk) == 1) {
        func_ov110_021bcfbc(wk, 1, TRUE);
    }
}

// pokebw2's PMSIVMenu_WaitSearchButton
static BOOL func_ov110_021bce70(PMSIVMenu *wk) {
    if (func_ov110_021bf6dc(wk->mwk) == 1) {
        if (func_ov110_021bcff0(wk, 1)) {
            func_020282a4(wk->buttons[1]);
            return TRUE;
        }
    } else {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSIVMenu_StartEraseButton
static void func_ov110_021bcea0(PMSIVMenu *wk) {
    if (func_ov110_021bf6dc(wk->mwk) == 1) {
        func_ov110_021bcfbc(wk, 0, TRUE);
    }
}

// pokebw2's PMSIVMenu_WaitEraseButton
static BOOL func_ov110_021bcebc(PMSIVMenu *wk) {
    if (func_ov110_021bf6dc(wk->mwk) == 1) {
        return func_ov110_021bcff0(wk, 0) ? TRUE : FALSE;
    }
    return TRUE;
}

// pokebw2's PMSIVMenu_StartButton
void func_ov110_021bcee4(PMSIVMenu *wk, u32 button) {
    switch (button) {
    case PMSIV_MENU_BUTTON_MODE:
        func_ov110_021bcde0(wk);
        break;
    case PMSIV_MENU_BUTTON_BACK:
        func_ov110_021bce24(wk);
        break;
    case PMSIV_MENU_BUTTON_SEARCH:
        func_ov110_021bce54(wk);
        break;
    case PMSIV_MENU_BUTTON_ERASE:
        func_ov110_021bcea0(wk);
        break;
    }
}

// pokebw2's PMSIVMenu_SetEditButton
void func_ov110_021bcf18(PMSIVMenu *wk, u32 which) {
    switch (which) {
    case 0:
        func_ov110_021bd0f8(wk, 0, TRUE);
        break;
    case 1:
        func_ov110_021bd0f8(wk, 6, TRUE);
        break;
    }
}

// pokebw2's PMSIVMenu_WaitButton
BOOL func_ov110_021bcf38(PMSIVMenu *wk, u32 button) {
    switch (button) {
    case PMSIV_MENU_BUTTON_MODE:
        return func_ov110_021bcdfc(wk);
    case PMSIV_MENU_BUTTON_BACK:
        return func_ov110_021bce30(wk);
    case PMSIV_MENU_BUTTON_SEARCH:
        return func_ov110_021bce70(wk);
    case PMSIV_MENU_BUTTON_ERASE:
        return func_ov110_021bcebc(wk);
    }
    return TRUE;
}

// pokebw2's PMSIVMenu_EndButton
void func_ov110_021bcf70(PMSIVMenu *wk, u32 button) {
    if (button == PMSIV_MENU_BUTTON_BACK) {
        func_ov110_021bce48(wk);
    }
}

// pokebw2's PMSIVMenu_SetCursor
void func_ov110_021bcf7c(PMSIVMenu *wk, u8 pos, BOOL active) {
    int i;

    if (wk->buttons[pos] != NULL) {
        for (i = 0; i < 3; i++) {
            if (wk->buttons[i] != NULL) {
                func_02028218(wk->buttons[i], FALSE);
            }
        }
        func_02028218(wk->buttons[pos], active);
    }
}

// pokebw2's PMSIVMenu_FlashButton
void func_ov110_021bcfbc(PMSIVMenu *wk, u8 pos, BOOL flash) {
    int i;

    for (i = 0; i < 3; i++) {
        if (wk->buttons[i] != NULL) {
            func_02028218(wk->buttons[i], FALSE);
        }
    }
    if (wk->buttons[pos] != NULL) {
        func_0202822c(wk->buttons[pos], flash);
    }
}

// pokebw2's PMSIVMenu_IsFlashFinished
BOOL func_ov110_021bcff0(PMSIVMenu *wk, u8 pos) {
    return func_02028230(wk->buttons[pos]);
}

// pokebw2's PMSIVMenu_StartReturn
BOOL func_ov110_021bd000(PMSIVMenu *wk) {
    if (func_0204b3f4(wk->returnButton) && wk->returnSeq == RETURN_SEQ_NONE) {
        func_0204b878(wk->returnButton, 9);
        wk->returnSeq = RETURN_SEQ_START;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSIVMenu_WaitReturn
BOOL func_ov110_021bd028(PMSIVMenu *wk) {
    if (func_0204b3f4(wk->returnButton) && wk->returnSeq != RETURN_SEQ_END) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's PMSIVMenu_SetupTypeButtons
static void func_ov110_021bd044(PMSIVMenu *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_PMSI, HEAPID_PMS_INPUT);
    int i;

    func_ov110_021bac64(wk->vwk, &wk->typeButtonRes, 0, 0);
    wk->typeButtonRes.chars = func_0204a6c8(arc, 13, FALSE, CLACT_VRAM_MAIN, HEAPID_PMS_INPUT);
    wk->typeButtonRes.cellAnims = func_0204af28(arc, 35, 41, HEAPID_PMS_INPUT);
    GFL_ArcToolFree(arc);
    for (i = 0; i < PMSIV_MENU_TYPE_BUTTON_COUNT; i++) {
        s16 x = i * 24;

        if (i == PMSIV_MENU_TYPE_BUTTON_BACK) {
            x = 0;
        }
        wk->typeButtons[i] = func_ov110_021bac78(wk->vwk, &wk->typeButtonRes, x, 168, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
        func_ov110_021bd0f8(wk, i, FALSE);
        func_0204b3dc(wk->typeButtons[i], FALSE);
    }
}

// pokebw2's PMSIVMenu_FreeTypeButtons
static void func_ov110_021bd0d4(PMSIVMenu *wk) {
    int i;

    for (i = 0; i < PMSIV_MENU_TYPE_BUTTON_COUNT; i++) {
        func_0204b3b4(wk->typeButtons[i]);
    }
    func_0204a8d4(wk->typeButtonRes.chars);
    func_0204afd8(wk->typeButtonRes.cellAnims);
}

// pokebw2's PMSIVMenu_SetTypeButtonAnim
static void func_ov110_021bd0f8(PMSIVMenu *wk, u32 button, BOOL on) {
    func_0204b878(wk->typeButtons[button], data_ov110_021c38df[button][on]);
}

// pokebw2's PMSIVMenu_SetupArrows
static void func_ov110_021bd114(PMSIVMenu *wk) {
    u8 i;

    for (i = 0; i < 2; i++) {
        const PMSIVMenuArrow *arrow = &data_ov110_021c38d9[i];

        wk->arrows[i] = func_ov110_021bac78(wk->vwk, &wk->typeButtonRes, data_ov110_021c38d9[i].x, arrow->y, 0,
                                          NNS_G2D_VRAM_TYPE_2DMAIN);
        func_0204b878(wk->arrows[i], arrow->anim);
        func_0204b3dc(wk->arrows[i], FALSE);
    }
}

// pokebw2's PMSIVMenu_FreeArrows
static void func_ov110_021bd174(PMSIVMenu *wk) {
    u8 i;

    for (i = 0; i < 2; i++) {
        func_0204b3b4(wk->arrows[i]);
    }
}

// pokebw2's PMSIVMenu_SetupReturnButton
static void func_ov110_021bd194(PMSIVMenu *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_PMS_INPUT);
    ClActorSetup setup;

    wk->returnPalette = func_0204ac18(arc, func_02027614(), CLACT_VRAM_MAIN, 0x180, 0, 3, HEAPID_PMS_INPUT);
    wk->returnChars = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_MAIN, HEAPID_PMS_INPUT);
    wk->returnCellAnims = func_0204af28(arc, func_0202761c(1), func_02027620(1), HEAPID_PMS_INPUT);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.x = 232;
    setup.y = 168;
    setup.sequence = 1;
    setup.priority = 0;
    setup.bgPriority = 3;
    wk->returnButton = func_0204b294(func_ov110_021bac40(wk->vwk), wk->returnChars, wk->returnPalette,
                                     wk->returnCellAnims, &setup, CLACT_SURFACE_MAIN, HEAPID_PMS_INPUT);
    func_0204b92c(wk->returnButton, TRUE);
    func_0204b3dc(wk->returnButton, FALSE);
    wk->returnSeq = RETURN_SEQ_NONE;
}

// pokebw2's PMSIVMenu_ShowReturnButton
static void func_ov110_021bd248(PMSIVMenu *wk) {
    func_0204b3dc(wk->returnButton, TRUE);
    func_0204b878(wk->returnButton, 1);
    wk->returnSeq = RETURN_SEQ_NONE;
}

// pokebw2's PMSIVMenu_SetupSearchButtons
static void func_ov110_021bd264(PMSIVMenu *wk) {
    int i;

    for (i = 0; i < 3; i++) {
        AppTaskMenuItem *item = &wk->items[i];

        wk->items[i].str = GFL_MsgDataLoadStrbufNew(wk->msgData, i + 18);
        wk->items[i].color = PMSIV_MENU_TEXT_COLOR;
        wk->items[i].type = i == 2 ? APP_TASKMENU_ITEM_RETURN : 0;
        wk->buttons[i] = func_02027fec(wk->taskMenuRes[i], item, i * 9 + 5, 21, 9, HEAPID_PMS_INPUT);
        func_020282ac(wk->buttons[i], wk->taskMenuRes[i], PMSIV_MENU_BUTTON_PALETTE,
                                  PMSIV_MENU_BUTTON_COLOR_1, PMSIV_MENU_BUTTON_COLOR_2);
    }
    func_ov110_021bcd80(wk);
    GFL_BGSysLoadScr(0);
}

// pokebw2's PMSIVMenu_SetupColors
static void func_ov110_021bd2f0(PMSIVMenu *wk) {
    NNSG2dPaletteData *palette;
    void *buf = GFL_G2DIOReadNCLR(ARCID_PMSI, 7, &palette, HEAPID_PMS_INPUT);
    u16 *colors = palette->pRawData;

    MI_CpuCopy8(&colors[75], wk->pulseFrom, sizeof(wk->pulseFrom));
    MI_CpuCopy8(&colors[59], wk->pulseTo, sizeof(wk->pulseTo));
    GFL_HeapFree(buf);
    MI_CpuCopy8(wk->pulseFrom, wk->pulse, sizeof(wk->pulse));
    wk->pulseAngle = 0;
}

// pokebw2's PMSIVMenu_FreeColors
static void func_ov110_021bd344(PMSIVMenu *wk) {
}

// pokebw2's PMSIVMenu_UpdateColors
static void func_ov110_021bd348(PMSIVMenu *wk) {
    fx16 ratio;
    u8 i;

    if (wk->pulseAngle + 0x400 >= 0x10000) {
        wk->pulseAngle = wk->pulseAngle - 0xfc00;
    } else {
        wk->pulseAngle = wk->pulseAngle + 0x400;
    }
    ratio = (FX_CosIdx(wk->pulseAngle) + FX32_ONE) / 2;
    for (i = 0; i < PMSIV_MENU_PULSE_COLOR_COUNT; i++) {
        u8 fromR = wk->pulseFrom[i] & GX_RGB_R_MASK;
        u8 fromG = (wk->pulseFrom[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        u8 fromB = (wk->pulseFrom[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        u8 toR = wk->pulseTo[i] & GX_RGB_R_MASK;
        u8 toG = (wk->pulseTo[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        u8 toB = (wk->pulseTo[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        u8 r = fromR + (((toR - fromR) * ratio) >> FX32_SHIFT);
        u8 g = fromG + (((toG - fromG) * ratio) >> FX32_SHIFT);
        u8 b = fromB + (((toB - fromB) * ratio) >> FX32_SHIFT);

        wk->pulse[i] = GX_RGB(r, g, b);
    }
    NNS_GfdRegisterNewVramTransferTask(14, 0x56, wk->pulse, sizeof(wk->pulse));
}

// pokebw2's PMSIVMenu_StartTypeButtonPulse
static void func_ov110_021bd448(PMSIVMenu *wk, u32 button) {
    func_0204b6f4(wk->typeButtons[button], 2, 1);
    MI_CpuCopy8(wk->pulseFrom, wk->pulse, sizeof(wk->pulse));
    wk->pulseAngle = 0;
}

// pokebw2's PMSIVMenu_EndTypeButtonPulse
static void func_ov110_021bd470(PMSIVMenu *wk, u32 button) {
    func_0204b6f4(wk->typeButtons[button], 6, 1);
}
