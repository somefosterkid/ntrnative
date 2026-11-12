// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/wbt.h: the lines in use so far.
#ifndef BWPORT_FIELD_WBT_H
#define BWPORT_FIELD_WBT_H

#include "types.h"
#include "battle/regulation.h"
#include "gfl/heap.h"
#include "nitro/math.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// One of the eight Trainers of a tournament
typedef struct {
    u8 unk0_0 : 3; // 3 for the player
    u8 unk0_3 : 1;
    u8 unk0_4 : 3; // The Trainer's rank, which decides a match between ranks that differ
    u8 unk0_7 : 1;
    u8 type;
    u8 unk2;
    u8 unk3_0 : 6;
    u8 unk3_6 : 2;
    u16 objCode;
    u16 unk6_0 : 12;
    u16 unk6_12 : 4;
    StrBuf *name;
    StrBuf *unkC;
} WbtEntrant;

// Black 2's, not checked against Black
// The result of a match of the bracket: whether the first Trainer won, and how
typedef struct {
    u8 firstWon;
    u8 score;
} WbtMatch;

// Black 2's, not checked against Black
typedef struct {
    u8 type;
    u8 unk1;
    u16 unk2;
} WbtUnk18A;

// Black 2's, not checked against Black
// A tournament downloaded to the save
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u8 unk8[0x52];
    u8 unk5A[0x4c];
    u8 unkA6;
    u8 unkA7;
    u8 unkA8;
    u8 unkA9;
    u16 unkAA;
} WbtDownloadedTournament;

// Black 2's, not checked against Black
// A Pokémon of a Trainer of a downloaded tournament, which becomes a party Pokémon
typedef struct {
    u16 species;
    u16 nickname[11];
    u16 item;
    u8 unk1A_0 : 1;
    u8 unk1A_1 : 7;
    // The ability, by its number
    u8 ability;
    u8 evs[6];
    u16 moves[4];
    // Two bits per move
    u8 ppUps;
    u8 nature;
    struct {
        u32 hp : 5;
        u32 attack : 5;
        u32 defense : 5;
        u32 speed : 5;
        u32 spAttack : 5;
        u32 spDefense : 5;
        u32 sex : 2;
    } ivs;
    u8 form;
    u8 ball;
    u8 level;
    u8 happiness;
} WbtDownloadedPokemon;

// Black 2's, not checked against Black
// A Trainer of a downloaded tournament: up to six Pokémon, and four lines of 74 characters that the Trainer says,
// each unless its message set is 0 a message of that set of the Trainers' messages instead
typedef struct {
    WbtDownloadedPokemon pokemon[6];
    // The Trainer's name and Trainer class name
    u16 name[16];
    u16 className[0x16];
    u16 lines[4][0x4a];
    u8 unk3D4;
    u8 messageSets[6];
    u8 unk3DB;
    u16 unk3DC;
    u8 unk3DE[2];
} WbtDownloadedTrainer;

// Black 2's, not checked against Black
// A tournament downloaded to the save, with the Trainers of its four rounds
typedef struct {
    u8 unk0[0xa];
    u16 name[0x25];
    u8 unk54[0x174];
    WbtDownloadedTournament tournament;
    WbtDownloadedTrainer trainers[4];
} WbtDownload;

// Black 2's, not checked against Black
struct WbtSystem {
    HeapID heapId;
    u8 unk2[2];
    BOOL unk4;
    WbtTrainers *trainers;
    u32 tournament;
    // The battle style, which starts out as 4, out of range
    u32 style;
    u32 unk14;
    u32 round;
    // The Type Expert Tournament's type
    u8 type;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u64 seed;
    MATHRandContext32 rand;
    WbtEntrant entrants[8];
    WbtMatch matches[7];
    Regulation regulation;
    WbtUnk18A unk18A;
    u8 unk18E[2];
    // Whether the downloaded tournament of the slot unk1D is in the save and its checksum holds
    BOOL downloadValid;
    WbtDownload download;
    // The kind and rank of the seven Trainers a downloaded tournament picks, from the second
    WbtUnk18A downloadSlots[7];
    u8 unk13A4[4];
    u8 unk13A8;
    u8 unk13A9[3];
    // The Battle Box's party
    PokeParty *unk13AC;
    u8 castSlots[6];
    // The Pokémon the party screen showed the summary of, and how it ended
    u16 listIndex;
    u16 listResult;
    u8 unk13BA[2];
    PokeParty *partyBC;
    PokeParty *partyC0;
    PokeParty *partyC4;
    u8 unk13C8[4];
    void *unk13CC;
    u8 unk13D0[4];
    // The texts that the battle shows after winning and after losing
    StrBuf *unk13D4;
    StrBuf *unk13D8;
    // One for each round. Tournaments 1 and 10 award their total as Battle Points
    u8 unk13DC[3];
    u8 unk13DF;
    // The script variable that plugin 6 passes with EventWbtPokeSelect_Create's event
    u16 *unk13E0;
    u32 unk13E4;
};

// Black 2's, not checked against Black
typedef struct {
    u16 name;
    // Which of the save's 29 counts the tournament's wins go to. The Type Expert Tournament, tournament 2, counts
    // each of the 17 types in 0 to 16 instead
    u8 record;
    u8 unk3;
    u8 kind;
    u8 unk5;
    // The Battle Points for winning, by battle style
    u8 battlePoints[4];
} WbtTournamentInfo;

#endif // BWPORT_FIELD_WBT_H
