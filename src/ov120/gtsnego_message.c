// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov195/gtsnego_message.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// GTS Negotiations' messages: the top screen's message window, the lower screen's menus with their labels and touch
// buttons, the names of the list of people of the Pal Pad, and the partner's name, trades, place and greeting. The
// name is the ROM's string, from GFL_HeapAllocate's call. Function names are ours.

#include "types.h"
#include "app/gtsnego_local.h"
#include "constants/arc.h"
#include "field/unity_tower.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "save/player_info.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/country_region.h"
#include "system/gf_font.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/0045.h"

#define GTSNEGO_LABEL_COUNT 12

// The colors of the buttons' text
#define GTSNEGO_MENU_COLOR PRINT_COLOR(14, 15, 0)
#define GTSNEGO_BUTTON_COLOR PRINT_COLOR(14, 15, 3)

struct GtsNegoMessage {
    // The frame characters, loaded at the end of BG 6 and of BG 1
    u32 subFrameChars;
    u32 mainFrameChars;
    // The touch buttons of the lower screen's menu
    ButtonMan *buttons;
    MsgData *msgData;
    WordSet *wordSet;
    Font *font;
    StrBuf *strbuf;
    StrBuf *messageStrbuf;
    StrBuf *formatStrbuf;
    // The labels of the search's settings
    int levelLabel;
    int myKindLabel;
    int wantKindLabel;
    BmpWin *messageWin;
    BmpWin *unk34;
    BmpWin *labels[GTSNEGO_LABEL_COUNT];
    BmpWin *titleWin;
    BmpWin *friendWins[GTSNEGO_LIST_ROWS];
    // The partner's windows
    BmpWin *locationWin;
    BmpWin *phraseWin;
    BmpWin *tradesLabelWin;
    BmpWin *tradesWin;
    BmpWin *headingWin;
    BmpWin *nameWin;
    PrintStream *stream;
    TCBExManager *tcbManager;
    PrintQueue *printQueue;
    AppTaskMenuItem items[2];
    AppTaskMenuRes *menuRes;
    AppTaskMenuWin *cancelButton;
    HeapID heapId;
    PMSDraw *pmsDraw;
};

static void func_ov120_021d7710(GtsNegoMessage *msg);

// The messages of the level ranges and of the kinds of Pokémon
static u16 data_ov120_021d8bd8[] = {
    Bank0045_Text_007,
    Bank0045_Text_004,
    Bank0045_Text_005,
    Bank0045_Text_006,
};

static u16 data_ov120_021d8be0[] = {
    Bank0045_Text_012, Bank0045_Text_008, Bank0045_Text_009, Bank0045_Text_010, Bank0045_Text_011,
};

