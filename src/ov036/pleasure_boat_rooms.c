// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov089/pleasure_boat_rooms.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/pleasure_boat.h"
#include "field/pleasure_boat_rooms.h"
#include "gfl/random.h"
#include "nitro/rtc.h"
#include "gfl/rtc_cache.h"

// Who is in the Royal Unova's rooms. The ROM has no string for the file; the name is a guess from what it does. The
// cruise has 15 rooms. Trainers fill as many as the day of the week allows, two hold a pair of people and the rest hold
// one person each. First the order of the rooms is drawn, with one of the six last rooms and one that suits its side
// first, then the people are drawn for the rooms in that order

#define TRAINER_TABLE_SIZE 10
#define PAIR_TABLE_SIZE 5
#define PERSON_TABLE_SIZE 15

// The kinds of people in a room, PleasureBoatRoom's kind
enum {
    ROOM_TRAINER,
    ROOM_PERSON,
    ROOM_PAIR,
};

// The rooms that are left to place and the order they were placed in, with the draws for each kind of room
typedef struct {
    // The rooms not placed yet. The last one at poolLast takes the place of a room that is placed
    u8 pool[PLEASURE_BOAT_ROOM_COUNT];
    u8 order[PLEASURE_BOAT_ROOM_COUNT];
    u8 orderCount;
    u8 poolLast;
    u8 trainers[7];
    u8 pairs[2];
    u8 people[10];
} RoomPicker;

// A Trainer who can be aboard
typedef struct {
    u32 objCode;
    u32 unk4;
    u32 values[2];
} TrainerEntry;

// A pair of people or a person who can be aboard
typedef struct {
    u32 objCode;
    u32 values[2];
} PersonEntry;

// Which side a room is on and something else about it (the counts of PleasureBoat_GetInfo)
typedef struct {
    u32 isPort;
    u32 unk4;
} RoomSide;

static void func_ov036_021f38f4(RoomPicker *picker);
static void func_ov036_021f38ac(int count, int picks, u8 *scratch, u8 *picked);
static int func_ov036_021f3934(const RoomPicker *picker, u32 room);
static int func_ov036_021f394c(const RoomPicker *picker, int index);
BOOL func_ov036_021f395c(RoomPicker *picker, int index);
BOOL func_ov036_021f3980(RoomPicker *picker);

// How many Trainers are aboard by weekday, Monday first
static const u8 sTrainerCounts[7] = { 4, 3, 4, 5, 4, 6, 7 };

// pokebw2's sFirstRooms
static const u8 data_ov036_021f3a7c[6] = { 9, 10, 11, 12, 13, 14 };

// pokebw2's sStarboardRooms
static const u8 data_ov036_021f3a82[7] = { 4, 5, 6, 7, 8, 13, 14 };

// pokebw2's sPortRooms
static const u8 data_ov036_021f3a90[8] = {
#include "asset/pleasure_boat_rooms_data_ov036_021f3a90.inc"
};

// pokebw2's sPairs
static const PersonEntry data_ov036_021f3a98[PAIR_TABLE_SIZE] = {
#include "asset/pleasure_boat_rooms_data_ov036_021f3a98.inc"
};

// pokebw2's sRoomSides
static const RoomSide data_ov036_021f3ad4[PLEASURE_BOAT_ROOM_COUNT] = {
#include "asset/pleasure_boat_rooms_data_ov036_021f3ad4.inc"
};

// pokebw2's sTrainers
static const TrainerEntry data_ov036_021f3b4c[TRAINER_TABLE_SIZE] = {
#include "asset/pleasure_boat_rooms_data_ov036_021f3b4c.inc"
};

// pokebw2's sPeople
static const PersonEntry data_ov036_021f3bec[PERSON_TABLE_SIZE] = {
#include "asset/pleasure_boat_rooms_data_ov036_021f3bec.inc"
};

