#include "global.h"

#include "filesystem.h"
#include "overlay_01_021F1348.h"

enum HeapID ov01_021F1478(FieldEffectManager *fieldEffMan);

void ov01_021F147C(FieldEffectManager *fieldEffMan);

void ov01_021F147C(FieldEffectManager *fieldEffMan) {
    fieldEffMan->fldEffNARC = NARC_New(NARC_a_1_0_3, ov01_021F1478(fieldEffMan));
}
