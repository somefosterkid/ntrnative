// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/encounter_effect_model.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_ENCOUNTER_EFFECT_MODEL_H
#define BWPORT_FIELD_ENCOUNTER_EFFECT_MODEL_H

#include "types.h"
#include "system/game_event.h"
#include "struct_decls.h"

// The create functions of the effects, by the model they play. white picks the fade to white that ends the effect
GameEvent *func_ov076_021f5260(GameSystem *gsys, Field *field, BOOL white);
GameEvent *func_ov076_021f5270(GameSystem *gsys, Field *field, BOOL white);
GameEvent *func_ov076_021f5280(GameSystem *gsys, Field *field, BOOL white);
GameEvent *func_ov076_021f5290(GameSystem *gsys, Field *field, BOOL white);
GameEvent *func_ov076_021f52a0(GameSystem *gsys, Field *field, BOOL white);
// pokebw2's EncEffModel_Create
GameEvent *func_ov076_021f52d8(GameSystem *gsys, Field *field, u32 modelId, u32 animId, BOOL white);
// pokebw2's EncEffModel_Render
// The render function of the effects
void func_ov076_021f5324(EncEff *effect);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's EncEffModel_CreateZoroark
// Zoroark's, which also plays its sound effect
GameEvent *func_ov076_021f52b0(GameSystem *gsys, Field *field, BOOL white);
// pokebw2's EncEffModel_EventCallback
GameEventReturnCode func_ov076_021f5334(GameEvent *event, u32 *state, void *data);

#endif // BWPORT_FIELD_ENCOUNTER_EFFECT_MODEL_H