// pokebw2's PleasureBoat_SetUpRooms
void func_ov036_021f36c0(PleasureBoat *boat) {
    RTCDate date;
    RoomPicker picker;
    u8 trainerScratch[TRAINER_TABLE_SIZE];
    u8 peopleScratch[PERSON_TABLE_SIZE];
    u8 pairScratch[PAIR_TABLE_SIZE];
    int trainerCount;
    int slot;
    u8 i;
    u8 j;

    RTC_GetCachedDate(&date);
    switch (date.week) {
    case RTC_WEEK_MONDAY:
        trainerCount = sTrainerCounts[0];
        break;
    case RTC_WEEK_TUESDAY:
        trainerCount = sTrainerCounts[1];
        break;
    case RTC_WEEK_WEDNESDAY:
        trainerCount = sTrainerCounts[2];
        break;
    case RTC_WEEK_THURSDAY:
        trainerCount = sTrainerCounts[3];
        break;
    case RTC_WEEK_FRIDAY:
        trainerCount = sTrainerCounts[4];
        break;
    case RTC_WEEK_SATURDAY:
        trainerCount = sTrainerCounts[5];
        break;
    case RTC_WEEK_SUNDAY:
        trainerCount = sTrainerCounts[6];
        break;
    default:
        trainerCount = 0;
        break;
    }
    boat->trainerCount = trainerCount;

    func_ov036_021f38f4(&picker);
    if (!func_ov036_021f3980(&picker)) {
        func_ov036_021f38f4(&picker);
    }
    func_ov036_021f38ac(NELEMS(data_ov036_021f3b4c), trainerCount, trainerScratch, picker.trainers);
    func_ov036_021f38ac(NELEMS(data_ov036_021f3a98), 2, pairScratch, picker.pairs);
    func_ov036_021f38ac(NELEMS(data_ov036_021f3bec), NELEMS(data_ov036_021f3bec) - (trainerCount + 2), peopleScratch, picker.people);

    for (i = 0; i < trainerCount; i++) {
        const RoomSide *side = &data_ov036_021f3ad4[picker.order[i]];

        if (side->isPort == 0) {
            boat->infoCounts[0]++;
        } else {
            boat->infoCounts[1]++;
        }
        if (side->unk4 == 0) {
            boat->infoCounts[2]++;
        } else {
            boat->infoCounts[3]++;
        }
    }

    slot = 0;
    for (i = 0; i < trainerCount; i++) {
        u32 room = picker.order[slot];
        u32 trainer = picker.trainers[i];

        boat->rooms[room].index = trainer;
        boat->rooms[room].unk4 = data_ov036_021f3b4c[trainer].unk4;
        boat->rooms[room].objCode = data_ov036_021f3b4c[trainer].objCode;
        boat->rooms[room].kind = ROOM_TRAINER;
        slot++;
        boat->rooms[room].values[0] = data_ov036_021f3b4c[trainer].values[0];
        boat->rooms[room].values[1] = data_ov036_021f3b4c[trainer].values[1];
    }
    for (i = 0; i < 2; i++) {
        u32 room = picker.order[slot];
        u32 person = picker.pairs[i];

        boat->rooms[room].index = i;
        boat->rooms[room].unk4 = 0;
        boat->rooms[room].objCode = data_ov036_021f3a98[person].objCode;
        boat->rooms[room].kind = ROOM_PAIR;
        slot++;
        boat->rooms[room].values[0] = data_ov036_021f3a98[person].values[0];
        boat->rooms[room].values[1] = data_ov036_021f3a98[person].values[1];
    }
    for (j = 0; j < PLEASURE_BOAT_ROOM_COUNT - (trainerCount + 2); j++) {
        u32 room = picker.order[slot];
        u32 person = picker.people[j];

        boat->rooms[room].index = j;
        boat->rooms[room].unk4 = 0;
        boat->rooms[room].objCode = data_ov036_021f3bec[person].objCode;
        boat->rooms[room].kind = ROOM_PERSON;
        slot++;
        boat->rooms[room].values[0] = data_ov036_021f3bec[person].values[0];
        boat->rooms[room].values[1] = data_ov036_021f3bec[person].values[1];
    }
}

