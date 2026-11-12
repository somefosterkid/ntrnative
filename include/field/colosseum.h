// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/colosseum.h: the lines in use so far.
#ifndef BWPORT_FIELD_COLOSSEUM_H
#define BWPORT_FIELD_COLOSSEUM_H

#include "types.h"
#include "field/trcard_sys.h"
#include "save/player_info.h"
#include "struct_decls.h"

#define COLOSSEUM_MEMBER_MAX 4

// Black 2's, not checked against Black
// What a player sends the others (0x14 bytes, command 0x1503)
struct ColosseumChoice {
    u32 unk00[5];
};

// Black 2's; twinned code confirms the offsets of unk26 in Black
// A player of the colosseum
struct ColosseumMember {
    PlayerInfo info;
    u8 mac[6];
    u8 unk26;
    u8 unk27;
    u8 unk28;
};

// Black's layout (0x170 bytes). Black 2's is 0x10 bigger from the cards on
struct UnionColosseum {
    CommPlayerSys *commPlayer; // from func_ov012_021613d0
    u8 unk04[0x14];
    ColosseumMember members[COLOSSEUM_MEMBER_MAX];
    u8 unkC8_0 : 1;
    u8 unkC8_1 : 1;
    u8 unkC8_2 : 6;
    u8 unkC9;
    u8 unkCA;
    u8 unkCB;
    u8 unkCC[4];
    u8 unkD0[4];
    u8 unkD4[4];
    u8 unkD8[4];
    u8 unkDC;
    u8 unkDD;
    u8 unkDE;
    u8 unkDF;
    ColosseumChoice choice[COLOSSEUM_MEMBER_MAX];
    u8 choiceReady[4];
    TrainerCardData *card[COLOSSEUM_MEMBER_MAX]; // where a player's card arrives
    u8 cardReady[4];
    PokeParty *party[COLOSSEUM_MEMBER_MAX];
    u8 partyReady[4];
    u8 unk15C[4];
    u8 unk160;
    u8 unk161;
    u8 unk162[4];
    u8 unk166[2];
    void *unk168; // a table of the field's players (see func_ov036_021c3f34)
    u8 unk16C[3];
    u8 unk16F;
};

u8 func_ov012_021751dc(UnionColosseum *colosseum, u32 netId, ColosseumChoice *choice);

// colosseum.c's functions that colosseum_comm.c's handlers call
u8 func_ov012_021750c0(const UnionColosseum *colosseum);
void func_ov012_021750c8(UnionColosseum *colosseum, u32 value);
BOOL func_ov012_021750d8(UnionColosseum *colosseum, u32 netId, u8 value);
void func_ov012_021751b4(UnionColosseum *colosseum, u32 netId, const ColosseumChoice *choice);

// colosseum_comm.c's that colosseum.c calls
BOOL func_ov012_02174d2c(UnionColosseum *colosseum, u32 netId, u32 value);
BOOL func_ov012_02174e9c(u32 netId);
BOOL func_ov012_02174ed8(void);

// pokebw2's Colosseum_CommandBattle
// Union commands (rows of union_main.c's table): the battle's event, and the greeting's phrase select. They take the
// GameSystem, the UnionSystem, the Field, an argument, the event that runs them, one to give them an event to chain and
// their step
BOOL func_ov012_02174ad8(GameSystem *gsys, UnionSystem *sys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step);
// pokebw2's Colosseum_RegisterCommands
// Registers the colosseum's net commands, with the UnionSystem as their work
void func_ov012_02174b40(UnionSystem *sys);
// pokebw2's Colosseum_SendMember
// Sends the player's info (to the parent only when toParent is TRUE, else to all), the card, and the card's first part
BOOL func_ov012_02174bb8(const void *member, BOOL toParent);
// pokebw2's Colosseum_SendCardHead
BOOL func_ov012_02174c34(const TrainerCardData *card);
// pokebw2's Colosseum_SendChoice
BOOL func_ov012_02174c90(const ColosseumChoice *choice);
BOOL func_ov012_02174ce4(UnionColosseum *colosseum);
// pokebw2's Colosseum_SendParty
BOOL func_ov012_02174d98(const PokeParty *party);
// pokebw2's Colosseum_SendResult
BOOL func_ov012_02174df0(const void *result);
BOOL func_ov012_02174e20(void);
BOOL func_ov012_02174e64(void);
BOOL func_ov012_02174f34(void);
void func_ov012_02175094(UnionColosseum *colosseum, BOOL value);
void func_ov012_021750b0(UnionColosseum *colosseum, u32 value);
u8 func_ov012_021750d0(const UnionColosseum *colosseum);
void func_ov012_02175110(UnionColosseum *colosseum);
void func_ov012_02175144(UnionColosseum *colosseum);
void func_ov012_0217534c(ColosseumPlayers *players);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Colosseum_CommandPmsSelect
BOOL func_ov012_02174b08(GameSystem *gsys, UnionSystem *sys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step);
// pokebw2's Colosseum_Create
// The colosseum's work, made for the player and freed
UnionColosseum *func_ov012_02174f4c(GameData *gameData, GameSystem *gsys, const PlayerInfo *info, u32 flag);
// pokebw2's Colosseum_Free
void func_ov012_0217505c(UnionColosseum *colosseum, GameSystem *gsys);
void func_ov012_02175208(UnionColosseum *colosseum);
BOOL func_ov012_02175214(const UnionColosseum *colosseum);
void func_ov012_02175240(UnionColosseum *colosseum, BOOL keepOwn);
// pokebw2's Colosseum_BuildBattlePlayers
void func_ov012_02175280(UnionColosseum *colosseum, GameSystem *gsys, ColosseumPlayers *players, u32 unused);
// pokebw2's Colosseum_RegisterCommands
// Registers the colosseum's net commands, with the UnionSystem as their work
// pokebw2's Colosseum_SendMember
// Sends the player's info (to the parent only when toParent is TRUE, else to all), the card, and the card's first part
// pokebw2's Colosseum_SendCardHead
// pokebw2's Colosseum_SendParty
// pokebw2's Colosseum_SendResult

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Colosseum_Free

#endif // BWPORT_FIELD_COLOSSEUM_H
