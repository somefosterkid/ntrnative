// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov165/plist_message.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pokelist.h"
#include "constants/sound.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "system/app_keycursor.h"
#include "system/app_printsys_common.h"
#include "system/bmp_winframe.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"

// The party list's message window, at the bottom of the screen, which prints at once or a character at a time

struct PokeListMessage {
    // POKELIST_MESSAGE_WINDOW_*, or POKELIST_MESSAGE_WINDOW_NONE
    u32 windowType;
    BmpWin *window;
    TCBExManager *tcbMgr;
    PrintStream *stream;
    StrBuf *str;
    KeyCursor *keyCursor;
    AppPrintsysCommon printWait;
    BOOL waitInput;
    BOOL dirty;
    PrintQueue *printQueue;
    WordSet *wordSet;
    // Whether the wait icon is to show once the message is printed
    BOOL wantWaitIcon;
    WaitIcon *waitIcon;
};

BOOL func_ov091_021bb910(u32 event);
void func_ov091_021bba28(PokeListWork *wk, PokeListMessage *msg);

// pokebw2's PokeListMessage_Create
PokeListMessage *func_ov091_021bb594(PokeListWork *wk) {
    PokeListMessage *msg = GFL_HeapAllocate(wk->heapId, sizeof(PokeListMessage), FALSE, "plist_message.c", 100);

    LoadSysMsgBox(0, 1, 12, 0, wk->heapId);
    msg->tcbMgr = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 1, 0);
    msg->stream = NULL;
    msg->windowType = POKELIST_MESSAGE_WINDOW_NONE;
    msg->dirty = FALSE;
    msg->waitInput = FALSE;
    msg->wordSet = NULL;
    msg->waitIcon = NULL;
    msg->wantWaitIcon = FALSE;
    msg->keyCursor = func_02028590(15, 1, 1, wk->heapId);
    msg->printQueue = func_0201c440(wk->heapId);
    return msg;
}

// pokebw2's PokeListMessage_Free
void func_ov091_021bb5fc(PokeListWork *wk, PokeListMessage *msg) {
    if (msg->windowType != POKELIST_MESSAGE_WINDOW_NONE) {
        func_ov091_021bb7a4(wk, msg);
    }
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
    }
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        GFL_StrBufFree(msg->str);
    }
    func_0201c6ec(msg->printQueue);
    func_0201c4c0(msg->printQueue);
    func_02028604(msg->keyCursor);
    GFL_TCBExMgrFree(msg->tcbMgr);
    GFL_HeapFree(msg);
}

// pokebw2's PokeListMessage_Update
void func_ov091_021bb644(PokeListWork *wk, PokeListMessage *msg) {
    GFL_TCBExMgrUpdate(msg->tcbMgr);
    if (msg->stream != NULL) {
        func_020286c4(msg->keyCursor, msg->stream, msg->window);
        if (func_02028484(&msg->printWait, msg->stream) == TRUE) {
            if (msg->waitInput == FALSE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) ||
                func_02035c60() == TRUE) {
                if (msg->waitInput == TRUE) {
                    GFL_SndSEPlay(SEQ_SE_MESSAGE);
                }
                func_0201ce48(msg->stream);
                msg->stream = NULL;
                GFL_StrBufFree(msg->str);
            } else {
                func_0202865c(msg->keyCursor, BmpWin_GetBitmap(msg->window), 15);
                BmpWin_FlushChar(msg->window);
            }
        }
    }
    if (msg->dirty == TRUE && func_0201c6c4(msg->printQueue, BmpWin_GetBitmap(msg->window)) == FALSE) {
        msg->dirty = FALSE;
        BmpWin_Transfer(msg->window);
        if (msg->wantWaitIcon == TRUE) {
            msg->wantWaitIcon = FALSE;
            msg->waitIcon = func_0202ee8c(msg->tcbMgr, msg->window, 15, 16, wk->heapId);
        }
    }
    func_0201c4e4(msg->printQueue);
}

// pokebw2's PokeListMessage_Open
void func_ov091_021bb714(PokeListWork *wk, PokeListMessage *msg, u32 windowType) {
    msg->windowType = windowType;
    switch (windowType) {
    case POKELIST_MESSAGE_WINDOW_SHORT:
        msg->window = BmpWin_CreateDynamic(0, 1, 21, 21, 2, 14, TRUE);
        break;
    case POKELIST_MESSAGE_WINDOW_SHORTER:
        msg->window = BmpWin_CreateDynamic(0, 1, 21, 20, 2, 14, TRUE);
        break;
    case POKELIST_MESSAGE_WINDOW_MENU:
        msg->window = BmpWin_CreateDynamic(0, 2, 19, 14, 4, 14, TRUE);
        break;
    case POKELIST_MESSAGE_WINDOW_WIDE:
        msg->window = BmpWin_CreateDynamic(0, 2, 19, 28, 4, 14, TRUE);
        break;
    }
    BmpWin_DrawFrame(msg->window, WINFRAME_TRANSFER_VBLANK, 1, 12);
    func_ov091_021bba28(wk, msg);
}

// pokebw2's PokeListMessage_Close
void func_ov091_021bb7a4(PokeListWork *wk, PokeListMessage *msg) {
    if (msg->windowType != POKELIST_MESSAGE_WINDOW_NONE) {
        if (msg->waitIcon != NULL) {
            func_0202f038(msg->waitIcon);
            msg->waitIcon = NULL;
        }
        func_0201f7a8(msg->window, WINFRAME_TRANSFER_VBLANK);
        BmpWin_Free(msg->window);
        func_0201c6ec(msg->printQueue);
        msg->windowType = POKELIST_MESSAGE_WINDOW_NONE;
    }
}

