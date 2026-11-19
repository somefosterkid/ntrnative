#ifndef POKEHEARTGOLD_OVERLAY_12_MENU_TASK_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_MENU_TASK_DATA_H

#include "battle/battle.h"

// Task data of the opponent controller's input menus (allocated in overlay_12_battle_controller_opponent_0225A414.c,
// freed by the ov12_0225E4CC / ov12_0225E6FC / ov12_0225FC80 tasks). The healthbox member is a pointer (8 bytes on the PC).

typedef struct TargetMonData {
    u8 partySlot;
    u8 gender : 2;
    u8 isPresent : 1;
    u8 : 5;
    u8 stockStatus;
    u8 padding_03;
    s16 curHP;
    u16 maxHP;
} TargetMonData;

typedef struct MoveSelectMenuData {
    BattleSystem *battleSys;
    void *healthbox;
    int input;
    u16 moves[4];
    u8 ppCur[4];
    u8 ppMax[4];
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 partySlot;
    u8 state;
    u8 unused;
    u16 invalidMoves;
} MoveSelectMenuData;

typedef struct TargetSelectMenuData {
    BattleSystem *battleSys;
    void *healthbox;
    int input;
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 state;
    TargetMonData targetMon[4];
    u16 range;
    u8 shouldHidePanel;
    u8 unused;
} TargetSelectMenuData;

typedef struct YesNoMenuData {
    BattleSystem *battleSys;
    void *healthbox;
    int input;
    u8 command;
    u8 battler;
    u8 state;
    u8 yesNoType;
    int promptMsg;
    int nickname;
    u16 move;
    u16 msgIdx;
} YesNoMenuData;

#endif // POKEHEARTGOLD_OVERLAY_12_MENU_TASK_DATA_H
