#include "global.h"

#include "sprite.h"
#include "overlay_34.h"

void ov34_0225DDB8(Sprite *param0, int param1) {
    VecFx32 v0;

    v0.x = ((fx32)(((256 - 24) > 0) ? ((256 - 24) * (1 << 12) + 0.5f) : ((256 - 24) * (1 << 12) - 0.5f)));
    v0.y = ((fx32)(((param1) > 0) ? ((param1) * (1 << 12) + 0.5f) : ((param1) * (1 << 12) - 0.5f))) + (192 << 12);
    v0.z = 0;

    Sprite_SetMatrix(param0, &v0);
}