// pokebw2's PokeListMessage_IsOpen
BOOL func_ov091_021bb7d8(PokeListWork *wk, PokeListMessage *msg) {
    if (msg->windowType == POKELIST_MESSAGE_WINDOW_NONE) {
        return FALSE;
    }
    return TRUE;
}

// Prints a message at once, expanded with the word set if there is one
// pokebw2's PokeListMessage_Print
void func_ov091_021bb7e8(PokeListWork *wk, PokeListMessage *msg, u32 msgId) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    if (msg->wordSet != NULL) {
        StrBuf *expanded = GFL_StrBufCreate(256, wk->heapId);

        GFL_WordSetFormatStrbuf(msg->wordSet, expanded, str);
        GFL_StrBufFree(str);
        str = expanded;
    }
    func_ov091_021bba28(wk, msg);
    func_0201c6ec(msg->printQueue);
    func_0201c724(msg->printQueue, BmpWin_GetBitmap(msg->window), 1, 1, str, wk->font, 0x440);
    msg->dirty = TRUE;
    GFL_StrBufFree(str);
}

void func_ov091_021bb860(PokeListWork *wk, PokeListMessage *msg, u32 msgId, BOOL waitInput) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    if (msg->wordSet != NULL) {
        StrBuf *expanded = GFL_StrBufCreate(256, wk->heapId);

        GFL_WordSetFormatStrbuf(msg->wordSet, expanded, str);
        GFL_StrBufFree(str);
        str = expanded;
    }
    func_ov091_021bba28(wk, msg);
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        GFL_StrBufFree(msg->str);
    }
    msg->waitInput = waitInput;
    msg->str = str;
    func_02028470(&msg->printWait, 6);
    msg->stream = func_0201cd10(msg->window, 0, 0, msg->str, wk->font, func_02012ffc(), msg->tcbMgr, 0, wk->heapId, 15, func_ov091_021bb910);
}


// pokebw2's PokeListMessage_IsDone
BOOL func_ov091_021bb8fc(PokeListWork *wk, PokeListMessage *msg) {
    if (msg->stream == NULL && msg->dirty == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// The events of the messages' control codes: a fanfare, a sound, and waiting for them
// pokebw2's PokeListMessage_StreamCallback
BOOL func_ov091_021bb910(u32 event) {
    switch (event) {
    case 2:
        GFL_SndBGMSetPaused(TRUE);
        GFL_SndBGMPush();
        GFL_SndBGMPlay(SEQ_ME_LVUP, 0xffff);
        break;
    case 3:
        GFL_SndSEPlay(SEQ_SE_KON);
        break;
    case 5:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            GFL_SndBGMPop();
            GFL_SndBGMSetPaused(FALSE);
            return FALSE;
        }
        return TRUE;
    case 6:
        return GFL_SndPlayerIsActiveAny();
    }
    return FALSE;
}

// pokebw2's PokeListMessage_CreateWordSet
void func_ov091_021bb97c(PokeListWork *wk, PokeListMessage *msg) {
    msg->wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
}

// pokebw2's PokeListMessage_FreeWordSet
void func_ov091_021bb98c(PokeListWork *wk, PokeListMessage *msg) {
    GFL_WordSetSystemFree(msg->wordSet);
    msg->wordSet = NULL;
}

// pokebw2's PokeListMessage_SetPkmName
void func_ov091_021bb99c(PokeListWork *wk, PokeListMessage *msg, u32 index, PartyPkm *pkm) {
    loadPokemonNicknameToStrbuf(msg->wordSet, index, pkm);
}

// pokebw2's PokeListMessage_SetItemName
void func_ov091_021bb9ac(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 item) {
    loadItemNameToStrbuf(msg->wordSet, index, item);
}

// pokebw2's PokeListMessage_SetItemTextName
void func_ov091_021bb9bc(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 item) {
    loadItemTextNameToStrbuf(msg->wordSet, index, item);
}

// pokebw2's PokeListMessage_SetMoveName
void func_ov091_021bb9cc(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 move) {
    loadMoveNameToStrbuf(msg->wordSet, index, move);
}

// pokebw2's PokeListMessage_SetStatName
void func_ov091_021bb9dc(PokeListWork *wk, PokeListMessage *msg, u32 index, u32 stat) {
    loadStatNameToStrbuf(msg->wordSet, index, stat);
}

// pokebw2's PokeListMessage_SetNumber
void func_ov091_021bb9ec(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 number, u8 digits) {
    WordSetNumber(msg->wordSet, index, number, digits, 0, TRUE);
}

// pokebw2's PokeListMessage_SetString
void func_ov091_021bba0c(PokeListWork *wk, PokeListMessage *msg, u32 index, const StrBuf *str, u32 a4) {
    func_0201ee0c(msg->wordSet, index, str, a4, 1, 2);
}

// pokebw2's PokeListMessage_Clear
void func_ov091_021bba28(PokeListWork *wk, PokeListMessage *msg) {
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), 15);
    BmpWin_Transfer(msg->window);
}

// pokebw2's PokeListMessage_LoadFrame
void func_ov091_021bba54(PokeListWork *wk, PokeListMessage *msg) {
    LoadSysMsgBox(0, 1, 12, 0, wk->heapId);
}

// Shows the wait icon in the window once the message is printed
// pokebw2's PokeListMessage_ShowWaitIcon
void func_ov091_021bba68(PokeListWork *wk, PokeListMessage *msg) {
    msg->wantWaitIcon = TRUE;
}

// pokebw2's PokeListMessage_DrawKeyCursor
void func_ov091_021bba70(PokeListWork *wk, PokeListMessage *msg) {
    func_02028618(msg->keyCursor, BmpWin_GetBitmap(msg->window), 15);
    BmpWin_FlushChar(msg->window);
}
