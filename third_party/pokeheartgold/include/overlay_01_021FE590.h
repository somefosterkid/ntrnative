#ifndef POKEHEARTGOLD_OVERLAY_01_021FE590_H
#define POKEHEARTGOLD_OVERLAY_01_021FE590_H

#include "global.h"
#include "overlay_01_021F1348.h"

void *ov01_021FE590(FieldEffectManager *fieldEffMan);

// The context of the berry patch field effect renderer (renderer 0x10).
typedef struct BerryPatchEffectCounter {
    int counter;
    BOOL isEnabled;
    FieldEffectManager *fieldEffMan;
} BerryPatchEffectCounter;

void ov01_021FE5A4(void *arg0);
void ov01_021FE5CC(BerryPatchEffectCounter *arg0);
void ov01_021FE61C(BerryPatchEffectCounter *counter);

#endif // POKEHEARTGOLD_OVERLAY_01_021FE590_H
