// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/trial_house.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_TRIAL_HOUSE_H
#define BWPORT_FIELD_TRIAL_HOUSE_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "field/bsubway_scr.h"
#include "system/game_event.h"

struct TrialHouseWork *CreateTrialHouseWk(GameSystem *gsys);
void TrialHouseWorkDelete(void *unused, struct TrialHouseWork **work);

// Black 2's; twinned code confirms the offsets of trainer, heapId, capacity, party, selectionFlag and saveBuffer, and
// its size in Black
struct TrialHouseWork {
    BSubwayTrainer trainer;
    u16 heapId;
    u8 unk122[2];
    int capacity;
    PokeParty *party;
    u32 battleType;
    u32 selectionFlag;
    // The battle's statistics, which TrialHouseCalcPointScore scores
    u16 stats[12];
    void *saveBuffer;
    u32 initState;
};

GameEvent *func_ov010_02168bf0(GameSystem *gsys, TrialHouseWork *work, u32 mode, BOOL battleBox, u16 *result);
GameEvent *func_ov010_02168e5c(GameSystem *gsys, u32 a1, u32 a2);

u32 func_ov021_021d0ac0(GameSystem *gsys, TrialHouseWork *work, u32 mode);
// The Trainer's message in a balloon over the actor
GameEvent *func_ov021_021d0bd8(GameSystem *gsys, TrialHouseWork *work, u32 index, u32 actorId);
void func_ov021_021d0ab8(TrialHouseWork *work, u32 selectionFlag);
u32 func_ov021_021d0fe0(GameSystem *gsys, TrialHouseWork *work);
GameEvent *func_ov021_021d0fe8(GameSystem *gsys, TrialHouseWork *work, u32 mode);
u32 func_ov021_021d1028(GameSystem *gsys);

void func_ov021_021d0ae4(TrialHouseWork *work, u32 mode);
u32 func_ov021_021d0bcc(TrialHouseWork *work);
GameEventReturnCode func_ov021_021d0c58(GameEvent *event, u32 *state, void *data);
u8 func_ov021_021d1058(TrialHouseSave *save, u32 index);
void func_ov021_021d1080(TrialHouseSave *save, u32 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void TrialHouseCalcPointScore(GameSystem *gsys, TrialHouseWork *work, u16 *rankOut, u16 *pointsOut);
GameEvent *CallTrialHouseBattle(GameSystem *gsys, TrialHouseWork *work);
void SyncTrialHouseWkStatsFromBattle(TrialHouseWork *work, BtlSetup *setup);
void func_ov021_021d0a74(TrialHouseWork *work, u32 mode);
GameEvent *func_ov021_021d0be4(GameSystem *gsys, TrialHouseWork *work, u16 *result);
void func_ov021_021d09d0(GameSystem *gsys, TrialHouseWork *work);
void func_ov021_021d0b58(GameSystem *gsys, TrialHouseWork *work, u32 mode);
GameEventReturnCode func_ov021_021d10a8(GameEvent *event, u32 *state, void *data);

#endif // BWPORT_FIELD_TRIAL_HOUSE_H
