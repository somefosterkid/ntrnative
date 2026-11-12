// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_gimmick_d06.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_FIELD_GIMMICK_D06_H
#define BWPORT_FIELD_FIELD_GIMMICK_D06_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's GimmickD06_End
void func_ov051_021f3868(Field *field);
// pokebw2's GimmickD06_CheckPush
GameEvent *func_ov051_021f38a4(Field *field, u16 dir);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GimmickD06_Setup
void func_ov051_021f3730(Field *field);
// pokebw2's GimmickD06_Update
void func_ov051_021f38a0(Field *field);

#endif // BWPORT_FIELD_FIELD_GIMMICK_D06_H
