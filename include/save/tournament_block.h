// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/tournament_block.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_TOURNAMENT_BLOCK_H
#define BWPORT_SAVE_TOURNAMENT_BLOCK_H

#include "types.h"
#include "struct_decls.h"
#include "battle/regulation.h"

// Black 2's; twinned code confirms the offsets of unkBC, unk152, unk154, unk155, unk156, unk157, unk158, unk159,
// unk15A, unk15B, unk15C, unk15D and crc in Black
typedef struct {
    Regulation regulation;
    u32 unkBC;
    u8 unkC0[0x92];
    s16 unk152;
    s8 unk154;
    s8 unk155;
    s8 unk156;
    s8 unk157;
    s8 unk158;
    s8 unk159;
    s8 unk15A;
    // Field 10 of TournamentEntry_GetField
    s8 unk15B;
    u8 unk15C;
    u8 unk15D;
    // getCRC16 of everything before it
    u16 crc;
} TournamentEntry; // size: 0x160

void *getTournamentBlockAddress(SaveControl *save);
// pokebw2's TournamentBlock_GetEntry
// An entry of the block
void *func_0200b1d4(void *block, u32 index);
// pokebw2's TournamentEntry_GetField
// An entry's field, and setting it. The Wi-Fi login reads field 2 and field 10 and sets field 10
u32 func_0200b078(void *entry, u32 field);
// The checksum of an entry's first 350 bytes
u16 CRC16_350bytes(const void *entry);
// pokebw2's TournamentEntry_SetField
void func_0200b110(void *entry, u32 field, u32 value);
// pokebw2's TournamentBlock_GetTeam
// The block's other entries, of 0x1e bytes
void *func_0200b204(void *block, u32 index);
// pokebw2's TournamentTeam_Clear
void func_0200b1f8(void *team);
// The entry's data of 0x92 bytes
void *func_0200b074(void *entry);

// pokebw2's TournamentTeam_GetField
// A field of the Pokémon number index of such an entry: the species (0), the form (1), whether it holds an item (2)
// and its gender (3)
u32 func_0200b214(void *record, u32 field, u8 index);

// The CRC16 of the first 0x15e bytes of an entry

// Black 2's, not checked against Black
#define TOURNAMENT_ENTRY_MAX 2
#define TOURNAMENT_TEAM_MAX 6

// The fields of an entry, for TournamentEntry_GetField and TournamentEntry_SetField
enum {
    TOURNAMENT_FIELD_UNKBC = 1,
    TOURNAMENT_FIELD_UNK152,
    TOURNAMENT_FIELD_UNK154,
    TOURNAMENT_FIELD_UNK155,
    TOURNAMENT_FIELD_UNK156,
    TOURNAMENT_FIELD_UNK157,
    TOURNAMENT_FIELD_UNK158,
    TOURNAMENT_FIELD_UNK159,
    TOURNAMENT_FIELD_UNK15A,
    TOURNAMENT_FIELD_UNK15B,
    TOURNAMENT_FIELD_UNK15C,
    TOURNAMENT_FIELD_UNK15D
};

// The fields of a team's record, for TournamentTeam_GetField
enum {
    TOURNAMENT_TEAM_SPECIES,
    TOURNAMENT_TEAM_FORM,
    TOURNAMENT_TEAM_HAS_ITEM,
    TOURNAMENT_TEAM_GENDER
};

// Black 2's; twinned code confirms the offsets of species, form, hasItem and gender in Black
// The Pokémon of a team entered
struct TournamentTeam {
    u16 species[TOURNAMENT_TEAM_MAX];
    u8 form[TOURNAMENT_TEAM_MAX];
    // A bit for each Pokémon: whether it holds an item, and its gender
    u8 hasItem;
    u8 gender;
    u8 unk14[0xa];
}; // size: 0x1e

// Black 2's; twinned code confirms the offsets of entries in Black
struct TournamentBlock {
    TournamentEntry entries[TOURNAMENT_ENTRY_MAX];
    TournamentTeam teams[TOURNAMENT_ENTRY_MAX];
}; // size: 0x2fc

u32 getSizeofTourneyBlock(void);
void clearTournamentBlock(void *block);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Clears the player's VS data
void clearVSPlayer(void);
// An entry's regulation, which is the entry itself, and a parameter of the regulation. The battle type is 0x15
Regulation *func_0200ae14(void *entry);

#endif // BWPORT_SAVE_TOURNAMENT_BLOCK_H
