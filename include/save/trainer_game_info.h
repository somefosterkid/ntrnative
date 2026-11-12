// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/trainer_game_info.h: the lines in use
// so far.
#ifndef BWPORT_SAVE_TRAINER_GAME_INFO_H
#define BWPORT_SAVE_TRAINER_GAME_INFO_H

#include "types.h"
#include "struct_decls.h"

#define TRAINER_GAME_INFO_CASH_MAX 9999999
#define TRAINER_GAME_INFO_COUNT_MAX 99999

// Black 2's, not checked against Black
#define TRAINER_GAME_INFO_KEYBOARD_COUNT 8
#define TRAINER_GAME_INFO_MESSAGE_LENGTH 9
#define TRAINER_GAME_INFO_SURVEY_ANSWER_COUNT 3
#define TRAINER_GAME_INFO_GYM_COUNT 8
#define TRAINER_GAME_INFO_DR_COUNT 8

// Black 2's; matching code confirms the offsets of cash, badges, faveSpecies, faveForm, unk0a_6, unk0a_7,
// keyboardTypes, unk1c, startMenuFlags, surveyQuestion, surveyAnswers, surveyCounts, surveySeconds, gymParties,
// drTable, battleRecorderColor and unke4, and its size in Black
struct TrainerGameInfoSave {
    u32 cash;
    u32 badges;
    u16 faveSpecies;
    u8 faveForm : 6;
    u8 unk0a_6 : 1;
    u8 unk0a_7 : 1;
    u8 unk0b;
    // The keyboard that each mode of the name entry last used
    u8 keyboardTypes[TRAINER_GAME_INFO_KEYBOARD_COUNT];
    u32 palParkHighScore : 28;
    u32 palParkResult : 4;
    // A counter that stops at TRAINER_GAME_INFO_COUNT_MAX
    u32 unk18;
    // A counter that stops at TRAINER_GAME_INFO_COUNT_MAX
    u32 unk1c;
    u16 greeting[TRAINER_GAME_INFO_MESSAGE_LENGTH];
    u16 thanks[TRAINER_GAME_INFO_MESSAGE_LENGTH];
    u8 surveyRank;
    // Two bits for each of the start menu's items that are added later
    u8 startMenuFlags;
    u8 surveyQuestion;
    u8 surveyAnswers[TRAINER_GAME_INFO_SURVEY_ANSWER_COUNT];
    u16 surveyCounts[TRAINER_GAME_INFO_SURVEY_ANSWER_COUNT];
    // When the survey started, in seconds since 2000
    s64 surveySeconds;
    u16 gymParties[TRAINER_GAME_INFO_GYM_COUNT][6];
    u32 drTable[TRAINER_GAME_INFO_DR_COUNT];
    u8 unkdc[4];
    // The color of the Battle Recorder
    u32 battleRecorderColor;
    u16 unke4;
    u8 unke6[0xa];
};

void SetFavePokemonData(TrainerGameInfoSave *info, u32 species, u32 form, u32 a3, int a4);
// Add one to the counter at 0x1c, and return it
u32 func_0200c1e0(TrainerGameInfoSave *info);

// Add one to the counter at 0x1c, and return it

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GetFavePokemonData
void func_0200c164(const TrainerGameInfoSave *info, int *species, int *form, int *a3, int *a4);
// The species of the party that beat a Gym
void SetGymDefeatPartyData(TrainerGameInfoSave *info, u32 gym, const u16 *species);
void GetGymDefeatPartyData(TrainerGameInfoSave *info, u32 gym, u16 *species);
// pokebw2's TrainerGameInfo_GetSize
// The size of the block
u32 func_0200c054(void);
// pokebw2's TrainerGameInfo_Init
void func_0200c058(TrainerGameInfoSave *info);

#endif // BWPORT_SAVE_TRAINER_GAME_INFO_H
