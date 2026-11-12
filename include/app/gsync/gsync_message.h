// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gsync/gsync_message.h, with Black's
// names: pokebw2's GSyncMessage_ names are its own guesses.
#ifndef BWPORT_APP_GSYNC_GSYNC_MESSAGE_H
#define BWPORT_APP_GSYNC_GSYNC_MESSAGE_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Game Sync's messages (gsync_message.c): the message window and an info window below it on the touch screen,
// and the yes/no menu

// Where func_ov123_021d5890 puts the menu
#define GSYNC_YESNO_POS_UPPER 0
#define GSYNC_YESNO_POS_LOWER 1

// pokebw2's GSyncMessage_Create, GSyncMessage_Main and GSyncMessage_Free
GSyncMessage *func_ov123_021d54f4(HeapID heapId, u32 msgFile);
void func_ov123_021d562c(GSyncMessage *msg);
void func_ov123_021d5644(GSyncMessage *msg);
// Prints the loaded string, at once or a character at a time (pokebw2's GSyncMessage_PrintLoaded)
void func_ov123_021d56fc(GSyncMessage *msg, BOOL now);
// Prints a message a character at a time (pokebw2's GSyncMessage_PrintStream)
void func_ov123_021d57b8(GSyncMessage *msg, u32 msgId);
// Loads a message with a Pokémon's nickname as word 0 and a number as word 1, to print (pokebw2's
// GSyncMessage_FormatPokemonNumber)
void func_ov123_021d57d0(GSyncMessage *msg, u32 msgId, s32 number, PartyPkm *pkm);
// pokebw2's GSyncMessage_IsPrintFinished, GSyncMessage_ClearMessage, GSyncMessage_CreateYesNo and
// GSyncMessage_LoadString
BOOL func_ov123_021d580c(GSyncMessage *msg);
void func_ov123_021d584c(GSyncMessage *msg);
AppTaskMenu *func_ov123_021d5890(GSyncMessage *msg, int pos);
void func_ov123_021d5950(GSyncMessage *msg, u32 msgId);
// The info window, with the loaded string, at a row and height of its own or the default ones (pokebw2's
// GSyncMessage_PrintInfoAt and GSyncMessage_PrintInfo)
void func_ov123_021d5960(GSyncMessage *msg, int y, int height);
void func_ov123_021d59e4(GSyncMessage *msg);
// pokebw2's GSyncMessage_ClearInfo and GSyncMessage_StartWaitIcon
void func_ov123_021d5a60(GSyncMessage *msg);
void func_ov123_021d5a7c(GSyncMessage *msg);
// Writes a Game Sync ID as its 10 characters (pokebw2's GSyncMessage_FormatGSyncId)
void func_ov123_021d5ab0(GSyncMessage *msg, u32 id);

#endif // BWPORT_APP_GSYNC_GSYNC_MESSAGE_H