// The main menu's buttons
// pokebw2's sGtsNegoMessageMainRects
static const TouchRect data_ov120_021d8af8[] = {
    { 56, 80, 64, 192 },
    { 104, 128, 64, 192 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// The arrows of the search's settings, left then right of each
// pokebw2's sGtsNegoMessageSearchRects
static const TouchRect data_ov120_021d8b04[] = {
    { 43, 69, 83, 109 },   { 43, 69, 227, 253 },   { 83, 109, 107, 133 },       { 83, 109, 227, 253 },
    { 123, 149, 107, 133 }, { 123, 149, 227, 253 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's GtsNegoMessage_FreeButtons
static void func_ov120_021d7710(GtsNegoMessage *msg) {
    if (msg->buttons != NULL) {
        func_02051c6c(msg->buttons);
        msg->buttons = NULL;
    }
}

// pokebw2's GtsNegoMessage_Create
GtsNegoMessage *func_ov120_021d7724(HeapID heapId, u32 bank) {
    GtsNegoMessage *msg = GFL_HeapAllocate(heapId, sizeof(GtsNegoMessage), TRUE, "gtsnego_message.c", 169);

    msg->heapId = heapId;
    BmpWin_InitAllocator(msg->heapId);
    func_0201dd60();
    msg->tcbManager = GFL_TCBExMgrCreate(msg->heapId, msg->heapId, 1, 0);
    msg->printQueue = func_0201c440(msg->heapId);
    msg->messageStrbuf = GFL_StrBufCreate(240, msg->heapId);
    msg->strbuf = GFL_StrBufCreate(240, msg->heapId);
    msg->formatStrbuf = GFL_StrBufCreate(240, msg->heapId);
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, msg->heapId);
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, bank, msg->heapId);
    msg->wordSet = GFL_WordSetSystemCreate(6, 64, msg->heapId);
    msg->menuRes = func_02027f60(5, 9, msg->font, msg->printQueue, msg->heapId);
    msg->mainFrameChars = LoadCursorImageEndOfHeap(1, 12, 0, msg->heapId);
    msg->subFrameChars = LoadCursorImageEndOfHeap(6, 12, 0, msg->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x160, 0x20, msg->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0x160, 0x20, msg->heapId);
    return msg;
}

// pokebw2's GtsNegoMessage_Update
void func_ov120_021d7858(GtsNegoMessage *msg) {
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TCBExMgrUpdate(msg->tcbManager);
    func_0201c4e4(msg->printQueue);
    if (msg->cancelButton != NULL) {
        func_02028178(msg->cancelButton);
    }
    if (msg->pmsDraw != NULL) {
        func_0202dbc4(msg->pmsDraw);
    }
}

// pokebw2's GtsNegoMessage_Free
void func_ov120_021d7898(GtsNegoMessage *msg) {
    int i;

    GFL_BGSysFreeCharMemory(1, CHAR_POS(msg->mainFrameChars), CHAR_SIZE(msg->mainFrameChars));
    GFL_BGSysFreeCharMemory(6, CHAR_POS(msg->subFrameChars), CHAR_SIZE(msg->subFrameChars));
    func_ov120_021d7710(msg);
    func_0201dd68();
    GFL_MsgDataFree(msg->msgData);
    GFL_FontFree(msg->font);
    GFL_StrBufFree(msg->strbuf);
    GFL_StrBufFree(msg->messageStrbuf);
    GFL_StrBufFree(msg->formatStrbuf);
    func_ov120_021d846c(msg);
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    GFL_WordSetSystemFree(msg->wordSet);
    if (msg->cancelButton != NULL) {
        func_02028148(msg->cancelButton);
        msg->cancelButton = NULL;
    }
    func_02027fd8(msg->menuRes);
    GFL_TCBExMgrFree(msg->tcbManager);
    func_0201c6ec(msg->printQueue);
    func_0201c4c0(msg->printQueue);
    if (msg->messageWin != NULL) {
        BmpWin_Free(msg->messageWin);
    }
    if (msg->locationWin != NULL) {
        BmpWin_Free(msg->locationWin);
    }
    if (msg->nameWin != NULL) {
        BmpWin_Free(msg->nameWin);
    }
    if (msg->tradesWin != NULL) {
        BmpWin_Free(msg->tradesWin);
    }
    if (msg->tradesLabelWin != NULL) {
        BmpWin_Free(msg->tradesLabelWin);
    }
    if (msg->headingWin != NULL) {
        BmpWin_Free(msg->headingWin);
    }
    if (msg->phraseWin != NULL) {
        BmpWin_Free(msg->phraseWin);
    }
    if (msg->unk34 != NULL) {
        BmpWin_Free(msg->unk34);
    }
    if (msg->titleWin != NULL) {
        BmpWin_Free(msg->titleWin);
    }
    for (i = 0; i < GTSNEGO_LABEL_COUNT; i++) {
        if (msg->labels[i] != NULL) {
            BmpWin_Free(msg->labels[i]);
        }
    }
    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (msg->friendWins[i] != NULL) {
            BmpWin_Free(msg->friendWins[i]);
        }
    }
    BmpWin_FreeAllocator();
    GFL_HeapFree(msg);
}

// pokebw2's GtsNegoMessage_Print
void func_ov120_021d79f4(GtsNegoMessage *msg, u32 msgNo) {
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->messageStrbuf);
    if (msg->messageWin == NULL) {
        msg->messageWin = BmpWin_CreateDynamic(1, 1, 19, 30, 4, 11, TRUE);
    }
    window = msg->messageWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, msg->messageStrbuf, msg->font);
    BmpWin_DrawFrame(window, WINFRAME_TRANSFER_VBLANK, msg->mainFrameChars, 12);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(1);
}

