#ifndef POKEHEARTGOLD_OVERLAY_01_021FE6B4_H
#define POKEHEARTGOLD_OVERLAY_01_021FE6B4_H

#include "global.h"
#include "billboard.h"
#include "field_types_def.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"

struct BerryPatchEffectCounter;

// User data of the berry patch sparkle funcs table ov01_02209084 (ov01_021FE66C passes it to ov01_021F1620).
typedef struct BerryPatchSparkleEffectContext {
    FieldEffectManager *fieldEffMan;
    struct BerryPatchEffectCounter *effectCounter;
    LocalMapObject *mapObject;
} BerryPatchSparkleEffectContext;

// Context of the funcs table ov01_02209084 (0x1C bytes on the DS): init copies the user data to `context`.
typedef struct BerryPatchSparkleEffect {
    int dummy;
    int animationFrame;
    BOOL isAnimating;
    BerryPatchSparkleEffectContext context;
    Billboard *graphicsObject;
} BerryPatchSparkleEffect;

OverworldAnimManager *ov01_021FE66C(LocalMapObject *mapObject);
BOOL ov01_021FE6B4(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect);
void ov01_021FE6F4(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect);
void ov01_021FE70C(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect);
void ov01_021FE768(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect);

#endif // POKEHEARTGOLD_OVERLAY_01_021FE6B4_H
