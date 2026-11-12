// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_eff.h: the lines in use so far.
#ifndef BWPORT_FIELD_UNION_EFF_H
#define BWPORT_FIELD_UNION_EFF_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's UnionEff_IsPlayerOnSpot
// Whether the player stands on the Union Room's spot (the one entry of its table)
BOOL func_ov019_0217f7e4(Field *field);
// pokebw2's UnionEff_GetKindStep
// A step of the Union Room's actors told in kinds: 1 for 4 to 0x17, 4 for 3, 2 for 0x18, 5 for 0x19, 3 for 0x1a
int func_ov019_0217f858(int kind);
// pokebw2's UnionEff_IsFriendBeacon
// Whether the beacon's player is one of the wifi list's friends
BOOL func_ov019_0217f894(WifiList *wifiList, const UnionBeacon *beacon);
// pokebw2's UnionEff_Create
void *func_ov019_0217f8c0(HeapID heapId);
// pokebw2's UnionEff_Free
void func_ov019_0217f8ec(void *work);
// pokebw2's UnionEff_Update
void func_ov019_0217f914(void *work);
// pokebw2's UnionEff_AddFocus
// Focus on an actor
void func_ov019_0217f93c(void *work, FieldActor *actor);
// pokebw2's UnionEff_AddMark
// Put a mark over an actor, the asonda one when asonda is TRUE and else the tomo one
void func_ov019_0217f948(void *work, FieldActor *actor, BOOL asonda);
// pokebw2's UnionEff_RemoveMark
// Take the mark off an actor
void func_ov019_0217f958(void *work, FieldActor *actor);
// pokebw2's UnionEff_AllColosseumMembersHere
// Whether the cards of all the players are in
BOOL func_ov019_0217fdc8(UnionColosseum *colosseum);
// pokebw2's UnionEff_GetEntrySpot
BOOL func_ov019_0217fe44(Field *field, u32 kind, u32 *index);
// pokebw2's UnionEff_IsOnSeatSpot
BOOL func_ov019_0217ff14(Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UnionEff_Draw
void func_ov019_0217f928(void *work);
// pokebw2's UnionEff_AllColosseumCardsIn
BOOL func_ov019_0217fdf8(UnionColosseum *colosseum);
// pokebw2's UnionEff_IsColosseumUnk170Set
BOOL func_ov019_0217fe24(UnionColosseum *colosseum);
// pokebw2's UnionEff_ClearColosseumUnk170
void func_ov019_0217fe38(UnionColosseum *colosseum);
// pokebw2's UnionEff_AllColosseumReady
BOOL func_ov019_0217feb8(UnionColosseum *colosseum);
// pokebw2's UnionEff_AnyColosseumUnk172
BOOL func_ov019_0217fef4(UnionColosseum *colosseum);

#endif // BWPORT_FIELD_UNION_EFF_H
