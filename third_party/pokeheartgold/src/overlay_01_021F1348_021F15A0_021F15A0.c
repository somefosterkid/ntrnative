#include "global.h"
#include "overlay_01_021F1348.h"

void ov01_021F15A0(FieldEffectRenderer *renderer);

void ov01_021F15A0(FieldEffectRenderer *renderer) {
    renderer->id = FIELD_EFFECT_RENDERER_ID_NONE;
    renderer->context = NULL;
}

void ov01_021F15AC(FieldEffectRenderer *renderer, u32 id, void *context);

void ov01_021F15AC(FieldEffectRenderer *renderer, u32 id, void *context) {
    renderer->id = id;
    renderer->context = context;
}

BOOL ov01_021F15B4(const FieldEffectRenderer *renderer);

BOOL ov01_021F15B4(const FieldEffectRenderer *renderer) {
    if (renderer->id == FIELD_EFFECT_RENDERER_ID_NONE) {
        return TRUE;
    }
    return FALSE;
}
