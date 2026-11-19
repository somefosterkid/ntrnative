#ifndef POKEHEARTGOLD_BATTLE_BATTLE_BAG_CONTEXT_H
#define POKEHEARTGOLD_BATTLE_BATTLE_BAG_CONTEXT_H

#include "constants/heap.h"

// The in-battle bag menu's arguments (BattleBag.context): the battle controller fills it in. DS size 0x34. The bag
// menu's files used to read heapID / battleSys through a raw `(u8 *)context + 0xC`, which on a 64-bit host lands inside
// trainerInfo (three 8-byte pointers in front of heapID): a garbage heap id at NARC_New / BattleCursor_New.

struct BattleSystem;
struct PlayerProfile;
struct Bag;

typedef struct BattleBagContext {
    struct BattleSystem *battleSys;
    struct PlayerProfile *trainerInfo;
    struct Bag *bag;
    enum HeapID heapID;
    s32 battler;
    BOOL isInCatchTutorial;
    u32 embargoRemainingTurns;
    u16 selectedBattleBagItem;
    u8 selectedBattleBagPocket;
    u8 lastUsedItemPocket;
    u16 lastUsedItem;
    u8 hasTwoOpponents;
    u8 opponentHidden;
    u8 opponentSubstituted;
    u8 isCursorEnabled;
    u8 battleBagExited;
    u8 pocketCurrentPagePositions[5];
    u8 pocketCurrentPages[5];
} BattleBagContext;

#endif // POKEHEARTGOLD_BATTLE_BATTLE_BAG_CONTEXT_H
