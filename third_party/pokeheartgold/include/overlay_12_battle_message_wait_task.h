#ifndef POKEHEARTGOLD_OVERLAY_12_BATTLE_MESSAGE_WAIT_TASK_H
#define POKEHEARTGOLD_OVERLAY_12_BATTLE_MESSAGE_WAIT_TASK_H

// BattleMessageWaitTask: merged by tools/hg/promote.py from 4 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225A414.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;

typedef struct BattleMessageWaitTask {
    struct BattleSystem *battleSys;
    u8 command;
    u8 battler;
    u8 msgIdx;
    u8 pad_7[0x1];
} BattleMessageWaitTask;

// sizeof(BattleMessageWaitTask) == 0x8

#endif // POKEHEARTGOLD_OVERLAY_12_BATTLE_MESSAGE_WAIT_TASK_H
