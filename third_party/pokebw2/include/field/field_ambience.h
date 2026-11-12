#ifndef POKEBW2_FIELD_FIELD_AMBIENCE_H
#define POKEBW2_FIELD_FIELD_AMBIENCE_H

#include "types.h"

// The field's ambient sound effects (ov036, file not decompiled yet; the header's name is a guess). Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// Whether a sound effect is one of the 22 ambient sounds
BOOL FieldAmbience_CheckSoundID(u32 se);
BOOL FieldAmbience_IsLooped(u32 se);

#endif // POKEBW2_FIELD_FIELD_AMBIENCE_H
