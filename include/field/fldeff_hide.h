#ifndef BWPORT_FIELD_FLDEFF_HIDE_H
#define BWPORT_FIELD_FLDEFF_HIDE_H

#include "types.h"
#include "field/field_effect.h"

// The objects an actor hides in or behind: Black has 2 kinds where Black 2 has 3
#define HIDE_KIND_COUNT 2

// The data of field effect 0xf, which fldeff_hide.c makes (pokebw2's FieldEffectHide, 0xc bytes in Black): each
// kind's resource group and object (the tasks of ov021_021d046c.c draw them)
typedef struct {
    u16 resGroup[HIDE_KIND_COUNT];
    FieldEffects *effects;
    u16 objIdx[HIDE_KIND_COUNT];
} FieldEffectHide;

#endif // BWPORT_FIELD_FLDEFF_HIDE_H
