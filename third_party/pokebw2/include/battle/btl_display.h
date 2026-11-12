#ifndef POKEBW2_BATTLE_BTL_DISPLAY_H
#define POKEBW2_BATTLE_BTL_DISPLAY_H

#include "types.h"
#include "struct_decls.h"

void ServerDisplay_AbilityPopupAdd(BtlServerFlow *handler, BattleMon *mon);
void ServerDisplay_AbilityPopupRemove(BtlServerFlow *handler, BattleMon *mon);
void scPut_SetContFlag(BtlServerFlow *handler, BattleMon *mon, u32 flag);
void scPut_ResetContFlag(BtlServerFlow *handler, BattleMon *mon, u32 flag);
void ServerDisplay_SetTurnFlag(BtlServerFlow *handler, BattleMon *mon, u32 flag);
void ServerDisplay_UseHeldItem(BtlServerFlow *handler, BattleMon *mon);
void ServerDisplay_RecoverPP(BtlServerFlow *handler, BattleMon *mon, u8 slot, u8 amount, BOOL original);
void ServerDisplay_SimpleHP(BtlServerFlow *handler, BattleMon *mon, s32 amount, BOOL show);
void ServerDisplay_StandardMessage(BtlServerFlow *handler, u16 message, u32 count, const u32 *args);
void ServerDisplay_SetMessage(BtlServerFlow *handler, u16 message, u32 count, const u32 *args);
void ServerDisplay_StandardMessageEx(BtlServerFlow *handler, u16 message, u16 soundEffect, u32 count, const u32 *args);
void ServerDisplay_SetMessageEx(BtlServerFlow *handler, u16 message, u16 soundEffect, u32 count, const u32 *args);

#endif // POKEBW2_BATTLE_BTL_DISPLAY_H
