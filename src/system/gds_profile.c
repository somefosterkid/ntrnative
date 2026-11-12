// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/gds_profile.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include <stddef.h>
#include "types.h"
#include "save/gds_profile.h"
#include "constants/language.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "field/unity_tower.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_game_info.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/pms.h"
#include "system/pms_data.h"
#include "system/pms_word.h"
#include "system/str_tool.h"
#include "stddef.h"

// The profile of the player that a battle video or a musical photo sent to the Global Link carries

// The species and the forms' entries that a profile's Pokémon can be
#define GDS_PROFILE_SPECIES_END 651
#define GDS_PROFILE_SEX_MALE 0
#define GDS_PROFILE_SEX_FEMALE 1
#define GDS_PROFILE_UNION_VIEW_MAX 15
// The sentences an introduction can be: the first five types and their first twenty sentences
#define GDS_PROFILE_PMS_TYPE_END 5
#define GDS_PROFILE_PMS_ID_MAX 19
// The introduction that replaces a broken one
#define GDS_PROFILE_DEFAULT_PMS_TYPE 4
#define GDS_PROFILE_DEFAULT_PMS_WORD 29

static u8 func_0200d82c(GdsProfile *profile);

// pokebw2's GdsProfile_Create
GdsProfile *func_0200d59c(u32 heapId) {
    GdsProfile *profile = GFL_HeapAllocate(heapId, sizeof(GdsProfile), FALSE, "gds_profile.c", 80);

    MI_CpuFill8(profile, 0, sizeof(GdsProfile));
    return profile;
}

// pokebw2's GdsProfile_Free
void func_0200d5c8(GdsProfile *profile) {
    GFL_HeapFree(profile);
}

// pokebw2's GdsProfile_SetFromGameData
void func_0200d5d0(GdsProfile *profile, GameData *gameData) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(gameData);
    getUnityTower_SurveySaveBlkAddrress(save);
    TrainerGameInfoSave *gameInfo = getTrainerCardDataBlkAddress(gameData);
    void *sentences = func_0200e6e8(save);
    OSOwnerInfo ownerInfo;
    int species;
    int form;
    int egg;
    int gender;
    int i;

    OS_GetOwnerInfo(&ownerInfo);
    func_0200c164(gameInfo, &species, &form, &egg, &gender);
    MI_CpuFill8(profile, 0, sizeof(GdsProfile));
    wcharsncpy(GetPlayerName(playerInfo), profile->name, GDS_PROFILE_NAME_LENGTH);
    profile->trainerId = getIDAsUInt(playerInfo);
    profile->sex = getTrainerGender(playerInfo);
    profile->species = species;
    profile->form = form;
    profile->egg = egg;
    profile->gender = gender;
    profile->country = UnityTowerVisitor_GetCountry(playerInfo);
    profile->region = UnityTowerVisitor_GetProvince(playerInfo);
    for (i = 0; i < GDS_PROFILE_INTRODUCTION_LENGTH; i++) {
        profile->introduction.text[i] = GFL_StrBufGetTerminator();
    }
    func_0200e6fc(sentences, 0, &profile->introduction.pms);
    profile->birthMonth = ownerInfo.birthday.month;
    profile->unionView = func_02008554(playerInfo);
    profile->version = GAME_VERSION;
    profile->language = GAME_LANGUAGE;
    profile->crc = getCRC16(profile, offsetof(GdsProfile, crc));
}

// pokebw2's GdsProfile_CreateNameStrBuf
StrBuf *func_0200d6d0(GdsProfile *profile, u32 heapId) {
    StrBuf *name = GFL_StrBufCreate(15, heapId);

    GFL_StrBufLoadFixedString(name, profile->name, 15);
    return name;
}

// pokebw2's GdsProfile_GetSex
u32 func_0200d6ec(GdsProfile *profile) {
    if (profile->sex != GDS_PROFILE_SEX_MALE && profile->sex != GDS_PROFILE_SEX_FEMALE) {
        return GDS_PROFILE_SEX_MALE;
    }
    return profile->sex;
}

