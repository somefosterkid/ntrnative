// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/sp_poke_event.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SP_POKE_EVENT_H
#define BWPORT_FIELD_SP_POKE_EVENT_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's SpPokeEvent_Appear
void func_ov059_021f3aa0(GameSystem *gsys, u16 a1, const VecFx32 *pos);
// pokebw2's SpPokeEvent_CreateWaitAnimEnd
GameEvent *func_ov059_021f3b68(GameSystem *gsys, u16 a1);
// pokebw2's SpPokeEvent_CreateWaitFrame
GameEvent *func_ov059_021f3b40(GameSystem *gsys, u16 a1);
// pokebw2's SpPokeEvent_CreateJump (ov059's: ov045 has another function at the address)
GameEvent *func_ov059_021f39b8(GameSystem *gsys, u32 mode, const VecFx32 *start, const VecFx32 *end, fx32 height,
                               u32 frames);

#endif // BWPORT_FIELD_SP_POKE_EVENT_H
