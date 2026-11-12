// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/building_enter_effect.h.
#ifndef BWPORT_FIELD_BUILDING_ENTER_EFFECT_H
#define BWPORT_FIELD_BUILDING_ENTER_EFFECT_H

// Overlay 10's building_enter_effect.c: the zoom into a building as the player enters it

#include "types.h"

// Starts the zoom, which runs each VBlank
void setBuildingEnterVBlankCallback(void);
// Ends it and resets BG 2's matrix
void func_ov010_02166b2c(void);

#endif // BWPORT_FIELD_BUILDING_ENTER_EFFECT_H
