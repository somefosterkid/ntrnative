// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/musical_savedata.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_MUSICAL_SAVEDATA_H
#define BWPORT_SAVE_MUSICAL_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"
#include "field/city_state.h"

#define MUSICAL_PROP_MAX 100
#define MUSICAL_PROP_BYTES (MUSICAL_PROP_MAX / 8 + 20)

// Black 2's; twinned code confirms the offsets of itemId, unk2 and unk4 in Black
// A prop a Pokémon wears in a musical shot, by the slot it is worn on
typedef struct {
    u16 itemId;
    s16 unk2;
    u8 unk4;
    u8 unk5[3];
} MusicalShotEquip;

// Black 2's; twinned code confirms the offsets of species, sex, rare, form, personality and equips in Black
typedef struct {
    u16 species;
    u16 sex : 2;
    u16 rare : 1;
    u16 form : 5;
    u32 personality;
    u16 name[8];
    MusicalShotEquip equips[8];
} MusicalShotPoke;

// Black 2's; twinned code confirms the offsets of unk0_0, tops, month and pokes in Black
// The photo of a musical's finale, which the musical event fills in and the save keeps. A month of 0 means no photo
struct MusicalShot {
    u32 unk0_0 : 5;
    // A bit for each Pokémon that got the most points
    u32 tops : 4;
    u32 year : 7;
    u32 month : 5;
    u32 day : 6;
    // The player's Pokémon
    u32 player : 2;
    u32 unk0_29 : 3;
    MusicalShotPoke pokes[4];
    u16 title[0x25];
    u8 unk1AE;
    u8 unk1AF;
};

// Black 2's; twinned code confirms the offsets of unk0 and unk1 in Black
typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
} MusicalSaveUnk1E0;

// Black 2's; twinned code confirms the offsets of unk0, itemId and rotation in Black
// A prop that the Pokémon wears: its position (10 for none), the prop and its rotation
typedef struct {
    u8 unk0;
    u8 unk1;
    u16 itemId;
    s16 rotation;
} MusicalSaveUnk1B0;

// Black 2's; matching code confirms the offsets of shot, unk1B0, unk1E0, name, unk252, unk254, unk256, props, newProps,
// unk298, unk29C, unk29D, unk29E and dressingRoomVisited, and its size in Black
struct MusicalSave {
    MusicalShot shot;
    MusicalSaveUnk1B0 unk1B0[8];
    MusicalSaveUnk1E0 unk1E0[10];
    // A name of 0x25 characters
    u16 name[0x25];
    u16 unk252;
    u16 unk254;
    u16 unk256;
    // A bit per prop owned, and per prop not yet looked at
    u8 props[MUSICAL_PROP_BYTES];
    u8 newProps[MUSICAL_PROP_BYTES];
    u8 unk298[4];
    u8 unk29C;
    u8 unk29D;
    u8 unk29E;
    // Whether the dressing room has been visited
    u8 dressingRoomVisited;
    // Whether the downloaded props were added
    u8 downloadedProps;
    u8 padding2A1[3];
}; // size: 0x2a4

// The size of the block and its init
u32 func_0200a53c(void);
void func_0200a544(MusicalSave *musical);
void func_0200a604(MusicalSave *musical);
MusicalSaveUnk1B0 *func_0200a628(MusicalSave *musical);
// Whether a musical photo is saved
BOOL func_0200a630(MusicalSave *musical);
// Whether a prop is owned, whether it is flagged as new, and giving one
BOOL func_0200a644(MusicalSave *musical, u8 prop);
BOOL func_0200a680(MusicalSave *musical, u8 prop);
void func_0200a6bc(MusicalSave *musical, u8 prop);
// Forgets which props are new
void func_0200a6f8(MusicalSave *musical);
// How many props are owned, and whether all of them are
u32 func_0200a714(MusicalSave *musical);
BOOL func_0200a73c(MusicalSave *musical);
MusicalSaveUnk1E0 *func_0200a750(MusicalSave *musical, u8 index);
// The counters at 0x252 and 0x254, and adding one to them, up to 0xffff
u16 func_0200a75c(MusicalSave *musical);
void func_0200a768(MusicalSave *musical);
u16 func_0200a780(MusicalSave *musical);
void func_0200a788(MusicalSave *musical);
u8 func_0200a7a0(MusicalSave *musical, u8 index);
void func_0200a7ac(MusicalSave *musical, u8 index, u8 value);
u8 func_0200a7b8(MusicalSave *musical);
void func_0200a7c0(MusicalSave *musical, u8 value);
u8 func_0200a7c8(MusicalSave *musical);
void func_0200a7d4(MusicalSave *musical, u8 value);
u8 func_0200a7e0(MusicalSave *musical);
void func_0200a7ec(MusicalSave *musical, u8 value);
const u16 *func_0200a7f8(MusicalSave *musical);
// The counter at 0x256, and adding to it, up to 0xffff
void func_0200a800(MusicalSave *musical, u16 value);
u16 func_0200a81c(MusicalSave *musical);
BOOL func_0200a828(MusicalSave *musical);
void func_0200a834(MusicalSave *musical, BOOL value);

// No twin in pokebw2's C. Black's callees here are overlay 10's city state functions, so these are not musical data:
// the size of save block 0x2b, the reset of that block's city state, and the block's pointer
u32 func_0200a840(void);
// The city state, then the ids of the residents and the first resident's info, which the reset fills in
typedef struct {
    CityState state;
    u16 unk_1e8[CITY_RESIDENT_MAX];
    CityResidentInfo unk_1fc;
} CityStateBlock;
void func_0200a848(CityStateBlock *block);
void *func_0200a864(SaveControl *save);

#endif // BWPORT_SAVE_MUSICAL_SAVEDATA_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/musical_savedata.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_MUSICAL_SAVEDATA_H

// Forgets which props are new
// Whether a prop is owned, whether it is flagged as new, and giving one

#endif // BWPORT_SAVE_MUSICAL_SAVEDATA_H