// pokebw2's GtsNegoMessage_PrintShort
void func_ov120_021d7a74(GtsNegoMessage *msg, u32 msgNo) {
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->strbuf);
    if (msg->messageWin == NULL) {
        msg->messageWin = BmpWin_CreateDynamic(1, 1, 21, 30, 2, 11, TRUE);
    }
    window = msg->messageWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, msg->strbuf, msg->font);
    BmpWin_DrawFrame(window, WINFRAME_TRANSFER_VBLANK, msg->mainFrameChars, 12);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(1);
}

// Whether no message is being printed a character at a time, which A continues
// pokebw2's GtsNegoMessage_IsDone
BOOL func_ov120_021d7af4(GtsNegoMessage *msg) {
    if (msg->stream != NULL) {
        switch (func_0201ce30(msg->stream)) {
        case PRINT_STREAM_DONE:
            func_0201ce48(msg->stream);
            msg->stream = NULL;
            break;
        case PRINT_STREAM_PAUSED:
            if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
                func_0201ce38(msg->stream);
            }
            break;
        }
        return FALSE;
    }
    return TRUE;
}

// pokebw2's GtsNegoMessage_ClearWindow
void func_ov120_021d7b38(GtsNegoMessage *msg) {
    if (msg->messageWin != NULL) {
        func_0201f7a8(msg->messageWin, WINFRAME_TRANSFER_NONE);
        func_020456f8(msg->messageWin);
        GFL_BGSysQueueScrLoad(1);
        BmpWin_Free(msg->messageWin);
        msg->messageWin = NULL;
    }
}

// pokebw2's GtsNegoMessage_CreateYesNo
AppTaskMenu *func_ov120_021d7b60(GtsNegoMessage *msg, u32 layout) {
    AppTaskMenuInit init;
    AppTaskMenu *menu;

    init.heapId = msg->heapId;
    init.itemCount = NELEMS(msg->items);
    init.items = msg->items;
    switch (layout) {
    case 0:
        init.x = 32;
        init.y = 14;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        break;
    case 1:
        init.x = 32;
        init.y = 24;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        break;
    }
    init.width = 13;
    init.height = 3;
    msg->items[0].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_025, msg->items[0].str);
    msg->items[0].color = GTSNEGO_MENU_COLOR;
    msg->items[1].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_026, msg->items[1].str);
    msg->items[1].color = GTSNEGO_MENU_COLOR;
    menu = func_02027778(&init, msg->menuRes);
    GFL_StrBufFree(msg->items[0].str);
    GFL_StrBufFree(msg->items[1].str);
    return menu;
}

// pokebw2's GtsNegoMessage_UpdateButtons
void func_ov120_021d7c10(GtsNegoMessage *msg) {
    func_02051c88(msg->buttons);
}

// pokebw2's GtsNegoMessage_FreeLabels
void func_ov120_021d7c1c(GtsNegoMessage *msg) {
    int i;

    for (i = 0; i < GTSNEGO_LABEL_COUNT; i++) {
        if (msg->labels[i] != NULL) {
            BmpWin_Free(msg->labels[i]);
            msg->labels[i] = NULL;
        }
    }
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
}

// Adds a label of a line on a BG, returning which it is
// pokebw2's GtsNegoMessage_AddLabel
int func_ov120_021d7c48(GtsNegoMessage *msg, u32 msgNo, int x, int y, int width, int bg) {
    int i;

    for (i = 0; i < GTSNEGO_LABEL_COUNT; i++) {
        if (msg->labels[i] == NULL) {
            BmpWin *window;

            msg->labels[i] = BmpWin_CreateDynamic(bg, x, y, width, 2, 11, TRUE);
            window = msg->labels[i];
            GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
            GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->strbuf);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 2, 2, msg->strbuf, msg->font);
            BmpWin_FlushChar(window);
            BmpWin_FlushMap(window);
            return i;
        }
    }
    return 0;
}

