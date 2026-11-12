// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gsync/pdwacc_message.h, the lines in
// use so far.
#ifndef BWPORT_APP_GSYNC_PDWACC_MESSAGE_H
#define BWPORT_APP_GSYNC_PDWACC_MESSAGE_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The Dream World account's messages (pdwacc_message.c)

// Writes a Game Sync ID as its 10 characters. Black 2 has this code in each of its two callers, pokebw2's
// GSyncMessage_FormatGSyncId and PdwAccMessage_ShowGSyncId
void func_ov123_021d97cc(u32 id, StrBuf *strBuf);

// Where PdwAccMessage_CreateYesNo puts the menu
#define PDWACC_YESNO_POS_UPPER 0

// pokebw2's PdwAccMessage_IsPrintFinished
BOOL func_ov123_021d95f0(PdwAccMessage *msg);
// pokebw2's PdwAccMessage_PrintStream
// Prints a message a character at a time
void func_ov123_021d9514(PdwAccMessage *msg, u32 msgId);
// pokebw2's PdwAccMessage_StartWaitIcon
void func_ov123_021d95b4(PdwAccMessage *msg);
// pokebw2's PdwAccMessage_ClearInfo
void func_ov123_021d97b0(PdwAccMessage *msg);
// pokebw2's PdwAccMessage_PrintInfo
void func_ov123_021d9730(PdwAccMessage *msg, u32 msgId);
// pokebw2's PdwAccMessage_ClearMessage
void func_ov123_021d9634(PdwAccMessage *msg);
// pokebw2's PdwAccMessage_CreateYesNo
AppTaskMenu *func_ov123_021d9670(PdwAccMessage *msg, int pos);
// pokebw2's PdwAccMessage_ClearGSyncId
void func_ov123_021d9918(PdwAccMessage *msg);
// pokebw2's PdwAccMessage_Main
void func_ov123_021d9404(PdwAccMessage *msg);
// pokebw2's PdwAccMessage_Free
void func_ov123_021d941c(PdwAccMessage *msg);

#define PDWACC_YESNO_POS_LOWER 1

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PdwAccMessage_ShowGSyncId
// Shows a Game Sync ID as its 10 characters, under its label. Both callers pass the ID again as the third argument,
// which is unused
void func_ov123_021d9844(PdwAccMessage *msg, u32 id, u32 unused);
// pokebw2's PdwAccMessage_Create
PdwAccMessage *func_ov123_021d92cc(HeapID heapId, u32 msgFile);

#endif // BWPORT_APP_GSYNC_PDWACC_MESSAGE_H
