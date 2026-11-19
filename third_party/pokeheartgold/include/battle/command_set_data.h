#ifndef POKEHEARTGOLD_BATTLE_COMMAND_SET_DATA_H
#define POKEHEARTGOLD_BATTLE_COMMAND_SET_DATA_H

#include "global.h"
#include "battle/battle.h"

// SysTask data of the command-selection tasks (OpponentData.taskFuncs.setCommandSelection), 0x3C bytes on the DS;
// allocated by ov12_0225A524. The first two members are pointers, so command/battler sit at +0x10 on a 64-bit host.
typedef struct CommandSetData {
    BattleSystem *battleSys;
    void *healthbox;
    u8 command;
    u8 battler;
    u8 state;
    s8 unused_0B;
    int input;
    u8 ballStatus[2][6]; // [PARTY_GAUGE_COUNT]
    u8 expPercents[6];
    u8 unused_22;
    u8 partySlot;
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    u8 battlerType;
    u8 msgIdx;
    s16 curHP;
    u16 maxHP;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
} CommandSetData;

#endif // POKEHEARTGOLD_BATTLE_COMMAND_SET_DATA_H
