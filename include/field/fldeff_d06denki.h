#ifndef BWPORT_FIELD_FLDEFF_D06DENKI_H
#define BWPORT_FIELD_FLDEFF_D06DENKI_H

#include "types.h"
#include "gfl/g3d.h"
#include "field/field_effect.h"

// The data of field effect 0xb, Chargestone Cave's electric effects, which fldeff_d06denki.c makes (pokebw2's
// FieldEffectD06Denki, 0x34 bytes): the resources of its models and animations, and the model, animation and actor
// of the sparks over a stone (ov021_021c8f7c.c)
typedef struct {
    FieldEffects *effects;
    void *mdlRes[4];
    void *anmRes[5];
    void *anm;
    G3DModel *mdl;
    G3DActor *actor;
} FieldEffectD06Denki;

#endif // BWPORT_FIELD_FLDEFF_D06DENKI_H
