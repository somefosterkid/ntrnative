// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/gds_profile.h: the lines in use so far.
#ifndef BWPORT_SAVE_GDS_PROFILE_H
#define BWPORT_SAVE_GDS_PROFILE_H

#include "types.h"
#include "struct_decls.h"
#include "system/pms_data.h"

// pokebw2's GdsProfile_CreateNameStrBuf
// A new string with the player's name
StrBuf *func_0200d6d0(GdsProfile *profile, u32 heapId);
// pokebw2's GdsProfile_GetSex
// The player's sex, or 0 if it isn't valid
u32 func_0200d6ec(GdsProfile *profile);

// Black 2's, not checked against Black
#define GDS_PROFILE_NAME_LENGTH 8
// The length of an introduction in free text
#define GDS_PROFILE_INTRODUCTION_LENGTH 40

// Black 2's; matching code confirms the offsets of name, trainerId, sex, birthMonth, unionView, country, region,
// version, language, egg, form, gender, species, introduction and crc, and its size in Black
struct GdsProfile {
    u16 name[GDS_PROFILE_NAME_LENGTH];
    u32 trainerId;
    u8 sex;
    u8 birthMonth;
    // The trainer's appearance in the Union Room
    u8 unionView;
    u8 country;
    u8 region;
    u8 version;
    u8 language;
    // The Pokémon shown with the profile
    u8 egg : 1;
    u8 form : 6;
    u8 gender : 1;
    u16 species;
    // Whether the introduction is free text instead of a sentence
    u8 introductionIsText;
    u8 padding1F;
    union {
        PMSData pms;
        u16 text[GDS_PROFILE_INTRODUCTION_LENGTH];
    } introduction;
    u8 unk70[0xc];
    // The checksum of everything before it
    u16 crc;
    u16 padding7E;
};

// pokebw2's GdsProfile_Create
// A new profile, with its fields cleared, and the function that frees it
GdsProfile *func_0200d59c(u32 heapId);
// pokebw2's GdsProfile_GetGender
u32 func_0200d72c(GdsProfile *profile);
// pokebw2's GdsProfile_IsEgg
BOOL func_0200d73c(GdsProfile *profile);
// pokebw2's GdsProfile_GetCountry
// Where the player lives
u32 func_0200d74c(GdsProfile *profile);
// pokebw2's GdsProfile_GetBirthMonth
// The month of the player's birthday
u32 func_0200d810(GdsProfile *profile);
// pokebw2's GdsProfile_GetUnionView
// The trainer's appearance in the Union Room
u32 func_0200d820(GdsProfile *profile);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GdsProfile_Free
void func_0200d5c8(GdsProfile *profile);
// pokebw2's GdsProfile_SetFromGameData
// Fills the profile from the player's game data
void func_0200d5d0(GdsProfile *profile, GameData *gameData);
// pokebw2's GdsProfile_GetSpecies
// The Pokémon shown with the profile: its species, form, gender and whether it is an egg
u32 func_0200d6fc(GdsProfile *profile);
// pokebw2's GdsProfile_GetForm
u32 func_0200d70c(GdsProfile *profile);
// pokebw2's GdsProfile_GetRegion
u32 func_0200d760(GdsProfile *profile);
// pokebw2's GdsProfile_GetIntroduction
// The player's introduction: a new string if it is free text, or NULL with the sentence written to pms
StrBuf *func_0200d774(GdsProfile *profile, PMSData *pms, u32 heapId);

// Black 2's, not checked against Black
// The length of an introduction in free text

#endif // BWPORT_SAVE_GDS_PROFILE_H
