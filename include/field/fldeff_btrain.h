#ifndef BWPORT_FIELD_FLDEFF_BTRAIN_H
#define BWPORT_FIELD_FLDEFF_BTRAIN_H

#include "types.h"
#include "field/field_effect.h"
#include "gfl/g3d.h"

#define BTRAIN_COUNT 8
#define BTRAIN_ANM_COUNT 4

// The data of the Battle Subway trains' effect, which fldeff_btrain.c makes and ov021_021d0160.c (the rest of
// pokebw2's src/ov036/fldeff_btrain.c) loads and frees: pokebw2's FieldEffectBTrain, 0x54 bytes in Black too. The
// resources are void *, as gfl/g3d.h has them
typedef struct FieldEffectBTrain {
    FieldEffects *effects;
    void *mdlRes[BTRAIN_COUNT];
    void *anmRes[BTRAIN_ANM_COUNT];
    G3DModel *mdl[BTRAIN_COUNT];
} FieldEffectBTrain;

#endif // BWPORT_FIELD_FLDEFF_BTRAIN_H
