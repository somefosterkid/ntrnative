#include "global.h"

#include "field_types_def.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE6B4.h"
#include "overworld_anim_manager.h"

extern const OverworldAnimManagerFuncs ov01_02209084;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FE590.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_02209084 = OVERWORLD_ANIM_MANAGER_FUNCS(BerryPatchSparkleEffect, ov01_021FE6B4, ov01_021FE6F4, ov01_021FE70C, ov01_021FE768);
#endif

void ov01_021F93AC(const LocalMapObject *param0, VecFx32 *adjustedObjectPos);

OverworldAnimManager *ov01_021FE66C(LocalMapObject *mapObject) {
    VecFx32 position;
    FieldEffectManager *fieldEffMan;
    BerryPatchSparkleEffectContext effectContext;
    OverworldAnimManager *effectTask;

    fieldEffMan = ov01_021F146C(mapObject);
    ov01_021F93AC(mapObject, &position);
    position.z += (FX32_ONE * 8); // Offset sparkle effect 8 units above ground

    effectContext.fieldEffMan = fieldEffMan;
    effectContext.effectCounter = ov01_021F1450(fieldEffMan, 16);
    effectTask = ov01_021F1620(fieldEffMan, &ov01_02209084, &position, 0, &effectContext, 255);

    return effectTask;
}
