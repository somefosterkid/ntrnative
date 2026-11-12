// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_ambience.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_AMBIENCE_H
#define BWPORT_FIELD_FIELD_AMBIENCE_H

#include "types.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether a sound effect is one of the 22 ambient sounds
BOOL FieldAmbience_CheckSoundID(u32 se);
BOOL FieldAmbience_IsLooped(u32 se);

#endif // BWPORT_FIELD_FIELD_AMBIENCE_H
