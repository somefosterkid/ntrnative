// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/gimmick_nuvema.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_GIMMICK_NUVEMA_H
#define BWPORT_FIELD_GIMMICK_NUVEMA_H

#include "types.h"
#include "struct_decls.h"

// Black's gimmick id of the Nuvema Town gimmick (Black 2 has 0x20): what gimmick_nuvema.c and scrcmd_sp_poke.c read
#define GIMMICK_NUVEMA 0x36

void func_ov065_021f37c4(Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov065_021f36c0(Field *field);
void func_ov065_021f37dc(Field *field);
// Shows the model and plays its animations
void func_ov065_021f385c(Field *field);
// Waits for the animations to finish
GameEvent *func_ov065_021f38bc(GameSystem *gsys);

#endif // BWPORT_FIELD_GIMMICK_NUVEMA_H
