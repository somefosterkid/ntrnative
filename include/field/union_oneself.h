// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_oneself.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_UNION_ONESELF_H
#define BWPORT_FIELD_UNION_ONESELF_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black 2's; twinned code confirms the values of UNION_STEP_IDLE, UNION_STEP_RESET, UNION_STEP_LEAVE,
// UNION_STEP_COLOSSEUM_IDLE and UNION_STEP_COLOSSEUM_END in Black
// The steps of this machine (UnionSelf.step): a row of union_oneself.c's table, with a function to begin the step, one
// to run it and one to end it. The rows from UNION_STEP_JOIN_DRAW to UNION_STEP_JOIN_EGG are those of the activities,
// UNION_STEP_FOR_ACTIVITY(activity)
enum {
    UNION_STEP_IDLE = 0,
    UNION_STEP_RESET,
    UNION_STEP_NONE,
    UNION_STEP_COUNTER,
    UNION_STEP_GREETING,
    UNION_STEP_ASK,
    UNION_STEP_ASKED,
    UNION_STEP_PARENT,
    UNION_STEP_OFFER,
    UNION_STEP_OFFER_PARTY,
    UNION_STEP_RULE_SELECT,
    UNION_STEP_ANSWER,
    UNION_STEP_ANSWER_MEMBER,
    UNION_STEP_CARD,
    UNION_STEP_JOIN_DRAW,
    UNION_STEP_JOIN_BATTLE_0,
    UNION_STEP_JOIN_BATTLE_1,
    UNION_STEP_JOIN_BATTLE_2,
    UNION_STEP_JOIN_BATTLE_3,
    UNION_STEP_JOIN_BATTLE_4,
    UNION_STEP_JOIN_BATTLE_5,
    UNION_STEP_JOIN_BATTLE_6,
    UNION_STEP_JOIN_BATTLE_7,
    UNION_STEP_JOIN_BATTLE_8,
    UNION_STEP_JOIN_BATTLE_9,
    UNION_STEP_JOIN_BATTLE_10,
    UNION_STEP_JOIN_BATTLE_11,
    UNION_STEP_JOIN_BATTLE_12,
    UNION_STEP_JOIN_BATTLE_13,
    UNION_STEP_JOIN_BATTLE_14,
    UNION_STEP_JOIN_BATTLE_15,
    UNION_STEP_JOIN_MULTI_0,
    UNION_STEP_JOIN_MULTI_1,
    UNION_STEP_JOIN_MULTI_2,
    UNION_STEP_JOIN_MULTI_3,
    UNION_STEP_TRADE,
    UNION_STEP_JOIN_EGG,
    UNION_STEP_LEAVE_2,
    UNION_STEP_RECRUIT,
    UNION_STEP_LEAVE,
    UNION_STEP_COLOSSEUM,
    UNION_STEP_COLOSSEUM_SEND,
    UNION_STEP_COLOSSEUM_IDLE,
    UNION_STEP_COLOSSEUM_SIT,
    UNION_STEP_COLOSSEUM_STAND,
    UNION_STEP_PARTY_SELECT,
    UNION_STEP_PARTY_WAIT,
    UNION_STEP_PARTY_SEND,
    UNION_STEP_BATTLE_LIST,
    UNION_STEP_QUIT,
    UNION_STEP_COLOSSEUM_END,
    UNION_STEP_COLOSSEUM_CARD,
    UNION_STEP_END,
};

#define UNION_STEP_FOR_ACTIVITY(activity) ((activity) + 11)

// Black 2's, not checked against Black
// The activities that this file names; the others are a number of the menu
#define UNION_ACTIVITY_TRADE 0x18

// Black 2's, not checked against Black
#define UNION_ACTIVITY_EGG_TRADE 0x19
#define UNION_ACTIVITY_NONE 0x1b

// pokebw2's Misc_IsMetTrainer
// Functions outside this file that it calls, without their own header yet
// (main) whether the trainer ID is one in the rival data
BOOL func_0200edc4(RivalDataSave *rival, u32 trainerId);
// (ov021) pokebw2's func_ov036_021b445c: puts a player met on the screen's list, an entry's or a name's
void func_ov021_021bba70(UnionSystem *unisys, UnionBeaconEntry *entry, const u16 *name, u8 flag);
// Black's overlay 21: the colosseum's table of the players in the field (pokebw2's CommEntryMenu). It is made with the
// player, the field, how many players, the kind (0 the leader, 1 picks a leader, 2 and 3 join one), the activity, and
// given the functions that tell of a player who comes and goes
// Called with the work of func_ov021_021cd60c when a player joins the group or leaves it (pokebw2's CommEntryMenuCallback)
typedef void (*CommEntryMenuCallback)(UnionSystem *unisys, PlayerInfo *info, const u8 *mac);
CommEntryMenu *func_ov021_021cd48c(PlayerInfo *info, Field *field, u32 minCount, u32 maxCount, HeapID heapId, u32 kind,
                                   u32 mode, const u8 *mac);
void func_ov021_021cd60c(CommEntryMenu *menu, UnionSystem *unisys, CommEntryMenuCallback onJoin,
                         CommEntryMenuCallback onLeave);
// A player who comes into the table
void func_ov021_021cd624(CommEntryMenu *menu, u32 netId, PlayerInfo *info, u8 autoAccept, const u8 *mac);
// How the table ended: 0 not yet, 1 to 4
u32 func_ov021_021cd688(CommEntryMenu *menu);
// Frees the table, and says what it asked for (a byte the colosseum keeps)
u32 func_ov021_021cd5a4(CommEntryMenu *menu);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UnionOneself_CheckEntries
void func_ov019_0217b75c(UnionSystem *unisys);
// pokebw2's UnionOneself_CreateCommandEvent
GameEvent *func_ov019_0217b80c(GameSystem *gsys, Field *field);
// pokebw2's UnionOneself_ReturnFalse
GameEvent *func_ov019_0217b83c(GameSystem *gsys, Field *field);
// pokebw2's UnionOneself_RunStep
u8 func_ov019_0217b840(UnionSystem *unisys, Field *field);
// pokebw2's UnionOneself_SetUpdateDisabled
void func_ov019_0217baf0(UnionSystem *unisys, Field *field, u8 disable);

#endif // BWPORT_FIELD_UNION_ONESELF_H
