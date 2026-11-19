#include "global.h"

#include "overlay_01_021F1348.h"

void ov01_021F151C(FieldEffectManager *fieldEffMan, FieldEffectRenderer *renderer);

BOOL ov01_021F15B4(const FieldEffectRenderer *renderer);

void ov01_021F1538(FieldEffectManager *fieldEffMan);

void ov01_021F1538(FieldEffectManager *fieldEffMan) {
    s32 v0;
    FieldEffectRenderer *v1;

    v0 = fieldEffMan->rendererCount;
    v1 = fieldEffMan->renderers;
    if (v0 != 0) {
        do {
            if (ov01_021F15B4(v1) == 0) {
                ov01_021F151C(fieldEffMan, v1);
            }
            v1++;
            v0 -= 1;
        } while (v0 != 0);
    }
}
