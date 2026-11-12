#ifndef POKEBW_OV060_021F36C0_H
#define POKEBW_OV060_021F36C0_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"
#include "field/field_exp_obj.h"

// The Funfest levels gimmick's work (the field's gimmick work block 0, allocated at 4 bytes): the player whose
// Black and White levels the models show, and whether the effect at the gate plays. pokebw2's FunfestLevelsWork
// holds the same two with its scene's pointers
typedef struct FunfestLevelsFlag {
    u8 netId;
    u8 active;
} FunfestLevelsFlag;

void func_ov060_021f37b8(Field *field, VecFx32 *pos);
u8 func_ov060_021f37d8(Field *field);

// gimmick_funfest_levels.c
void func_ov060_021f3950(FunfestLevelsFlag *work, FieldExpObjSystem *system);
void func_ov060_021f399c(FunfestLevelsFlag *work, FieldExpObjSystem *system, VecFx32 *pos);
void func_ov060_021f39f4(FunfestLevelsFlag *work, FieldExpObjSystem *system);

#endif // POKEBW_OV060_021F36C0_H
