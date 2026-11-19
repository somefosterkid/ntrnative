#ifndef POKEHEARTGOLD_UNK_02066EDC_H
#define POKEHEARTGOLD_UNK_02066EDC_H

#include "script.h"

void sub_02067118(TaskManager *taskManager, void *a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7);
void sub_02067200(TaskManager *taskManager, u16 a1, u16 a2, u16 a3);
void sub_020672A4(TaskManager *taskManager, u16 a1, u16 a2);
u32 sub_020672D8(SaveData *saveData);
u16 sub_02067398(SaveData *saveData);
void sub_02067484(FieldSystem *fieldSystem, void *);
u32 sub_020674A4(u32 a0);
void sub_020674BC(SaveData *saveData);
u32 sub_020674E0(SaveData *saveData);
u32 sub_0206751C(SaveData *saveData);
BOOL FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem);
void sub_0206759C(SaveData *saveData, int a1);

#endif // POKEHEARTGOLD_UNK_02066EDC_H