// pokebw2's GtsNegoMessage_AddMenuLabel
int func_ov120_021d7cd4(GtsNegoMessage *msg, u32 msgNo, int x, int y) {
    return func_ov120_021d7c48(msg, msgNo, x, y, 20, 6);
}

// pokebw2's GtsNegoMessage_SetLabel
void func_ov120_021d7ce8(GtsNegoMessage *msg, u32 msgNo, int label) {
    BmpWin *window = msg->labels[label];

    if (window != NULL) {
        GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
        GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->strbuf);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 2, 2, msg->strbuf, msg->font);
        BmpWin_FlushChar(window);
    }
}

// pokebw2's GtsNegoMessage_QueueLabelsLoad
void func_ov120_021d7d30(GtsNegoMessage *msg) {
    GFL_BGSysQueueScrLoad(6);
}

// pokebw2's GtsNegoMessage_ClearLabels
void func_ov120_021d7d3c(GtsNegoMessage *msg) {
    int i;

    for (i = 0; i < GTSNEGO_LABEL_COUNT; i++) {
        if (msg->labels[i] != NULL) {
            GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(msg->labels[i]));
            func_0201f7a8(msg->labels[i], WINFRAME_TRANSFER_NONE);
            func_020456f8(msg->labels[i]);
        }
    }
}

// pokebw2's GtsNegoMessage_ShowMainMenu
void func_ov120_021d7d6c(GtsNegoMessage *msg, ButtonManCallback callback, void *work) {
    func_ov120_021d7c1c(msg);
    func_ov120_021d7c48(msg, Bank0045_Text_000, 8, 7, 16, 6);
    func_ov120_021d7c48(msg, Bank0045_Text_001, 8, 13, 16, 6);
    func_ov120_021d7d30(msg);
    func_ov120_021d7710(msg);
    msg->buttons = func_02051b84(data_ov120_021d8af8, callback, work, msg->heapId);
}

void func_ov120_021d7dc8(GtsNegoMessage *msg, ButtonManCallback callback, void *work, int level, int myKind,
                                   int wantKind) {
    func_ov120_021d7c1c(msg);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    func_ov120_021d7cd4(msg, Bank0045_Text_002, 0, 0);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    func_ov120_021d7cd4(msg, Bank0045_Text_003, 3, 6);
    func_ov120_021d7cd4(msg, Bank0045_Text_023, 1, 11);
    func_ov120_021d7cd4(msg, Bank0045_Text_024, 1, 16);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    msg->levelLabel = func_ov120_021d7cd4(msg, data_ov120_021d8bd8[level], 13, 6);
    msg->myKindLabel = func_ov120_021d7cd4(msg, data_ov120_021d8be0[myKind], 16, 11);
    msg->wantKindLabel = func_ov120_021d7cd4(msg, data_ov120_021d8be0[wantKind], 16, 16);
    func_ov120_021d7d30(msg);
    func_ov120_021d7710(msg);
    msg->buttons = func_02051b84(data_ov120_021d8b04, callback, work, msg->heapId);
}

// pokebw2's GtsNegoMessage_SetLevel
void func_ov120_021d7e88(GtsNegoMessage *msg, int level) {
    func_ov120_021d7ce8(msg, data_ov120_021d8bd8[level], msg->levelLabel);
}

// pokebw2's GtsNegoMessage_SetMyKind
void func_ov120_021d7e9c(GtsNegoMessage *msg, int kind) {
    func_ov120_021d7ce8(msg, data_ov120_021d8be0[kind], msg->myKindLabel);
}

// pokebw2's GtsNegoMessage_SetWantKind
void func_ov120_021d7eb0(GtsNegoMessage *msg, int kind) {
    func_ov120_021d7ce8(msg, data_ov120_021d8be0[kind], msg->wantKindLabel);
}

