// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_ochiba.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SCRCMD_OCHIBA_H
#define BWPORT_FIELD_SCRCMD_OCHIBA_H

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// Where the fallen leaves effect plays
typedef struct {
    Field *field;
    FieldPlayer *player;
} OchibaEffectArgs;

// Black 2's, not checked against Black
typedef struct {
    struct FieldG3DObjSystem *sys;
    u16 resGroup;
    u16 obj;
} OchibaEffectWork;

// Black 2's, not checked against Black
// A Pokémon of a Habitat List page, and where it appears in each part of the day
typedef struct {
    u16 species;
    u8 encounters[4][3];
    u8 unk0E[14];
} HabitatListEntry;

// Black 2's, not checked against Black
// A Habitat List page, a file of archive 0x128
typedef struct {
    u8 unk00[8];
    u16 count;
    HabitatListEntry entries[30];
} HabitatList;

// Black 2's, not checked against Black
// The page of each zone
typedef struct {
    u16 unk0;
    u16 zoneId;
    u8 fileId;
    u8 unk5;
} HabitatListHeader;

#endif // BWPORT_FIELD_SCRCMD_OCHIBA_H
