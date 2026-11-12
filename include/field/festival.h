// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/festival.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_FIELD_FESTIVAL_H
#define BWPORT_FIELD_FESTIVAL_H

#include "types.h"

#include "struct_decls.h"
// Black 2's, not checked against Black
// A Funfest mission's config, as GetFestMissionCfg returns it. A mission's beacons send its first 0x14 bytes
struct FestMissionConfig {
    u32 unk00[5];
    // The kind of mission, which picks the overlay that runs it on the field
    u8 type;
    u8 unk15[3];
    u16 missionId;
    u16 unk1A;
    u32 unk1C;
    u32 unk20;
    s32 unk24;
    s32 unk28;
};

// Black 2's, not checked against Black
struct FestMissionEntry {
    FestMissionConfig config;
    u16 trainerId;
    u16 zoneId;
    u32 missionId;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 GetTrainerCardTextMSGID(u32 type);

#endif // BWPORT_FIELD_FESTIVAL_H
