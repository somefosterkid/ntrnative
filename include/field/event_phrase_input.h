// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_phrase_input.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_PHRASE_INPUT_H
#define BWPORT_FIELD_EVENT_PHRASE_INPUT_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *EventPhraseInput_Create(GameSystem *gsys, Field *field, GameEvent *parent, u32 mode, u16 *result);

#endif // BWPORT_FIELD_EVENT_PHRASE_INPUT_H
