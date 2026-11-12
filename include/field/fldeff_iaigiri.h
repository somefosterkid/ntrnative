#ifndef BWPORT_FIELD_FLDEFF_IAIGIRI_H
#define BWPORT_FIELD_FLDEFF_IAIGIRI_H

#include "types.h"
#include "field/field_effect.h"

#define IAIGIRI_ANM_COUNT 3

// The data of field effect 0xc, the slash Cut leaves, which fldeff_iaigiri.c makes (pokebw2's FieldEffectIaigiri,
// 0x1c bytes): the slash's model and animation resources (the tasks of ov021_021c9550.c draw it)
typedef struct {
    FieldEffects *effects;
    void *mdlRes;
    void *anmRes[IAIGIRI_ANM_COUNT];
    u32 unk14;
    u32 unk18;
} FieldEffectIaigiri;

#endif // BWPORT_FIELD_FLDEFF_IAIGIRI_H
