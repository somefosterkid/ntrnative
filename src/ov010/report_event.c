// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/report_event.c, which is the same code in
// Black 2/White 2's overlay 12, except for the message files and that Black's report always goes back to the first
// subscreen. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); pokebw2's own guesses are in the
// headers' comments.

// The report: the screen that asks whether to save the game, saves it alongside the field and says so
#include "types.h"
#include "field/event_save.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_player.h"
#include "field/subscreen.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"

#define SEQ_SE_SAVE 0x558
#define SEQ_SE_MESSAGE 0x547

typedef struct {
    BmpWin *window;
    // Set while the print queue has yet to draw into the window
    u8 printing;
} ReportWindow;

struct ReportWork {
    TCBExManager *tcbManager;
    MsgData *msgData;
    ReportWindow window;
    PrintStream *stream;
    PrintQueue *printQueue;
    StrBuf *strbuf;
    KeyCursor *cursor;
    // Set once the player has continued a paused message
    u32 continued;
    AppTaskMenuItem items[2];
    void *menuRes;
    void *menu;
    WaitIcon *waitIcon;
    void *effect;
    u32 timer;
};

static void func_ov010_02169e0c(EventSaveWork *work);
static void func_ov010_02169e4c(EventSaveWork *work);
static void func_ov010_02169e98(EventSaveWork *work);
static void func_ov010_02169ea4(EventSaveWork *work, u32 messageId);
static void func_ov010_02169ebc(EventSaveWork *work);
static BOOL func_ov010_02169f34(EventSaveWork *work);
static BOOL func_ov010_02169fb8(EventSaveWork *work);
static void func_ov010_02169ffc(EventSaveWork *work, u32 messageId);
static BOOL func_ov010_0216a07c(EventSaveWork *work);
static void func_ov010_0216a0bc(EventSaveWork *work);
static void func_ov010_0216a128(EventSaveWork *work);
static void func_ov010_0216a148(EventSaveWork *work);
static int func_ov010_0216a184(EventSaveWork *work);
static void func_ov010_0216a1c0(EventSaveWork *work);
static void func_ov010_0216a1f8(EventSaveWork *work);
static BOOL func_ov010_0216a21c(EventSaveWork *work);
static BOOL func_ov010_0216a244(EventSaveWork *work);
static void func_ov010_0216a264(EventSaveWork *work);
static void func_ov010_0216a280(EventSaveWork *work);

