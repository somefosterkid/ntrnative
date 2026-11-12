// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_gimmick_bsubway.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_FIELD_GIMMICK_BSUBWAY_H
#define BWPORT_FIELD_FIELD_GIMMICK_BSUBWAY_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// pokebw2's FieldGimmickBSubway_Setup
void func_ov052_021f36c0(Field *field);
// pokebw2's FieldGimmickBSubway_End
void func_ov052_021f3724(Field *field);
// pokebw2's FieldGimmickBSubway_Move
void func_ov052_021f3770(Field *field);
// pokebw2's FieldGimmickBSubway_StartTrainEffect
// Starts the field effect of the zone's train, which FieldGimmickBSubway_GetTrainEffect returns
void func_ov052_021f37c0(Field *field, u32 a1, const VecFx32 *pos);
// pokebw2's FieldGimmickBSubway_GetTrainEffect
void *func_ov052_021f380c(Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's FieldGimmickBSubway_StopShake
// Stops the shaking of zone 75's train
void func_ov052_021f388c(Field *field);

#endif // BWPORT_FIELD_FIELD_GIMMICK_BSUBWAY_H
