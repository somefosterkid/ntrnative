// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_colosseum_battle.h.
#ifndef BWPORT_FIELD_EVENT_COLOSSEUM_BATTLE_H
#define BWPORT_FIELD_EVENT_COLOSSEUM_BATTLE_H

// Overlay 10's event_colosseum_battle.c (a descriptive name): the battle that the Union Room colosseum starts, from
// the play category the players chose

#include "types.h"
#include "battle/battle_proc.h"
#include "struct_decls.h"

// The players of a colosseum battle, as colosseum.c's func_ov012_02175280 fills them (0x58 bytes in Black)
struct ColosseumPlayers {
    BattlePlayers battle;
    u32 unk50;
    GameRecords *records;
};

typedef struct {
    PokeParty *party;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u16 bgm;
    u8 unk12;
    Regulation *regulation;
    // The battle's players and what follows them (Black's param is 0x70 bytes), which colosseum.c fills
    ColosseumPlayers players;
} ColosseumBattleParam;

GameEvent *func_ov010_02157450(GameSystem *gsys, Field *field, u32 category, ColosseumBattleParam *param);

#endif // BWPORT_FIELD_EVENT_COLOSSEUM_BATTLE_H