u32 EventSave_Update(EventSaveWork *work, u32 *state) {
    WordSet *wordSet;
    StrBuf *str;

    switch (*state) {
    case 0:
        FieldSubscreen_ReqChange(Field_GetSubscreen(work->field), 7);
        *state = 1;
        break;
    case 1:
        if (FieldSubscreen_IsReady(Field_GetSubscreen(work->field)) == TRUE &&
            func_ov021_021a1518(Field_GetSubscreen(work->field)) == TRUE) {
            work->report = GFL_HeapAllocate(work->heapId, sizeof(ReportWork), FALSE, "report_event.c", 159);
            work->report->msgData = GFL_MsgSysLoadData(FALSE, 3, 0x9e, work->heapId);
            work->report->strbuf = GFL_StrBufCreate(0x200, work->heapId);
            work->report->tcbManager = GFL_TCBExMgrCreate(work->heapId, work->heapId, 1, 4);
            work->report->cursor = func_02028590(0xf, 1, 1, work->heapId);
            work->report->printQueue = func_0201c440(work->heapId);
            work->report->timer = 0;
            func_ov010_02169e4c(work);
            func_ov010_0216a0bc(work);
            *state = 2;
        }
        break;
    case 2:
        func_ov010_02169ea4(work, 2);
        *state = 3;
        break;
    case 3:
        if (func_ov010_02169fb8(work) == FALSE) {
            func_ov010_0216a148(work);
            *state = 4;
        }
        break;
    case 4:
        switch (func_ov010_0216a184(work)) {
        case 0:
            if (func_020071f0(work->save) == TRUE) {
                func_ov010_02169ea4(work, 8);
                *state = 14;
            } else {
                func_0201f7a8(work->report->window.window, WINFRAME_TRANSFER_VBLANK);
                *state = 5;
            }
            break;
        case 1:
            *state = 17;
            break;
        }
        break;
    case 5:
        RecordAddOne(GameData_GetRecords(GSYS_GetGameData(work->gameSystem)), 1);
        func_ov021_021a1524(Field_GetSubscreen(work->field));
        if (func_ov021_021a1554(Field_GetSubscreen(work->field)) == TRUE) {
            func_ov010_02169ffc(work, 10);
        } else {
            func_ov010_02169ffc(work, 4);
        }
        *state = 6;
        break;
    case 6:
        if (func_ov010_0216a07c(work) == TRUE) {
            func_ov010_0216a1c0(work);
            *state = 7;
        }
        break;
    case 7:
        if (func_ov010_0216a21c(work) == TRUE) {
            *state = 8;
        }
        break;
    case 8:
        if (func_ov010_0216a244(work)) {
            work->report->waitIcon =
                func_0202ee30(GFL_VBlankGetTCBMgr(), work->report->window.window, 0xf, 0x10, work->heapId);
            func_ov021_021a1530(Field_GetSubscreen(work->field));
            func_ov010_0216a264(work);
            func_02012dac(GSYS_GetGameData(work->gameSystem));
            *state = 9;
        }
        break;
    case 9:
        switch (func_02012dd0(GSYS_GetGameData(work->gameSystem))) {
        case 0:
        case 1:
            break;
        case 2:
            *state = 10;
            break;
        case 3:
            func_ov021_021a1548(Field_GetSubscreen(work->field));
            func_ov010_0216a280(work);
            func_ov010_0216a1f8(work);
            func_ov010_02169ea4(work, 7);
            func_0202f038(work->report->waitIcon);
            *state = 13;
            break;
        }
        break;
    case 10:
        if (func_ov021_021a153c(Field_GetSubscreen(work->field)) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_SAVE);
            func_ov010_0216a280(work);
            func_ov010_0216a1f8(work);
            wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
            str = GFL_MsgDataLoadStrbufNew(work->report->msgData, 5);
            copyVarForText(wordSet, 0, GetGameDataPlayerInfo(GSYS_GetGameData(work->gameSystem)));
            GFL_WordSetFormatStrbuf(wordSet, work->report->strbuf, str);
            GFL_StrBufFree(str);
            GFL_WordSetSystemFree(wordSet);
            func_ov010_02169ebc(work);
            func_0202f038(work->report->waitIcon);
            *state = 11;
        }
        break;
    case 11:
        if (func_ov010_0216a21c(work) == TRUE && func_ov010_0216a244(work) && func_ov010_02169fb8(work) == FALSE) {
            *state = 12;
        }
        break;
    case 12:
        if (!GFL_SndIsPlaying(SEQ_SE_SAVE)) {
            *state = 16;
        }
        break;
    case 13:
        if (func_ov010_0216a21c(work) != TRUE || !func_ov010_0216a244(work)) {
            break;
        }
        *state = 14;
        // fallthrough
    case 14:
        if (func_ov010_02169fb8(work) == FALSE) {
            *state = 15;
        }
        break;
    case 15:
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            *state = 19;
        }
        break;
    case 16:
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            *state = 19;
        }
        if (work->report->timer >= 150) {
            *state = 19;
        } else {
            work->report->timer++;
        }
        break;
    case 17:
        func_ov010_02169e0c(work);
        FieldSubscreen_ReqChange(Field_GetSubscreen(work->field), 1);
        *state = 18;
        break;
    case 18:
        return 1;
    case 19:
        func_ov010_02169e0c(work);
        FieldSubscreen_ReqChange(Field_GetSubscreen(work->field), 0);
        *state = 20;
        break;
    case 20:
        func_ov021_021a0fb4(Field_GetSubscreen(work->field), work->heapId);
        return 0;
    }
    return 2;
}

