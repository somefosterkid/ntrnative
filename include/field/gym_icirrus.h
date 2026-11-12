// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/gym_icirrus.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_GYM_ICIRRUS_H
#define BWPORT_FIELD_GYM_ICIRRUS_H

#include "types.h"
#include "struct_decls.h"

void func_ov045_021f36c0(Field *field);
void func_ov045_021f381c(Field *field);
// The gym's script commands
// The event that plays a switch's animation, or NULL if there are fewer switches
GameEvent *func_ov045_021f384c(GameSystem *gsys, int switchIndex);
// Switches the player between grid and rail movement if the switch is in the state that the place calls for
void func_ov045_021f38c4(GameSystem *gsys, int place);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov045_021f37f8(Field *field);
// The same event with the sound, and the switch's state flipped when it ends
GameEvent *func_ov045_021f3888(GameSystem *gsys, int switchIndex);

// Switches the player between grid and rail movement if the switch is in the state that the place calls for

#endif // BWPORT_FIELD_GYM_ICIRRUS_H
