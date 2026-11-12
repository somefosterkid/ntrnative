// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_gimmick_gate.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_FIELD_GIMMICK_GATE_H
#define BWPORT_FIELD_FIELD_GIMMICK_GATE_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of unk00, flagId, unk08 and type in Black
// A news entry in archive 0xa4, shown when its flag is set and the gate's zone is one of zones
struct GimmickGateBoardEntry {
    u32 unk00;
    u32 flagId;
    u32 unk08;
    u32 unk0c;
    u32 type;
    u32 zones[4];
};

u32 func_ov048_021f3790(Field *field);

// Black 2's; twinned code confirms the offsets of x, y, z, direction, anmIndex and enabled in Black
// A gate's entry in archive 0xac, 0x7c bytes
struct GimmickGateZoneData {
    u32 zoneId;
    u32 version;
    u32 x;
    u32 y;
    u32 z;
    u32 direction;
    u32 messageIds[19];
    u32 fallbackZones[4];
    u32 anmIndex;
    u32 enabled;
};

void func_ov048_021f374c(Field *field);
void func_ov048_021f376c(Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov048_021f36c0(Field *field);

#endif // BWPORT_FIELD_FIELD_GIMMICK_GATE_H
