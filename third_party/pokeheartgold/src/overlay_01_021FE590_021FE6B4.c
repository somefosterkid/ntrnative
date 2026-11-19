#include "global.h"

#include "billboard.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "overlay_01_021FE6B4.h"

Billboard *ov01_021F1740(FieldEffectManager *fieldEffMan, u32 id, const VecFx32 *position);

void ov01_021FE5B4(struct BerryPatchEffectCounter *counter);
void ov01_021FE5BC(struct BerryPatchEffectCounter *counter);
void ov01_021FE64C(struct BerryPatchEffectCounter *counter);
void ov01_021FE65C(struct BerryPatchEffectCounter *counter);
BOOL sub_02023DA4(Billboard *billboard);

BOOL ov01_021FE6B4(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect) {
    VecFx32 position;
    const BerryPatchSparkleEffectContext *effectContext = sub_02068D98(effect);

    sparkleEffect->context = *effectContext;

    ov01_021FE64C(sparkleEffect->context.effectCounter);
    sub_02068DB8(effect, &position);

    sparkleEffect->graphicsObject = ov01_021F1740(sparkleEffect->context.fieldEffMan, 13, &position);
    ov01_021FE5B4(sparkleEffect->context.effectCounter);

    return 1;
}

void ov01_021FE6F4(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect) {
    sub_02023DA4(sparkleEffect->graphicsObject);
    ov01_021FE5BC(sparkleEffect->context.effectCounter);
    ov01_021FE65C(sparkleEffect->context.effectCounter);
    return;
}
