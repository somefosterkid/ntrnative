// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/trainer_game_info_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/trainer_game_info.h"
#include "app/name_entry.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
#include "field/survey.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "text/script/castelia_city_13.h"

// The save block of the trainer's game info: the money, the badges, the favorite Pokémon, the greeting and thanks,
// the survey's answers, the Gym parties and the table of the gifts given once. The ROM has no string for the file,
// so the name is a guess. Block 0x34.

#define SAVE_BLOCK_TRAINER_GAME_INFO 0x34

void func_0200c10c(TrainerGameInfoSave *info);

// pokebw2's TrainerGameInfo_GetSize
u32 func_0200c054(void) {
    return sizeof(TrainerGameInfoSave);
}

void func_0200c058(TrainerGameInfoSave *info) {
    MsgData *msgData;
    StrBuf *str;

    MI_CpuFill8(info, 0, sizeof(TrainerGameInfoSave));
    func_0200c10c(info);
    info->palParkHighScore = 0;

    // The save data heap is 7 here; its ID isn't declared
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_CASTELIA_CITY_13, HEAPID_TAIL(7));
    str = GFL_MsgDataLoadStrbufNew(msgData, CasteliaCity13_Text_337);
    GFL_StrBufStoreString(str, info->greeting, TRAINER_GAME_INFO_MESSAGE_LENGTH);
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(msgData, CasteliaCity13_Text_345);
    GFL_StrBufStoreString(str, info->thanks, TRAINER_GAME_INFO_MESSAGE_LENGTH);
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);

    info->surveyRank = 0;
    info->surveyQuestion = 0;
    info->surveyAnswers[0] = 0xff;
    info->surveyAnswers[1] = 0xff;
    info->surveyAnswers[2] = 0xff;
}


TrainerCardSave *getTrainerCardData_wrapper(SaveControl *save) {
    return getTrainerGameInfoAddress(save);
}

TrainerGameInfoSave *getTrainerGameInfoAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_GAME_INFO);
}

void func_0200c0fc(TrainerGameInfoSave *info, u32 index, u8 value) {
    info->keyboardTypes[index] = value;
}

u8 func_0200c104(TrainerGameInfoSave *info, u32 index) {
    return info->keyboardTypes[index];
}

// pokebw2's TrainerGameInfo_ClearKeyboardTypes
void func_0200c10c(TrainerGameInfoSave *info) {
    MI_CpuFill8(info->keyboardTypes, 0, 8);
}

void SetFavePokemonData(TrainerGameInfoSave *info, u32 species, u32 form, u32 a3, int a4) {
    info->faveSpecies = species;
    info->faveForm = form;
    info->unk0a_6 = a3;
    info->unk0a_7 = a4 == 1;
}

void func_0200c164(const TrainerGameInfoSave *info, int *species, int *form, int *a3, int *a4) {
    *species = info->faveSpecies;
    *form = info->faveForm;
    *a3 = info->unk0a_6;
    *a4 = info->unk0a_7;
}


u32 func_0200c184(TrainerCardSave *trainerCard) {
    return trainerCard->palParkHighScore;
}

void func_0200c18c(TrainerCardSave *trainerCard, u32 highScore) {
    trainerCard->palParkHighScore = highScore;
}

u8 func_0200c1a4(TrainerCardSave *trainerCard) {
    return trainerCard->palParkResult;
}

void func_0200c1b0(TrainerCardSave *trainerCard, u32 result) {
    trainerCard->palParkResult = result;
}





u32 func_0200c1c4(TrainerCardSave *trainerCard) {
    return trainerCard->unk18;
}

u32 func_0200c1c8(TrainerCardSave *trainerCard) {
    if (trainerCard->unk18 < TRAINER_GAME_INFO_COUNT_MAX) {
        trainerCard->unk18++;
    }
    return trainerCard->unk18;
}


u32 func_0200c1dc(TrainerCardSave *trainerCard) {
    return trainerCard->unk1c;
}



u32 func_0200c1e0(TrainerGameInfoSave *info) {
    if (info->unk1c < TRAINER_GAME_INFO_COUNT_MAX) {
        info->unk1c++;
    }
    return info->unk1c;
}

const u16 *func_0200c1f4(TrainerGameInfoSave *info) {
    return info->greeting;
}

void func_0200c1f8(TrainerGameInfoSave *info, StrBuf *strBuf) {
    GFL_StrBufStoreString(strBuf, info->greeting, 9);
}


const u16 *func_0200c20c(TrainerGameInfoSave *info) {
    return info->thanks;
}

void func_0200c210(TrainerGameInfoSave *info, StrBuf *strBuf) {
    GFL_StrBufStoreString(strBuf, info->thanks, 9);
}

u8 func_0200c224(TrainerCardSave *trainerCard) {
    return trainerCard->surveyRank;
}

void func_0200c22c(TrainerCardSave *trainerCard, int rank) {
    trainerCard->surveyRank = rank;
}






