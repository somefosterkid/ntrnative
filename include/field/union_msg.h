// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_msg.h: the lines in use so far.
#ifndef BWPORT_FIELD_UNION_MSG_H
#define BWPORT_FIELD_UNION_MSG_H

#include "types.h"
#include "stddef.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// The rules of a regulation printed (PokeRegulation_CreatePrintMsg): its message data and, for each of the six rules, the name of the
// rule and its value. alloc.rpm of the system holds one
struct RegulationRulesText {
    MsgData *msgData;
    StrBuf *rule[6];
    StrBuf *value[6];
    u8 unk34;
};

// The message IDs of the Union Room's texts, for a gender (0 or 1) or a language. The tables are of two IDs
u16 func_ov019_0217ec04(UnionSystem *unisys);
u16 func_ov019_0217ec1c(u8 gender);
u16 func_ov019_0217ec40(u32 row, u8 gender);
u16 func_ov019_0217ec68(u32 row);
u16 func_ov019_0217ece8(u8 gender);
u16 func_ov019_0217ed10(u8 gender);
// pokebw2's UnionMsg_GetOriginMessage
// The message of a trainer card's origin: nothing, another country, another province or the same
u32 func_ov019_0217ee4c(UnionSystem *unisys, u8 country, u8 province);
// pokebw2's UnionMsg_GetTableByte
int func_ov019_0217f060(s32 index);
// pokebw2's UnionMsg_FreeAll
// Frees the message system, the menus and the windows
void func_ov019_0217f074(UnionSystem *unisys);
// pokebw2's UnionMsg_IsPrintDone
BOOL func_ov019_0217f1ec(UnionSystem *unisys);
// pokebw2's UnionMsg_CreateAndPrint
// Creates the message system, sets the words and prints a message
void func_ov019_0217f210(UnionSystem *unisys, Field *field, u32 messageId);
// pokebw2's UnionMsg_CreateYesNo
void func_ov019_0217f22c(UnionSystem *unisys, Field *field);
// pokebw2's UnionMsg_FreeYesNo
void func_ov019_0217f24c(UnionSystem *unisys);
// pokebw2's UnionMsg_CreateMainMenu
void func_ov019_0217f360(UnionSystem *unisys, Field *field);
// pokebw2's UnionMsg_CreateRuleMenu
void func_ov019_0217f388(UnionSystem *unisys, Field *field, int kind, u8 *ruleSel);
// pokebw2's UnionMsg_CreateInfoWindow
void func_ov019_0217f5e0(UnionSystem *unisys, Field *field);
// pokebw2's UnionMsg_FreeInfoWindow
void func_ov019_0217f624(UnionSystem *unisys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov021_0219160c(MsgBGBalloonWin *window);
void func_ov021_0219161c(MsgBGBalloonWin *window, u32 x, u32 y, u32 messageId);
void func_ov021_02191648(MsgBGBalloonWin *window, u8 x, u8 y, StrBuf *strbuf);
void func_ov021_02191674(MsgBGBalloonWin *window, u16 x, u16 y, StrBuf *strbuf, u16 color);
u16 func_ov019_0217ec28(u8 gender);
u16 func_ov019_0217ec34(u8 gender);
u16 func_ov019_0217ec54(u32 row, u8 gender);
u16 func_ov019_0217ec78(u32 row);
u16 func_ov019_0217ec88(u8 gender);
u16 func_ov019_0217ec94(u8 gender);
u16 func_ov019_0217eca0(u8 gender);
u16 func_ov019_0217ecac(u8 gender);
u16 func_ov019_0217ecb8(UnionSystem *unisys);
u16 func_ov019_0217ecd0(UnionSystem *unisys);
u16 func_ov019_0217ed04(u8 gender);
u16 func_ov019_0217ed2c(u8 gender);
u16 func_ov019_0217ed48(u8 gender);
u16 func_ov019_0217ed64(u8 gender);
u16 func_ov019_0217ed80(u8 gender);
u16 func_ov019_0217ed8c(u8 gender);
u16 func_ov019_0217ed98(u8 gender);
u16 func_ov019_0217eda4(u8 gender);
u16 func_ov019_0217edb0(u8 gender);
u16 func_ov019_0217edbc(u8 gender);
u16 func_ov019_0217edc8(u8 gender);
u16 func_ov019_0217edd4(u8 gender);
u16 func_ov019_0217ede0(u8 gender);
u16 func_ov019_0217edec(u8 gender);
u16 func_ov019_0217edf8(u8 gender);
u16 func_ov019_0217ee04(u8 gender);
u16 func_ov019_0217ee10(u8 gender);
u16 func_ov019_0217ee1c(u8 gender);
u16 func_ov019_0217ee28(u8 gender);
u16 func_ov019_0217ee34(u8 gender);
u16 func_ov019_0217ee40(u8 gender);
// pokebw2's UnionMsg_CreateMsgSystem
// Creates the message system (the message data, the stream window, the word set) or reopens its window
void func_ov019_0217f098(UnionSystem *unisys, Field *field);
// pokebw2's UnionMsg_PrintMessage
void func_ov019_0217f188(UnionSystem *unisys, u32 messageId);
// pokebw2's UnionMsg_UpdateYesNo
BOOL func_ov019_0217f268(UnionSystem *unisys, BOOL *yes);
// pokebw2's UnionMsg_FreeMainMenu
void func_ov019_0217f378(UnionSystem *unisys);
// pokebw2's UnionMsg_UpdateMainMenu
intptr_t func_ov019_0217f380(UnionSystem *unisys, Field *field);
// pokebw2's UnionMsg_FreeRuleMenu
void func_ov019_0217f4ac(UnionSystem *unisys);
// pokebw2's UnionMsg_UpdateRuleMenu
u32 func_ov019_0217f4b4(UnionSystem *unisys, BOOL *picked, u8 *ruleSel, BOOL *isFinal, Field *field);
// pokebw2's UnionMsg_CreateRulesWindow
void func_ov019_0217f640(UnionSystem *unisys, Field *field, u32 okMask, u32 battleType, u32 mode);
// pokebw2's UnionMsg_IsRulesPrinted
BOOL func_ov019_0217f794(UnionSystem *unisys, Field *field);
// pokebw2's UnionMsg_FreeRulesWindow
void func_ov019_0217f7b8(UnionSystem *unisys);

#endif // BWPORT_FIELD_UNION_MSG_H
