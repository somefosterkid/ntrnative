#ifndef BWPORT_FIELD_FLDEFF_REFLECT_H
#define BWPORT_FIELD_FLDEFF_REFLECT_H

#include "types.h"
#include "field/field_effect.h"

// The data of the reflection effect, which fldeff_reflect.c makes (pokebw2's FieldEffectReflect, 4 bytes): the
// effects it belongs to (the tasks of ov021_021bbfb8.c draw the reflections)
typedef struct FieldEffectReflect {
    FieldEffects *effects;
} FieldEffectReflect;

#endif // BWPORT_FIELD_FLDEFF_REFLECT_H
