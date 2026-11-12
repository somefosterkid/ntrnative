// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/gimmick_roller_coaster.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_GIMMICK_ROLLER_COASTER_H
#define BWPORT_FIELD_GIMMICK_ROLLER_COASTER_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's RollerCoaster_End
void func_ov039_021f3cd8(Field *field);
// pokebw2's RollerCoaster_StartAmbience
void func_ov039_021f3d74(GameSystem *gsys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's RollerCoaster_Init
void func_ov039_021f36fc(Field *field);
// pokebw2's RollerCoaster_Update
void func_ov039_021f3d1c(Field *field);
// pokebw2's RollerCoaster_CreateSwitchEvent
// Presses one of the four switches, which changes its car's path. NULL for a switch out of range
GameEvent *func_ov039_021f3e24(GameSystem *gsys, u8 switchId);
// pokebw2's RollerCoaster_CheckRide
GameEvent *func_ov039_021f404c(GameSystem *gsys);
// pokebw2's RollerCoaster_SetCarIntroSeen
// Marks the first ride of car 1 or 2 as shown, so that later rides skip its scripts
void func_ov039_021f4134(GameSystem *gsys, u16 car);

#endif // BWPORT_FIELD_GIMMICK_ROLLER_COASTER_H
