// After pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/pleasure_boat.h, the lines in use so
// far, with Black's names: pokebw2's are its own.
#ifndef BWPORT_FIELD_PLEASURE_BOAT_H
#define BWPORT_FIELD_PLEASURE_BOAT_H

#include "types.h"
#include "struct_decls.h"

// The Royal Unova's cruise (pleasure_boat.c, overlay 21 in Black): its OS_GetTick, and the people and Trainers aboard

// Sets up a cruise, with overlay 36. Black 2's takes a flag
PleasureBoat *func_ov021_021c8608(void);
// Frees the cruise and clears the pointer
void func_ov010_021666a4(PleasureBoat **boat);
u32 func_ov021_021c8738(PleasureBoat *boat, u32 info);
// Moves the OS_GetTick on by steps * 30, stopping just short of the end of a period when stopBefore is set
void func_ov021_021c86e8(PleasureBoat *boat, u16 steps, BOOL stopBefore);
void func_ov021_021c87d4(PleasureBoat *boat, u32 trainer, BOOL defeated);
void func_ov021_021c8810(PleasureBoat *boat);

#define PLEASURE_BOAT_ROOM_COUNT 15

// Black 2's; matching code confirms the offsets of index, unk4, objCode, kind and values in Black
typedef struct {
    // Which of the kind's people the room has
    u32 index;
    u32 unk4;
    // The object code of the person
    u32 objCode;
    // 0 for a Trainer, 1 for a person and 2 for a pair of them
    u32 kind;
    u32 values[2];
    // Whether the Trainer was defeated
    u32 defeated;
} PleasureBoatRoom;

// Black 2's; matching code confirms the offsets of OS_GetTick, trainerCount, period, rooms and infoCounts in Black
struct PleasureBoat {
    u32 unk0;
    // The OS_GetTick, which counts periods of 1350
    s16 OS_GetTick;
    // How many Trainers are aboard, by the day of the week
    u16 trainerCount;
    // The last period reached
    s32 period;
    PleasureBoatRoom rooms[PLEASURE_BOAT_ROOM_COUNT];
    // The answers to PLEASURE_BOAT_INFO_STARBOARD_PEOPLE to PLEASURE_BOAT_INFO_UNK_3
    u8 infoCounts[4];
    u32 unk1B4;
};

BOOL s01A2_CallRoyalUnovaView(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_02166734(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 func_ov021_021c87a0(PleasureBoat *boat, u32 room);
u32 func_ov021_021c87ac(PleasureBoat *boat, u32 room);
u32 func_ov021_021c87b8(PleasureBoat *boat, u32 room, u32 index);
BOOL func_ov021_021c87c8(PleasureBoat *boat, u32 room);
u32 func_ov021_021c87e0(PleasureBoat *boat);
GameEvent *func_ov021_021c868c(GameSystem *gsys, PleasureBoat *boat);

#endif // BWPORT_FIELD_PLEASURE_BOAT_H
