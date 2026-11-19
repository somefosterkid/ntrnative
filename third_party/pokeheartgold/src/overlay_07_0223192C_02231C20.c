#include "global.h"
#include "overlay_07_battle_anim_funcs.h"

void ov07_02231C20(void *ctx, void *param1, void *param2);

void *sub_02015530(void *a);

void *ov07_02231924(void *ctx, void *b);

void *ov07_0221BFC0(void *ctx);

void ov07_02231B00(void *a, void *b, void *c, void *d);

void ov07_02231C20(void *ctx, void *param1, void *param2) {
    void *v0 = sub_02015530(ov07_0221C478(ctx));
    void *v1 = ov07_02231924(ctx, param1);

    ov07_02231B00(v1, param2, ov07_0221BFC0(ctx), v0);
}
