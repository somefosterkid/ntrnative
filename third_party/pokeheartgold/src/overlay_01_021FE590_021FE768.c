#include "global.h"

#include "billboard.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE6B4.h"
#include "sys_task.h"

void sub_02023E50(Billboard *billboard, const VecFx32 *position);

void ov01_021FE768(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect) {
    VecFx32 position;

    sub_02068DB8(effect, &position);
    sub_02023E50(sparkleEffect->graphicsObject, &position);
}
