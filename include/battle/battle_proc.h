// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/battle_proc.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_BATTLE_PROC_H
#define BWPORT_BATTLE_BATTLE_PROC_H

#include "types.h"
#include "gfl/overlay.h"
#include "struct_decls.h"
#include "app/ov306.h"
#include "battle/vs_multi_list.h"

// Overlay 8 starts a battle
#define OVERLAY_BATTLE OVERLAY_ID(8)

typedef struct {
    PokeParty *party;
    PlayerInfo *info;
    u8 unk08[2];
    // The party slots of the Pokémon the player sends out
    u8 order[6];
} BattlePlayer;

// The players of a battle, and the result that the battle proc leaves
typedef struct {
    BattlePlayer players[4];
    u32 result;
    u32 unk44;
    u32 rule;
    u32 unk4C;
} BattlePlayers;

// Black 2's; twinned code confirms the offsets of gameData, setup, players, rule, unk10 and unk14 in Black
struct BattleParam {
    GameData *gameData;
    BtlSetup *setup;
    BattlePlayers *players;
    u32 rule;
    u32 unk10;
    u32 unk14;
    Ov306Param ov306Param;
};

// Black 2's; twinned code confirms the offsets of manager in Black
// The battle proc's work
struct BattleProcWork {
    GameProcManager *manager;
    // What overlay 171 gets
    VsMultiListParam ov171Param;
    BOOL ov167Loaded;
    BOOL commandsSet;
    BOOL vsPlayerLoaded;
};

BOOL func_ov008_02154ca0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov008_02154cc8(GameProc *proc, u32 *state, void *param, void *work);

// Black 2's, not checked against Black
#define OVERLAY_BATTLE_MAIN OVERLAY_ID(93) // Black's
#define OVERLAY_BATTLE_SELECT OVERLAY_ID(213)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL func_ov008_02154cf8(GameProc *proc, u32 *state, void *param, void *work);
// The battle proc's proc functions, which start a battle with OVERLAY_NONE
extern const GameProcFunctions data_ov093_021efdac;
extern const GameProcFunctions data_ov097_021f75ec;
extern const GameProcFunctions data_ov214_021bb474;

#endif // BWPORT_BATTLE_BATTLE_PROC_H