// pokebw2's GtsNegoMessage_HideMenu
void func_ov120_021d7ec4(GtsNegoMessage *msg) {
    func_ov120_021d7710(msg);
}

// A button as wide as its text, centered
// pokebw2's GtsNegoMessage_CreateButton
AppTaskMenuWin *func_ov120_021d7ecc(GtsNegoMessage *msg, u32 msgNo) {
    AppTaskMenuWin *button;
    int width;

    msg->items[0].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->items[0].str);
    msg->items[0].color = GTSNEGO_BUTTON_COLOR;
    msg->items[0].type = 0;
    width = ((int)GFL_FontGetBlockWidth(msg->items[0].str, msg->font, 0) + 32) / 16;
    button = func_0202800c(msg->menuRes, &msg->items[0], 16 - width, 21, width * 2, 3, FALSE, TRUE,
                                     msg->heapId);
    GFL_StrBufFree(msg->items[0].str);
    return button;
}

// pokebw2's GtsNegoMessage_PrintHeading
void func_ov120_021d7f5c(GtsNegoMessage *msg, u32 msgNo) {
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->strbuf);
    if (msg->headingWin == NULL) {
        msg->headingWin = BmpWin_CreateDynamic(1, 1, 13, 14, 3, 4, TRUE);
    }
    window = msg->headingWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(15, 14, 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 2, 3, msg->strbuf, msg->font);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
}

// Prints where the partner lives, or nowhere when the place is not a valid one
// pokebw2's GtsNegoMessage_PrintLocation
void func_ov120_021d7fd0(GtsNegoMessage *msg, PlayerInfo *partner) {
    BmpWin *window;
    u8 country = UnityTowerVisitor_GetCountry(partner);
    u8 province = UnityTowerVisitor_GetProvince(partner);
    u8 validCountry = func_02025c1c(country, province, TrainerInfo_GetRegion(partner));
    u8 validProvince = func_02025c30(country, province, TrainerInfo_GetRegion(partner));

    if (country == validCountry && province == validProvince) {
        loadCountryToStrbuf(msg->wordSet, 0, UnityTowerVisitor_GetCountry(partner));
        loadCountryAreaToStrbuf(msg->wordSet, 1, UnityTowerVisitor_GetCountry(partner),
                                UnityTowerVisitor_GetProvince(partner));
    } else {
        loadCountryToStrbuf(msg->wordSet, 0, 0);
        loadCountryAreaToStrbuf(msg->wordSet, 1, 0, 0);
    }
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_034, msg->formatStrbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, msg->formatStrbuf);
    if (msg->locationWin == NULL) {
        msg->locationWin = BmpWin_CreateDynamic(1, 1, 15, 30, 5, 11, TRUE);
    }
    window = msg->locationWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 5, 5, msg->strbuf, msg->font);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(1);
}

// pokebw2's GtsNegoMessage_ClearLocation
void func_ov120_021d80d4(GtsNegoMessage *msg) {
    if (msg->locationWin != NULL) {
        GFL_BitmapFill(BmpWin_GetBitmap(msg->locationWin), 0);
        BmpWin_FlushChar(msg->locationWin);
        func_020456f8(msg->locationWin);
        GFL_BGSysQueueScrLoad(1);
        BmpWin_Free(msg->locationWin);
        msg->locationWin = NULL;
    }
}

// Prints a person's name, ID and country in a row of the list
// pokebw2's GtsNegoMessage_PrintFriend
int func_ov120_021d8118(GtsNegoMessage *msg, PlayerInfo *friend, int row) {
    BmpWin *window;

    if (msg->friendWins[row] == NULL) {
        msg->friendWins[row] = BmpWin_CreateDynamic(6, 6, row * 6 + 1, 24, 4, 11, TRUE);
    }
    window = msg->friendWins[row];
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_StrBufClear(msg->formatStrbuf);
    GFL_StrBufClear(msg->strbuf);
    copyVarForText(msg->wordSet, 0, friend);
    WordSetNumber(msg->wordSet, 1, getTrainerID(friend), 5, 2, TRUE);
    loadCountryToStrbuf(msg->wordSet, 2, UnityTowerVisitor_GetCountry(friend));
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_028, msg->formatStrbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, msg->formatStrbuf);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, msg->strbuf, msg->font);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    return 0;
}

