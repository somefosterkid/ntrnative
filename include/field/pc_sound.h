// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/pc_sound.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_FIELD_PC_SOUND_H
#define BWPORT_FIELD_PC_SOUND_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "system/game_event.h"
#include "struct_decls.h"

GameEventReturnCode pcLogOffSound(GameEvent *event, u32 *state, void *data);

// Black 2's; twinned code confirms the offsets of gameSystem, field, pcProp and skipSound, and its size in Black
struct PCSoundEventData {
    GameSystem *gameSystem;
    Field *field;
    FieldChunkPropHolder *volatile pcProp;
    u32 unkC;
    u32 skipSound;
};

GameEventReturnCode pcEntrySound(GameEvent *event, u32 *state, void *data);
GameEvent *CreatePCSoundCallEvent(GameEvent *parent, GameSystem *gsys, Field *field);
GameEvent *func_ov021_021c5bd0(GameSystem *gsys, u16 option, u16 *result);
GameEventReturnCode func_ov021_021c97b8(GameEvent *event, u32 *state, void *data);
GameEvent *func_ov021_021c98f4(GameEvent *parent, GameSystem *gsys, Field *field, u32 skipSound);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *func_ov021_021c9828(GameEvent *parent, GameSystem *gsys, Field *field);

#endif // BWPORT_FIELD_PC_SOUND_H
