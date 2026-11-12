// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/gym_driftveil_lift.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_GYM_DRIFTVEIL_LIFT_H
#define BWPORT_FIELD_GYM_DRIFTVEIL_LIFT_H

#include "types.h"
#include "struct_decls.h"

void func_ov044_021f3740(Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov044_021f36c0(Field *field);
void func_ov044_021f392c(FieldExpObjSystem *system);
void func_ov044_021f3970(GameSystem *gsys);
void func_ov044_021f3764(Field *field);
GameEvent *func_ov044_021f3770(GameSystem *gsys, BOOL down);

#endif // BWPORT_FIELD_GYM_DRIFTVEIL_LIFT_H
