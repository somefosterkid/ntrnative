// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/trcard_sys.h: the lines in use so far.
#ifndef BWPORT_FIELD_TRCARD_SYS_H
#define BWPORT_FIELD_TRCARD_SYS_H

#include "types.h"
#include "system/pms_data.h"
#include "struct_decls.h"

// Black 2's up to unk674, where Black's ends (size 0x678); matching code confirms the offsets of version, stars, gender,
// unk04_2, unk04_4, unk04_5, unk04_7, unk06 and unk74 in Black
typedef struct {
    u8 version;
    u8 unk01;
    u8 unk02;
    // Up to five, from func_ov012_02169b78
    u8 stars;
    // Set when the card is the player's own, whose play time keeps running
    u8 isOwn : 1;
    u8 gender : 1;
    u8 unk04_2 : 1;
    u8 unk04_3 : 1;
    u8 unk04_4 : 1;
    u8 unk04_5 : 1;
    u8 canEdit : 1;
    u8 unk04_7 : 1;
    u8 unk05;
    u8 unk06;
    u8 unk07;
    // One bit for each badge
    u16 badges;
    u16 name[8];
    // The player's play time, while isOwn is set
    PlayTime *playTime;
    u32 money;
    u32 seenCount;
    u16 trainerId;
    u16 playHours;
    u16 clearHour;
    u8 playMinutes;
    // The date the adventure started on, and the date and time of the first Hall of Fame entry
    u8 startYear;
    u8 startMonth;
    u8 startDay;
    u8 clearYear;
    u8 clearMonth;
    u8 clearDay;
    u8 clearMinute;
    u8 unk36[2];
    // unk38 to unk44, unk4C to unk54, unk5C and unk60 are sums of records, from the rows of func_ov012_02169738's
    // table
    u32 unk38;
    u32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u16 passPowerCount;
    u16 unk60;
    u16 palParkHighScore;
    u16 unk64;
    // The Battle Test rank
    u16 battleTestRank;
    u32 unk68_0 : 1;
    u32 unk68_1 : 1;
    u32 unk68_2 : 1;
    u32 unk68_3 : 1;
    u32 unk68_4 : 1;
    u32 unk68_5 : 27;
    // The phrase the player greets with, which is also the C-Gear's
    PMSData greeting;
    u8 unk74[0x600];
    u32 unk674;
} TrainerCardData;

// Black 2's; matching code confirms the offsets of gameData and canEdit in Black
typedef struct {
    TrainerCardData *data;
    GameData *gameData;
    BOOL canEdit;
    // What the screen asks for next: 1 the greeting's phrase select, 2 overlay 186's other screen, 3 back to the
    // card, 4 the medals
    u32 next;
    // 1 when the medals screen asked to return to the field
    u32 result;
    s32 appParam;
} TrainerCardParam;

// Black's overlay 10: fills in a trainer card from the player's data
void func_ov010_0216f40c(TrainerCardData *card, GameData *gameData, u32 a2, u32 a3, HeapID heapId);
// Black's overlay 10: the parameters of the trainer card screen for the player's own card, which the screen fills in
TrainerCardParam *func_ov010_0216f7bc(GameData *gameData, HeapID heapId, BOOL canEdit);
// Black's overlay 10: the parameters of the trainer card screen for a card
TrainerCardParam *func_ov010_0216f7f4(GameData *gameData, TrainerCardData *card, HeapID heapId);

// The trainer card's proc, from the field menu
extern const GameProcFunctions data_ov010_02172614;

#endif // BWPORT_FIELD_TRCARD_SYS_H