// The list's title, and the first people in the rows on the screen
// pokebw2's GtsNegoMessage_ShowFriendList
void func_ov120_021d81e0(GtsNegoMessage *msg, GameData *gameData) {
    int i = 0;

    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    func_ov120_021d7c48(msg, Bank0045_Text_030, 0, 0, 20, 5);
    for (; i < GTSNEGO_LIST_SHOWN; i++) {
        PlayerInfo *friend = func_ov120_021d4f70(gameData, i);

        if (friend != NULL) {
            func_ov120_021d8118(msg, friend, i + 2);
        }
    }
    GFL_BGSysQueueScrLoad(5);
}

// pokebw2's GtsNegoMessage_PrintLastFriend
void func_ov120_021d822c(GtsNegoMessage *msg, GameData *gameData, int index) {
    func_ov120_021d8118(msg, func_ov120_021d4f70(gameData, index), GTSNEGO_LIST_ROWS - 1);
    GFL_BGSysQueueScrLoad(6);
}

// Moves the names a row up, the first row's going away
// pokebw2's GtsNegoMessage_ShiftFriendsUp
void func_ov120_021d824c(GtsNegoMessage *msg) {
    int i;

    if (msg->friendWins[0] != NULL) {
        BmpWin_Free(msg->friendWins[0]);
        msg->friendWins[0] = NULL;
    }
    for (i = 1; i < GTSNEGO_LIST_ROWS; i++) {
        if (msg->friendWins[i] != NULL) {
            func_02045788(msg->friendWins[i], BmpWin_GetPosY(msg->friendWins[i]) - 6);
            BmpWin_FlushMap(msg->friendWins[i]);
        }
    }
    GFL_BGSysQueueScrLoad(6);
    for (i = 0; i < GTSNEGO_LIST_ROWS - 1; i++) {
        msg->friendWins[i] = msg->friendWins[i + 1];
    }
    msg->friendWins[GTSNEGO_LIST_ROWS - 1] = NULL;
}

// pokebw2's GtsNegoMessage_PrintFriends
void func_ov120_021d82a8(GtsNegoMessage *msg, GameData *gameData, int top) {
    int i;

    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (i + top >= 0) {
            PlayerInfo *friend = func_ov120_021d4f70(gameData, i + top);

            if (friend != NULL) {
                func_ov120_021d8118(msg, friend, i);
            }
        }
    }
    GFL_BGSysQueueScrLoad(6);
}

// pokebw2's GtsNegoMessage_PrintFirstFriend
void func_ov120_021d82d8(GtsNegoMessage *msg, GameData *gameData, int index) {
    func_ov120_021d8118(msg, func_ov120_021d4f70(gameData, index), 0);
    GFL_BGSysQueueScrLoad(6);
}

// Moves the names a row down, the last row's going away
// pokebw2's GtsNegoMessage_ShiftFriendsDown
void func_ov120_021d82f8(GtsNegoMessage *msg) {
    int i;

    if (msg->friendWins[GTSNEGO_LIST_ROWS - 1] != NULL) {
        BmpWin_Free(msg->friendWins[GTSNEGO_LIST_ROWS - 1]);
        msg->friendWins[GTSNEGO_LIST_ROWS - 1] = NULL;
    }
    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (msg->friendWins[i] != NULL) {
            func_02045788(msg->friendWins[i], BmpWin_GetPosY(msg->friendWins[i]) + 6);
            BmpWin_FlushMap(msg->friendWins[i]);
        }
    }
    GFL_BGSysQueueScrLoad(6);
    for (i = GTSNEGO_LIST_ROWS - 1; i > 0; i--) {
        msg->friendWins[i] = msg->friendWins[i - 1];
    }
    msg->friendWins[0] = NULL;
}

