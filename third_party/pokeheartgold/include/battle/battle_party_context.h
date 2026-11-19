#ifndef POKEHEARTGOLD_BATTLE_BATTLE_PARTY_CONTEXT_H
#define POKEHEARTGOLD_BATTLE_BATTLE_PARTY_CONTEXT_H

#include "constants/heap.h"

// The in-battle party menu's arguments: the battle controller allocates it (ov12_0225A700, ov12_0225F4E0,
// ov12_022609F8: Heap_Alloc sizeof), ov08_0221BE20 opens the menu (BattleParty.context) and fills the result in.
// DS size 0x38.

struct BattleSystem;
struct Party;

typedef struct BattlePartyContext {
    struct Party *party;              // 0x00
    void *sLotoId;                    // 0x04
    struct BattleSystem *battleSys;   // 0x08
    enum HeapID heapID;               // 0x0C
    u8 unused2;                       // 0x10
    u8 selectedPartyIndex;            // 0x11
    u8 doubleBattleFirstSelectedPartySlot;
    u8 unused3;
    u8 playerPokemonPartySlot;        // 0x14
    u8 partnerPokemonPartySlot;
    u32 embargoRemainingTurns[2];     // 0x18
    u16 currentDamage;                // 0x20
    u16 selectedBattleBagItem;
    u16 moveToLearn;                  // 0x24
    s32 battler;                      // 0x28
    u8 pokemonPartySlots[6];          // 0x2C
    u8 isCursorEnabled;
    u8 selectedBattleBagPocket;
    u8 selectedMoveSlot;              // 0x34
    u8 battlePartyMode;
    u8 battlePartyExited;
} BattlePartyContext;

void ov08_0221BE20(BattlePartyContext *context);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_PARTY_CONTEXT_H