BOOL isBadgeObtained(TrainerCardSave *trainerCard, u32 badgeId) {
    if (trainerCard->badges & (1 << badgeId)) {
        return TRUE;
    }
    return FALSE;
}

void addBadge(TrainerCardSave *trainerCard, u32 badgeId) {
    trainerCard->badges |= 1 << badgeId;
}

u32 getBadgeCount(TrainerGameInfoSave *info) {
    u32 count = 0;
    u32 badges;

    for (badges = info->badges; badges != 0; badges >>= 1) {
        if (badges & 1) {
            count++;
        }
    }
    return count;
}

u32 getCash(TrainerGameInfoSave *info) {
    return info->cash;
}

u32 addCashToTotal(TrainerGameInfoSave *info, u32 amount) {
    if (amount > TRAINER_GAME_INFO_CASH_MAX) {
        info->cash = TRAINER_GAME_INFO_CASH_MAX;
    } else {
        info->cash += amount;
    }
    if (info->cash > TRAINER_GAME_INFO_CASH_MAX) {
        info->cash = TRAINER_GAME_INFO_CASH_MAX;
    }
    return info->cash;
}

u32 subCashFromTotal(TrainerGameInfoSave *info, u32 amount) {
    if (info->cash < amount) {
        info->cash = 0;
    } else {
        info->cash -= amount;
    }
    return info->cash;
}

void SetGymDefeatPartyData(TrainerGameInfoSave *info, u32 gym, const u16 *species) {
    int i;

    for (i = 0; i < 6; i++) {
        info->gymParties[gym][i] = species[i];
    }
}

void GetGymDefeatPartyData(TrainerGameInfoSave *info, u32 gym, u16 *species) {
    int i;

    for (i = 0; i < 6; i++) {
        species[i] = info->gymParties[gym][i];
    }
}

void func_0200c2f0(TrainerGameInfoSave *info, u32 item, u32 bits) {
    info->startMenuFlags |= bits << (item * 2);
}

u8 func_0200c308(TrainerGameInfoSave *info, u32 item) {
    return (info->startMenuFlags >> (item * 2)) & 3;
}

u32 func_0200c31c(TrainerGameInfoSave *info) {
    return info->startMenuFlags;
}

void func_0200c324(TrainerGameInfoSave *info, u32 value) {
    info->startMenuFlags = value;
}

u16 func_0200c32c(TrainerGameInfoSave *info) {
    return info->surveyQuestion;
}

void func_0200c334(TrainerGameInfoSave *info, u8 question) {
    info->surveyQuestion = question;
}

u8 func_0200c33c(TrainerGameInfoSave *info, u8 index) {
    return info->surveyAnswers[index];
}

void func_0200c344(TrainerGameInfoSave *info, u8 index, u8 answer) {
    info->surveyAnswers[index] = answer;
}

u16 func_0200c34c(TrainerGameInfoSave *info, u8 index) {
    return info->surveyCounts[index];
}

void func_0200c358(TrainerGameInfoSave *info, u8 index, u16 count) {
    info->surveyCounts[index] = count;
}

// Adds to the count of the answer, up to 0xffff
void func_0200c364(TrainerGameInfoSave *info, u8 index, int count) {
    u32 sum = info->surveyCounts[index];

    sum += count;
    if (sum > 0xffff) {
        sum = 0xffff;
    }
    info->surveyCounts[index] = sum;
}

s64 getSecondsFromTrainerCardData(TrainerGameInfoSave *info) {
    return info->surveySeconds;
}

void setSecondsCurrentTimeInTrainerCard(TrainerCardSave *trainerCard, s64 seconds) {
    trainerCard->surveySeconds = seconds;
}

void setToTrainerCardDRTable(TrainerCardSave *trainerCard, int flag, u32 key) {
    if (flag < TRAINER_GAME_INFO_DR_COUNT) {
        trainerCard->drTable[flag] = key;
    }
}

u32 getOneShotDRKeyFromSave(TrainerCardSave *trainerCard, int flag) {
    if (flag >= TRAINER_GAME_INFO_DR_COUNT) {
        return 0;
    }
    return trainerCard->drTable[flag];
}

u16 func_0200c3b0(TrainerGameInfoSave *info) {
    return info->unke4;
}

void func_0200c3b8(TrainerGameInfoSave *info, u16 value) {
    info->unke4 = value;
}

void func_0200c3c0(TrainerGameInfoSave *info, u16 minutes) {
    if (minutes <= info->unke4) {
        info->unke4 -= minutes;
    } else {
        info->unke4 = 0;
    }
}

u8 func_0200c3e0(TrainerCardSave *trainerCard) {
    return trainerCard->battleRecorderColor;
}

void func_0200c3ec(TrainerCardSave *trainerCard, u8 color) {
    trainerCard->battleRecorderColor = color;
}

u32 func_0200c3f4(void) {
    return 0x1a;
}

void func_0200c3f8(void *data) {
    MI_CpuFill8(data, 0x19, 0x1a);
}
