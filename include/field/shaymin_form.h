// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/shaymin_form.h, with Black's names.
#ifndef BWPORT_FIELD_SHAYMIN_FORM_H
#define BWPORT_FIELD_SHAYMIN_FORM_H

// Overlay 10's shaymin_form.c (a descriptive name): Shaymin's Sky Forme turns back into its Land Forme at night

#include "types.h"
#include "nitro/rtc.h"
#include "struct_decls.h"

// Whether the night came within the minutes passed up to the time, turning the party's Shaymin back if so
BOOL func_ov010_0216a2ec(GameData *gameData, PokeParty *party, s32 minutes, const RTCTime *time, u32 season);
// Whether it is night at the time, turning the party's Shaymin back and registering the first in the Pokédex if so
BOOL func_ov010_0216a358(GameData *gameData, PokeParty *party, const RTCTime *time, u8 season);
void func_ov010_0216a390(GameData *gameData, PokeParty *party);

#endif // BWPORT_FIELD_SHAYMIN_FORM_H
