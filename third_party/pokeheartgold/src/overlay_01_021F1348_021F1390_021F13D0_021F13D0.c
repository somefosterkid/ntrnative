#include "global.h"
#include "overlay_01_021F1348.h"

void FieldEffectManager_InitRenderers(FieldEffectManager *fieldEffMan, const u32 *ids);

void FieldEffectManager_InitRenderers(FieldEffectManager *fieldEffMan, const u32 *ids) {
    while (*ids != FIELD_EFFECT_RENDERER_ID_NONE) {
        ov01_021F13B0(fieldEffMan, *ids);
        ids++;
    }
}