static void func_ov010_02169e0c(EventSaveWork *work) {
    func_ov010_0216a128(work);
    func_ov010_02169e98(work);
    func_0201c4c0(work->report->printQueue);
    func_02028604(work->report->cursor);
    GFL_TCBExMgrFree(work->report->tcbManager);
    GFL_StrBufFree(work->report->strbuf);
    GFL_MsgDataFree(work->report->msgData);
    GFL_HeapFree(work->report);
}

static void func_ov010_02169e4c(EventSaveWork *work) {
    work->report->window.window = BmpWin_CreateDynamic(6, 1, 1, 30, 4, 12, 1);
    LoadSysMsgBox(6, 1, 13, 0, work->heapId);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x180, 0x20, work->heapId);
}

static void func_ov010_02169e98(EventSaveWork *work) {
    BmpWin_Free(work->report->window.window);
}

static void func_ov010_02169ea4(EventSaveWork *work, u32 messageId) {
    GFL_MsgDataLoadStrbuf(work->report->msgData, messageId, work->report->strbuf);
    func_ov010_02169ebc(work);
}

static void func_ov010_02169ebc(EventSaveWork *work) {
    ReportWork *report;
    BmpWin *window;

    GFL_BitmapFill(BmpWin_GetBitmap(work->report->window.window), 15);
    BmpWin_DrawFrame(work->report->window.window, 2, 1, 13);
    report = work->report;
    work->report->stream = func_0201cce4(report->window.window, 0, 0, report->strbuf, func_ov021_021904ac(work->msgBgSys),
                                         func_02012ffc(), report->tcbManager, 10, work->heapId, 15);
    work->report->continued = FALSE;
    window = work->report->window.window;
    BmpWin_Transfer(window);
}

static BOOL func_ov010_02169f34(EventSaveWork *work) {
    switch (func_0201ce30(work->report->stream)) {
    case PRINT_STREAM_RUNNING:
        if (func_02035c60() == TRUE || (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_0201ce5c(work->report->stream, 0);
        }
        work->report->continued = FALSE;
        break;
    case PRINT_STREAM_PAUSED:
        if (work->report->continued == FALSE &&
            (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)))) {
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            func_0201ce38(work->report->stream);
            work->report->continued = TRUE;
        }
        break;
    case PRINT_STREAM_DONE:
        func_0201ce48(work->report->stream);
        work->report->continued = FALSE;
        return FALSE;
    }
    return TRUE;
}

// Prints at twice the speed
static BOOL func_ov010_02169fb8(EventSaveWork *work) {
    GFL_TCBExMgrUpdate(work->report->tcbManager);
    func_020286c4(work->report->cursor, work->report->stream, work->report->window.window);
    if (func_ov010_02169f34(work) == FALSE) {
        return FALSE;
    }
    GFL_TCBExMgrUpdate(work->report->tcbManager);
    func_020286c4(work->report->cursor, work->report->stream, work->report->window.window);
    return func_ov010_02169f34(work);
}

static void func_ov010_02169ffc(EventSaveWork *work, u32 messageId) {
    StrBuf *str;
    ReportWindow *reportWindow;
    PrintQueue *queue;
    Font *font;
    BmpWin *window;

    GFL_BitmapFill(BmpWin_GetBitmap(work->report->window.window), 15);
    BmpWin_DrawFrame(work->report->window.window, 2, 1, 13);
    str = GFL_MsgDataLoadStrbufNew(work->report->msgData, messageId);
    font = func_ov021_021904ac(work->msgBgSys);
    reportWindow = &work->report->window;
    queue = work->report->printQueue;
    func_0201c6fc(queue, BmpWin_GetBitmap(reportWindow->window), 0, 0, str, font);
    reportWindow->printing = TRUE;
    GFL_StrBufFree(str);
    window = work->report->window.window;
    BmpWin_Transfer(window);
}

