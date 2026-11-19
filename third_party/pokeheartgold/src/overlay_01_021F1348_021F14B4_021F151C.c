#include "global.h"
#include "overlay_01_021F1348.h"

void ov01_021F15A0(FieldEffectRenderer *renderer);

const FieldEffectRendererFuncs *ov01_021F15C4(u32 id);

void ov01_021F151C(FieldEffectManager *fieldEffMan, FieldEffectRenderer *renderer);

void ov01_021F151C(FieldEffectManager *fieldEffMan, FieldEffectRenderer *renderer) {
    const FieldEffectRendererFuncs *funcs;

    funcs = ov01_021F15C4(renderer->id);
    funcs->freeFunc(renderer->context);
    ov01_021F15A0(renderer);
}
