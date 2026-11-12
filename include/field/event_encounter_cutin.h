// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_encounter_cutin.h: the
// prototypes of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_ENCOUNTER_CUTIN_H
#define BWPORT_FIELD_EVENT_ENCOUNTER_CUTIN_H

// Event function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).

#include "types.h"
#include "system/game_event.h"
#include "struct_decls.h"

GameEvent *EventEncountEffectCutin_Create(GameSystem *gsys, Field *field, u32 selection, u32 param);
GameEventReturnCode EventEncountEffectCutin_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventEncountEffectCutinFallback_Callback(GameEvent *event, u32 *state, void *data);

// The encounter effects' create functions, called with the field
GameEvent *func_ov073_021f5260(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f526c(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5278(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5284(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5290(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f529c(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52a8(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52b4(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52c0(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52cc(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52d8(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52e4(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52f0(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5314(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5308(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5320(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f52fc(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f532c(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5338(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5344(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5350(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f535c(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5368(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5374(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f5380(GameSystem *gsys, Field *field, u32 param);
GameEvent *func_ov073_021f538c(GameSystem *gsys, Field *field, u32 param);

#endif // BWPORT_FIELD_EVENT_ENCOUNTER_CUTIN_H
