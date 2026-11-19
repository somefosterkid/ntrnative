#include "global.h"
#include "overlay_01_021F1348.h"

FieldEffectRenderer *ov01_021F1560(FieldEffectManager *fieldEffMan) {
    u32 i = fieldEffMan->rendererCount;
    FieldEffectRenderer *iter = fieldEffMan->renderers;

    while (i) {
        if (ov01_021F15B4(iter) == 1) {
            return iter;
        }

        iter++;
        i--;
    }

    GF_AssertFail();
    return NULL;
}

FieldEffectRenderer *ov01_021F1588(FieldEffectManager *fieldEffMan, u32 id) {
    u32 i = fieldEffMan->rendererCount;
    FieldEffectRenderer *iter = fieldEffMan->renderers;

    while (i) {
        if (iter->id == id) {
            return iter;
        }

        iter++;
        i--;
    }

    return NULL;
}
