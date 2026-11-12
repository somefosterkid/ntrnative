// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mb_parent/mb_util_msg.h.
#ifndef BWPORT_APP_MB_PARENT_MB_UTIL_MSG_H
#define BWPORT_APP_MB_PARENT_MB_UTIL_MSG_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The messages, windows and menus of the DS Download Play parent (mb_util_msg.c, overlay 107). The names are Black's
// dsd names; pokebw2's guesses are in the comments

// msgBg and menuBg are BGs; with useTalkWin, the messages of window MB_UTIL_MSG_WINDOW_TALK get the field's talk
// window frame, and with useKeys they wait for the keys as well as the touch screen (MBUtilMsg_Create)
MBUtilMsg *func_ov107_021f1404(HeapID heapId, u8 msgBg, u8 menuBg, u32 fileId, BOOL useTalkWin, u32 useKeys);
void func_ov107_021f1520(MBUtilMsg *msg); // MBUtilMsg_Delete
void func_ov107_021f15b0(MBUtilMsg *msg); // MBUtilMsg_Update
// Shows the message window in a shape, MB_UTIL_MSG_WINDOW_* of mb_util_msg.c, and clears it (MBUtilMsg_SetWindow)
void func_ov107_021f1628(MBUtilMsg *msg, u32 type);
// Prints a message of the message data, formatted by the word set if there is one (MBUtilMsg_Print)
void func_ov107_021f1704(MBUtilMsg *msg, u32 msgId, s32 wait);
void func_ov107_021f17e0(MBUtilMsg *msg, u32 msgId); // MBUtilMsg_PrintAtOnce
void func_ov107_021f1898(MBUtilMsg *msg); // MBUtilMsg_ClearWindow
void func_ov107_021f18d8(MBUtilMsg *msg); // MBUtilMsg_CreateWordSet
void func_ov107_021f18ec(MBUtilMsg *msg); // MBUtilMsg_FreeWordSet
void func_ov107_021f1900(MBUtilMsg *msg, u32 index, s32 number, u32 digits); // MBUtilMsg_SetNumber
void func_ov107_021f1918(MBUtilMsg *msg, u32 index, s32 number, u32 digits); // MBUtilMsg_SetNumberZeroPadded
// A yes/no task menu, at one of three heights (MBUtilMsg_CreateYesNoMenu)
void func_ov107_021f1930(MBUtilMsg *msg, u32 pos);
void func_ov107_021f19b8(MBUtilMsg *msg); // MBUtilMsg_FreeMenu
int func_ov107_021f19c8(MBUtilMsg *msg); // MBUtilMsg_GetMenuResult
// A yes/no dialog; the argument is not used (MBUtilMsg_CreateConfirm)
void func_ov107_021f19e4(MBUtilMsg *msg, u32 unused);
// Drops the dialog without freeing it, once it has freed itself (MBUtilMsg_ForgetConfirm)
void func_ov107_021f1a24(MBUtilMsg *msg);
int func_ov107_021f1a2c(MBUtilMsg *msg); // MBUtilMsg_UpdateConfirm
// Prints the message that the wireless is off, from the script messages (MBUtilMsg_PrintNoWireless)
void func_ov107_021f1a58(MBUtilMsg *msg, s32 wait);
MsgData *func_ov107_021f1afc(MBUtilMsg *msg); // MBUtilMsg_GetMsgData
WordSet *func_ov107_021f1b00(MBUtilMsg *msg); // MBUtilMsg_GetWordSet
Font *func_ov107_021f1b04(MBUtilMsg *msg); // MBUtilMsg_GetFont
BOOL func_ov107_021f1b08(MBUtilMsg *msg); // MBUtilMsg_IsQueueDone
BOOL func_ov107_021f1b14(MBUtilMsg *msg); // MBUtilMsg_IsPrintDone
void func_ov107_021f1b24(MBUtilMsg *msg, BOOL shown); // MBUtilMsg_ShowWindow
// Shows the wait icon once the message printed at once is done (MBUtilMsg_SetShowWaitIcon)
void func_ov107_021f1b38(MBUtilMsg *msg, BOOL show);

#endif // BWPORT_APP_MB_PARENT_MB_UTIL_MSG_H