// pokebw2's GdsProfile_GetSpecies
u32 func_0200d6fc(GdsProfile *profile) {
    if (profile->species >= GDS_PROFILE_SPECIES_END) {
        return 0;
    }
    return profile->species;
}

// pokebw2's GdsProfile_GetForm
u32 func_0200d70c(GdsProfile *profile) {
    if (profile->species >= GDS_PROFILE_SPECIES_END) {
        return 0;
    }
    return func_02019d58(profile->species, profile->form);
}

// pokebw2's GdsProfile_GetGender
u32 func_0200d72c(GdsProfile *profile) {
    if (profile->gender != 0 && profile->gender != 1) {
        return 0;
    }
    return profile->gender;
}

// pokebw2's GdsProfile_IsEgg
BOOL func_0200d73c(GdsProfile *profile) {
    if (profile->egg > TRUE) {
        return TRUE;
    }
    return profile->egg;
}

// pokebw2's GdsProfile_GetCountry
u32 func_0200d74c(GdsProfile *profile) {
    return func_02025c1c(profile->country, profile->region, func_0200d82c(profile));
}

// pokebw2's GdsProfile_GetRegion
u32 func_0200d760(GdsProfile *profile) {
    return func_02025c30(profile->country, profile->region, func_0200d82c(profile));
}

// pokebw2's GdsProfile_GetIntroduction
StrBuf *func_0200d774(GdsProfile *profile, PMSData *pms, u32 heapId) {
    int errors = 0;

    if (!profile->introductionIsText) {
        u32 category;
        u32 index;

        *pms = profile->introduction.pms;
        if (pms->type >= GDS_PROFILE_PMS_TYPE_END) {
            errors++;
        } else if (pms->id > GDS_PROFILE_PMS_ID_MAX) {
            errors++;
        } else if ((pms->words[0] != PMS_WORD_NULL && !func_020248d8(pms->words[0], &category, &index)) ||
                   (pms->words[1] != PMS_WORD_NULL && !func_020248d8(pms->words[1], &category, &index))) {
            errors++;
        }
        if (errors > 0) {
            func_020242a4(pms, GDS_PROFILE_DEFAULT_PMS_TYPE);
            pms->id = 0;
            pms->words[0] = func_02024888(TEXT_BANK_PMS_WORDS_TRAINER, GDS_PROFILE_DEFAULT_PMS_WORD);
            pms->words[1] = PMS_WORD_NULL;
        }
        return NULL;
    } else {
        StrBuf *text = GFL_StrBufCreate(GDS_PROFILE_INTRODUCTION_LENGTH, heapId);

        GFL_StrBufLoadFixedString(text, profile->introduction.text, GDS_PROFILE_INTRODUCTION_LENGTH);
        return text;
    }
}

// pokebw2's GdsProfile_GetBirthMonth
u32 func_0200d810(GdsProfile *profile) {
    if (profile->birthMonth < 1 || profile->birthMonth > 12) {
        return 1;
    }
    return profile->birthMonth;
}

// pokebw2's GdsProfile_GetUnionView
u32 func_0200d820(GdsProfile *profile) {
    if (profile->unionView > GDS_PROFILE_UNION_VIEW_MAX) {
        return 0;
    }
    return profile->unionView;
}

// The language of the game that made the profile, or English if it isn't one
// pokebw2's getValidLanguage
static u8 func_0200d82c(GdsProfile *profile) {
    switch (profile->language) {
        case LANGUAGE_JAPANESE:
        case LANGUAGE_ENGLISH:
        case LANGUAGE_FRENCH:
        case LANGUAGE_ITALIAN:
        case LANGUAGE_GERMAN:
        case LANGUAGE_SPANISH:
        case LANGUAGE_KOREAN:
            return profile->language;
    }
    return LANGUAGE_ENGLISH;
}