// Picks picks of the numbers below count, none twice
// pokebw2's PickRandom
static void func_ov036_021f38ac(int count, int picks, u8 *scratch, u8 *picked) {
    u8 i;
    u8 n;

    for (i = 0; i < count; i++) {
        scratch[i] = i;
    }
    for (n = 0; n < picks; n++) {
        u8 index = GFL_RandomLC(count);

        picked[n] = scratch[index];
        scratch[index] = scratch[count - 1];
        count--;
    }
}

// pokebw2's RoomPicker_Reset
static void func_ov036_021f38f4(RoomPicker *picker) {
    int i;

    picker->poolLast = 14;
    picker->orderCount = 0;
    for (i = 0; i < PLEASURE_BOAT_ROOM_COUNT; i++) {
        picker->pool[i] = i;
        picker->order[i] = 0;
    }
    for (i = 0; i < 7; i++) {
        picker->trainers[i] = i;
    }
    for (i = 0; i < 2; i++) {
        picker->pairs[i] = i;
    }
    for (i = 0; i < 10; i++) {
        picker->people[i] = i;
    }
}

// pokebw2's RoomPicker_Find
static int func_ov036_021f3934(const RoomPicker *picker, u32 room) {
    int i;

    for (i = 0; i < PLEASURE_BOAT_ROOM_COUNT; i++) {
        if (room == picker->pool[i]) {
            return i;
        }
    }
    return -1;
}

// pokebw2's RoomPicker_Get
static int func_ov036_021f394c(const RoomPicker *picker, int index) {
    if (index > picker->poolLast) {
        return -1;
    }
    return picker->pool[index];
}

// pokebw2's RoomPicker_Remove
BOOL func_ov036_021f395c(RoomPicker *picker, int index) {
    if (index > picker->poolLast) {
        return FALSE;
    }
    if (index < 0) {
        return FALSE;
    }
    picker->pool[index] = picker->pool[picker->poolLast];
    if (picker->poolLast != 0) {
        picker->poolLast--;
    }
    return TRUE;
}

// pokebw2's RoomPicker_PickOrder
BOOL func_ov036_021f3980(RoomPicker *picker) {
    BOOL starboard = FALSE;
    BOOL port = FALSE;
    u32 roll;
    u8 room;
    u8 second;
    int left = PLEASURE_BOAT_ROOM_COUNT;
    int i;

    roll = GFL_RandomLC(6);
    room = data_ov036_021f3a7c[roll];
    if (!func_ov036_021f395c(picker, func_ov036_021f3934(picker, room))) {
        return FALSE;
    }
    picker->order[picker->orderCount] = room;
    picker->orderCount++;
    left--;
    if (data_ov036_021f3ad4[roll].isPort == 0) {
        starboard = TRUE;
    } else {
        port = TRUE;
    }
    if (!port || !starboard) {
        if (port == 0) {
            second = data_ov036_021f3a82[GFL_RandomLC(7)];
        } else {
            second = data_ov036_021f3a90[GFL_RandomLC(8)];
        }
        if (!func_ov036_021f395c(picker, func_ov036_021f3934(picker, second))) {
            return FALSE;
        }
        picker->order[picker->orderCount] = second;
        picker->orderCount++;
        left--;
    }
    for (i = 0; i < left; i++) {
        int index = GFL_RandomLC(left - i);
        int value = func_ov036_021f394c(picker, index);

        if (value < 0) {
            return FALSE;
        }
        if (!func_ov036_021f395c(picker, index)) {
            return FALSE;
        }
        picker->order[picker->orderCount] = value;
        picker->orderCount++;
    }
    return TRUE;
}