// pokebw2's GtsNegoMessage_CreateConfirm
AppTaskMenu *func_ov120_021d8358(GtsNegoMessage *msg, u32 layout) {
    AppTaskMenuInit init;
    AppTaskMenu *menu;

    init.heapId = msg->heapId;
    init.itemCount = NELEMS(msg->items);
    init.items = msg->items;
    switch (layout) {
    case 0:
        init.x = 32;
        init.y = 14;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        break;
    case 1:
        init.x = 32;
        init.y = 24;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        break;
    }
    init.width = 13;
    init.height = 3;
    msg->items[0].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_027, msg->items[0].str);
    msg->items[0].color = GTSNEGO_MENU_COLOR;
    msg->items[0].type = 0;
    msg->items[1].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_033, msg->items[1].str);
    msg->items[1].color = GTSNEGO_MENU_COLOR;
    msg->items[1].type = 0;
    menu = func_02027778(&init, msg->menuRes);
    GFL_StrBufFree(msg->items[0].str);
    GFL_StrBufFree(msg->items[1].str);
    return menu;
}

// pokebw2's GtsNegoMessage_FreeMenu
void func_ov120_021d8418(AppTaskMenu *menu) {
    if (menu != NULL) {
        G2_BlendNone();
        func_02027858(menu);
    }
}

// pokebw2's GtsNegoMessage_CreatePMSDraw
void func_ov120_021d8430(GtsNegoMessage *msg, GtsNegoDisp *disp) {
    if (msg->pmsDraw == NULL) {
        msg->pmsDraw = func_0202db0c(func_ov120_021d60fc(disp), CLACT_VRAM_MAIN, msg->printQueue, msg->font, 8,
                                      4, msg->heapId);
    }
}

// pokebw2's GtsNegoMessage_FreePMSDraw
void func_ov120_021d846c(GtsNegoMessage *msg) {
    if (msg->pmsDraw != NULL) {
        func_0202dc08(msg->pmsDraw);
        msg->pmsDraw = NULL;
    }
}

// pokebw2's GtsNegoMessage_PrintPhrase
void func_ov120_021d8484(GtsNegoMessage *msg, PMSData *phrase) {
    PMSDrawPos pos = { 5, 5 };
    BmpWin *window;

    if (msg->phraseWin == NULL) {
        msg->phraseWin = BmpWin_CreateDynamic(1, 1, 15, 30, 5, 11, TRUE);
    }
    window = msg->phraseWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    func_0202de5c(msg->pmsDraw, 0);
    func_0202de64(msg->pmsDraw, PRINT_COLOR(15, 2, 0));
    func_0202dc60(msg->pmsDraw, window, phrase, 0, &pos);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(1);
}

// pokebw2's GtsNegoMessage_PrintTitle
void func_ov120_021d8520(GtsNegoMessage *msg, u32 msgNo) {
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->strbuf);
    if (msg->titleWin == NULL) {
        msg->titleWin = BmpWin_CreateDynamic(1, 0, 0, 14, 2, 11, TRUE);
    }
    window = msg->titleWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 2, 2, msg->strbuf, msg->font);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(1);
}