static BOOL func_ov010_0216a07c(EventSaveWork *work) {
    ReportWindow *reportWindow;
    PrintQueue *queue;

    func_0201c4e4(work->report->printQueue);
    queue = work->report->printQueue;
    reportWindow = &work->report->window;
    if (reportWindow->printing && !func_0201c6c4(queue, BmpWin_GetBitmap(reportWindow->window))) {
        BmpWin_FlushChar(reportWindow->window);
        reportWindow->printing = FALSE;
    }
    return func_0201c6b4(work->report->printQueue);
}

static void func_ov010_0216a0bc(EventSaveWork *work) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, 2, 0xa8, work->heapId);

    work->report->items[0].str = GFL_MsgDataLoadStrbufNew(msgData, 8);
    work->report->items[0].color = 0x39e3;
    work->report->items[0].type = 0;
    work->report->items[1].str = GFL_MsgDataLoadStrbufNew(msgData, 9);
    work->report->items[1].color = 0x39e3;
    work->report->items[1].type = 0;
    GFL_MsgDataFree(msgData);
    work->report->menuRes = func_02027f60(6, 14, func_ov021_021904ac(work->msgBgSys),
                                          func_ov021_021904a8(work->msgBgSys), work->heapId);
}

static void func_ov010_0216a128(EventSaveWork *work) {
    func_02027fd8(work->report->menuRes);
    GFL_StrBufFree(work->report->items[1].str);
    GFL_StrBufFree(work->report->items[0].str);
}

static void func_ov010_0216a148(EventSaveWork *work) {
    AppTaskMenuInit init;

    init.heapId = work->heapId;
    init.itemCount = 2;
    init.items = work->report->items;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 12;
    init.width = 8;
    init.height = 3;
    work->report->menu = func_0202784c(&init, work->report->menuRes);
}

// 0 for yes, 1 for no, and -1 until one is picked
static int func_ov010_0216a184(EventSaveWork *work) {
    u32 pos;

    func_02027974(work->report->menu);
    if (func_020279e8(work->report->menu) == TRUE) {
        pos = func_02027a04(work->report->menu);
        func_02027858(work->report->menu);
        if (pos == 0) {
            return 0;
        }
        return 1;
    }
    return -1;
}

static void func_ov010_0216a1c0(EventSaveWork *work) {
    FieldPlayer *player = Field_GetPlayer(work->field);
    FieldActor *actor;

    if (func_ov021_021a3208(player) == TRUE) {
        actor = FieldPlayer_GetActor(player);
        FieldPlayer_SetSpecialSeq(player, 0x20);
        EnableActorMovement(actor);
        ClearActorFlag(actor, 0x10);
        func_ov021_021a2f54(player);
    }
}

static void func_ov010_0216a1f8(EventSaveWork *work) {
    FieldPlayer *player = Field_GetPlayer(work->field);

    if (func_ov021_021a3208(player) == TRUE) {
        FieldPlayer_SetSpecialSeq(player, 8);
        func_ov021_021a2f54(player);
    }
}

static BOOL func_ov010_0216a21c(EventSaveWork *work) {
    FieldPlayer *player = Field_GetPlayer(work->field);

    if (func_ov021_021a3208(player) == FALSE) {
        return TRUE;
    }
    if (func_ov021_021a2f54(player) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov010_0216a244(EventSaveWork *work) {
    FieldPlayer *player = Field_GetPlayer(work->field);

    if (func_ov021_021a3208(player) == TRUE) {
        return func_ov021_021a3244(player);
    }
    return TRUE;
}

static void func_ov010_0216a264(EventSaveWork *work) {
    work->report->effect = func_ov021_021d1164(work->heapId, work->field);
    func_ov021_021d11b0(work->report->effect);
}

static void func_ov010_0216a280(EventSaveWork *work) {
    func_ov021_021d11d8(work->report->effect);
    func_ov021_021d1194(work->report->effect);
}
