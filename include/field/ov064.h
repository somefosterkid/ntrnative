// After pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/ov132.h: Black 2's overlay 132 is
// the same gimmick as Black's overlay 64, with the same calls from its script commands.
#ifndef BWPORT_FIELD_OV064_H
#define BWPORT_FIELD_OV064_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

GameEvent *func_ov064_021f37b8(GameSystem *gsys);
GameEvent *func_ov064_021f3970(GameSystem *gsys, const VecFx32 *from, const VecFx32 *to, fx32 a3, u32 a4);
void func_ov064_021f3a44(GameSystem *gsys, const VecFx32 *pos);
GameEvent *func_ov064_021f3ac0(GameSystem *gsys);
GameEvent *func_ov064_021f3adc(GameSystem *gsys);

#endif // BWPORT_FIELD_OV064_H
