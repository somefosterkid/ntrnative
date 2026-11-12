// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifilogin_message.h: the lines in use so
// far.
#ifndef BWPORT_APP_WIFILOGIN_MESSAGE_H
#define BWPORT_APP_WIFILOGIN_MESSAGE_H

#include "types.h"
#include "gfl/heap.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"
#include "struct_decls.h"

// The screen the windows are on, which also picks the BGs: the Dream World account style's or Game Sync's, as in
// wifilogin_disp.h
#define WIFILOGIN_MESSAGE_STYLE_PDWACC 0

#define WIFILOGIN_MESSAGE_STYLE_GSYNC 1

// The yes/no menu, of the style's kind
typedef struct {
    u32 style;
    AppTaskMenuItem items[2];
    AppTaskMenuRes *res;
    AppTaskMenu *taskMenu;
    BmpMenu *dialog;
    // The confirm dialog's result, BMPMENU_NULL until picked
    s32 result;
} WifiLoginMessageMenu;

// Black 2's; twinned code confirms the offsets of frameChars, frameCharsOther, msgData, wordSet, font, strbuf,
// messageWin, unk30, titleWin, stream, printsys, tcbManager, printQueue, heapId, style, menu and waitIcon, and its size
// in Black
typedef struct WifiLoginMessage {
    // The frame characters, loaded at the end of the BGs of the message window and of the other one
    u32 frameChars;
    u32 frameCharsOther;
    u8 unk8[0x14];
    MsgData *msgData;
    WordSet *wordSet;
    Font *font;
    StrBuf *strbuf;
    BmpWin *messageWin;
    BmpWin *unk30;
    BmpWin *titleWin;
    PrintStream *stream;
    AppPrintsysCommon printsys;
    TCBExManager *tcbManager;
    PrintQueue *printQueue;
    u16 heapId;
    u32 style;
    WifiLoginMessageMenu menu;
    WaitIcon *waitIcon;
} WifiLoginMessage;

// pokebw2's WifiLoginMessage_Create
WifiLoginMessage *func_ov115_021d5984(HeapID heapId, u32 bank, u32 style);
// pokebw2's WifiLoginMessage_Update
void func_ov115_021d5a88(WifiLoginMessage *msg);
// pokebw2's WifiLoginMessage_Free
void func_ov115_021d5a9c(WifiLoginMessage *msg);
// pokebw2's WifiLoginMessage_Print
// Print a message of the loaded bank (WifiLoginMessage_PrintFrom: of msgData), and wait for it with WifiLoginMessage_IsDone
void func_ov115_021d5b50(WifiLoginMessage *msg, u32 msgNo);
// pokebw2's WifiLoginMessage_IsDone
BOOL func_ov115_021d5c70(WifiLoginMessage *msg);
// pokebw2's WifiLoginMessage_ClearWindow
// Clears the message window
void func_ov115_021d5c94(WifiLoginMessage *msg);
// pokebw2's WifiLoginMessage_PrintTitle
void func_ov115_021d5ccc(WifiLoginMessage *msg, u32 msgNo);
// pokebw2's WifiLoginMessage_ClearTitle
void func_ov115_021d5d3c(WifiLoginMessage *msg);
// pokebw2's WifiLoginMessage_CreateMenu
WifiLoginMessageMenu *func_ov115_021d5d5c(WifiLoginMessage *msg, u32 layout, BOOL dimTopScreen, u32 cursorPos);
// pokebw2's WifiLoginMessage_FreeMenu
void func_ov115_021d5e34(WifiLoginMessageMenu *menu);
// pokebw2's WifiLoginMessage_PrintNotice
// Prints a message in the window of the notices, and clears the window
void func_ov115_021d5ed8(WifiLoginMessage *msg, u32 msgNo);
// pokebw2's WifiLoginMessage_ClearNotice
void func_ov115_021d5f7c(WifiLoginMessage *msg);
// pokebw2's WifiLoginMessage_CreateConfirmDialog
BmpMenu *func_ov115_021d5fe8(WifiLoginMessage *msg, u32 layout, u32 cursorPos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WifiLoginMessage_PrintFrom
void func_ov115_021d5b5c(WifiLoginMessage *msg, MsgData *msgData, u32 msgNo);
// pokebw2's WifiLoginMessage_PrintWaiting
// The same, then with the wait icon on the window
void func_ov115_021d5c24(WifiLoginMessage *msg, u32 msgNo);
// pokebw2's WifiLoginMessage_UpdateMenu
void func_ov115_021d5e48(WifiLoginMessageMenu *menu);
// pokebw2's WifiLoginMessage_IsMenuAnswered
// The yes/no menu of the message, its window of notices, the BGs of its windows and the confirm dialog
// The yes/no menu: whether the player has answered, and the answer, 0 for yes
BOOL func_ov115_021d5e6c(WifiLoginMessageMenu *menu);
// pokebw2's WifiLoginMessage_GetMenuAnswer
u8 func_ov115_021d5ea8(WifiLoginMessageMenu *menu);
// pokebw2's WifiLoginMessage_GetMessageBG
u8 func_ov115_021d5fa0(u32 style);
// pokebw2's WifiLoginMessage_GetNoticeBG
u8 func_ov115_021d5fc4(u32 style);

#endif // BWPORT_APP_WIFILOGIN_MESSAGE_H