// The search's cancel button
// pokebw2's sGtsNegoMessageCancelRect
static const TouchRect data_ov120_021d8af0[] = {
    { 160, 192, 80, 176 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's GtsNegoMessage_CreateCancelButton
void func_ov120_021d8590(GtsNegoMessage *msg, ButtonManCallback callback, void *work) {
    msg->items[0].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_015, msg->items[0].str);
    msg->items[0].color = GTSNEGO_BUTTON_COLOR;
    msg->items[0].type = 0;
    msg->cancelButton = func_0202800c(msg->menuRes, &msg->items[0], 10, 21, 12, 3, FALSE, TRUE, msg->heapId);
    if (!func_020355c4()) {
        func_02028218(msg->cancelButton, TRUE);
    }
    GFL_StrBufFree(msg->items[0].str);
    func_ov120_021d7710(msg);
    msg->buttons = func_02051b84(data_ov120_021d8af0, callback, work, msg->heapId);
}

// pokebw2's GtsNegoMessage_PressCancelButton
void func_ov120_021d8638(GtsNegoMessage *msg) {
    func_0202822c(msg->cancelButton, TRUE);
}

// pokebw2's GtsNegoMessage_FreeCancelButton
void func_ov120_021d8648(GtsNegoMessage *msg) {
    func_02028148(msg->cancelButton);
    GFL_BGSysQueueScrLoad(5);
    msg->cancelButton = NULL;
}

// pokebw2's GtsNegoMessage_IsCancelPressDone
BOOL func_ov120_021d8664(GtsNegoMessage *msg) {
    if (func_02028230(msg->cancelButton)) {
        func_ov120_021d8648(msg);
        return TRUE;
    }
    return FALSE;
}

// Prints how many trades the partner has made, under its label. msgNo is not used
// pokebw2's GtsNegoMessage_PrintTradeCount
void func_ov120_021d8684(GtsNegoMessage *msg, u32 msgNo, int count) {
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_040, msg->strbuf);
    if (msg->tradesLabelWin == NULL) {
        msg->tradesLabelWin = BmpWin_CreateDynamic(1, 21, 4, 10, 2, 4, TRUE);
    }
    window = msg->tradesLabelWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(15, 14, 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 3, msg->strbuf, msg->font);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    WordSetNumber(msg->wordSet, 0, count, 5, 1, TRUE);
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_036, msg->formatStrbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, msg->formatStrbuf);
    if (msg->tradesWin == NULL) {
        msg->tradesWin = BmpWin_CreateDynamic(1, 21, 6, 10, 3, 11, TRUE);
    }
    window = msg->tradesWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 5, msg->strbuf, msg->font);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
}

// pokebw2's GtsNegoMessage_PrintPartnerName
void func_ov120_021d8784(GtsNegoMessage *msg, PlayerInfo *partner) {
    BmpWin *window;

    copyVarForText(msg->wordSet, 0, partner);
    GFL_MsgDataLoadStrbuf(msg->msgData, Bank0045_Text_037, msg->formatStrbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, msg->formatStrbuf);
    if (msg->nameWin == NULL) {
        msg->nameWin = BmpWin_CreateDynamic(1, 11, 10, 10, 3, 11, TRUE);
    }
    window = msg->nameWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, msg->strbuf, msg->font);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
}

// pokebw2's GtsNegoMessage_ShowPartner
void func_ov120_021d880c(GtsNegoMessage *msg, PlayerInfo *partner, int count) {
    func_ov120_021d8784(msg, partner);
    func_ov120_021d8684(msg, Bank0045_Text_036, count);
    func_ov120_021d7f5c(msg, Bank0045_Text_041);
    func_ov120_021d7fd0(msg, partner);
    func_ov120_021d7a74(msg, Bank0045_Text_017);
}

// Clears the windows but the message's, back for the main menu
// pokebw2's GtsNegoMessage_Reset
void func_ov120_021d883c(GtsNegoMessage *msg) {
    int i;

    if (msg->titleWin != NULL) {
        func_0201f7a8(msg->titleWin, WINFRAME_TRANSFER_NONE);
        func_020456f8(msg->titleWin);
        GFL_BGSysQueueScrLoad(1);
        BmpWin_Free(msg->titleWin);
        msg->titleWin = NULL;
    }
    if (msg->cancelButton != NULL) {
        func_02028148(msg->cancelButton);
        msg->cancelButton = NULL;
    }
    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (msg->friendWins[i] != NULL) {
            func_020456f8(msg->friendWins[i]);
            BmpWin_Free(msg->friendWins[i]);
            msg->friendWins[i] = NULL;
        }
    }
    func_ov120_021d7d3c(msg);
    func_ov120_021d7c1c(msg);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x160, 0x20, msg->heapId);
    GFL_BGSysQueueScrLoad(5);
}
