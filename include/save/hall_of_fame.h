// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/hall_of_fame.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_HALL_OF_FAME_H
#define BWPORT_SAVE_HALL_OF_FAME_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/rtc.h"

#define HALL_OF_FAME_TEAMS 15
#define HALL_OF_FAME_TEAM_SIZE 6

// A Pokémon of a team, as HallOfFameSave_GetPokemon copies it out. The names are loaded into the caller's buffers
typedef struct {
    StrBuf *nickname;
    StrBuf *trainerName;
    u32 personality;
    u32 trainerId;
    u16 species;
    u8 level;
    u8 form : 6;
    u8 sex : 2;
    u16 moves[4];
} HallOfFamePokemon;

// pokebw2's HallOfFameSave_AddTeam
// Adds a team to the Hall of Fame, dated date, with its strings in the heap
void func_0200ebd0(HallOfFameSave *save, PokeParty *party, const RTCDate *date, HeapID heapId);

// pokebw2's HallOfFameTeam_GetPokemonCount
// A team's number of Pokémon, one of its Pokémon and its date
s32 func_0200ec14(HallOfFameTeam *team);
// pokebw2's HallOfFameTeam_GetDate
void func_0200ec98(const HallOfFameTeam *team, RTCDate *date);
// pokebw2's HallOfFameTeam_GetPokemon
void func_0200ec2c(const HallOfFameTeam *team, s32 slot, HallOfFamePokemon *pokemon);
// pokebw2's HallOfFameSave_GetPokemonCount
// The number of Pokémon in a team, one of its Pokémon and its date, counting back from the newest
s32 func_0200ecd0(HallOfFameSave *save, s32 team);
// pokebw2's HallOfFameSave_GetEntryNumber
// Which entry into the Hall of Fame a team was, counting back from the newest
u32 func_0200ecc4(HallOfFameSave *save, u32 team);
// pokebw2's HallOfFameSave_GetDate
void func_0200ed14(HallOfFameSave *save, s32 team, RTCDate *date);
// pokebw2's HallOfFameSave_GetPokemon
void func_0200ecf0(HallOfFameSave *save, s32 team, s32 slot, HallOfFamePokemon *pokemon);

#define HALL_OF_FAME_MAX_COUNT 9999
#define HALL_OF_FAME_NICKNAME_LENGTH 11
#define HALL_OF_FAME_TRAINER_NAME_LENGTH 8

// Black 2's; twinned code confirms the offsets of species, level, form, sex, personality, trainerId, nickname,
// trainerName and moves in Black
// A Pokémon of a team, as it is saved
struct HallOfFameSavePokemon {
    u16 species;
    u8 level;
    u8 form : 6;
    u8 sex : 2;
    u32 personality;
    u32 trainerId;
    u16 nickname[HALL_OF_FAME_NICKNAME_LENGTH];
    u16 trainerName[HALL_OF_FAME_TRAINER_NAME_LENGTH];
    u16 moves[4];
    u16 padding;
};

// A team and the date it entered the Hall of Fame. A slot of the team is free when its species is 0
struct HallOfFameTeam {
    HallOfFameSavePokemon pokemon[HALL_OF_FAME_TEAM_SIZE];
    u16 year;
    u8 month;
    u8 day;
};

struct HallOfFameSave {
    HallOfFameTeam teams[HALL_OF_FAME_TEAMS];
    // The team that the next entry overwrites
    u32 next;
    // How many times the player entered the Hall of Fame, up to 9999
    u32 count;
};

// pokebw2's HallOfFameSave_GetSize
// The size and the initial state of the Hall of Fame, and of block 0x40's single team
u32 func_0200ea0c(void);
// pokebw2's HallOfFameTeam_GetSize
u32 func_0200ea14(void);
// pokebw2's HallOfFameSave_Init
void func_0200ea1c(HallOfFameSave *save);
// pokebw2's HallOfFameTeam_Init
void func_0200ea2c(HallOfFameTeam *team);
// pokebw2's HallOfFameSave_GetTeamCount
// The number of teams, at most 15
u32 func_0200ecb4(HallOfFameSave *save);

// Enters the team into the Hall of Fame with its party, its date and the heap
void func_0200ea3c(HallOfFameTeam *team, PokeParty *party, const RTCDate *date, HeapID heapId);

#endif // BWPORT_SAVE_HALL_OF_FAME_H
