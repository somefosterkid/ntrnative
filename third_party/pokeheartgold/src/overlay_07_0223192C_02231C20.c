#include "global.h"
#include "overlay_07_battle_anim_funcs.h"

void ov07_02231C20(void *ctx, void *param1, void *param2);

u8 sub_02015530(void *a);

u8 ov07_02231924(void *ctx, void *b);

s32 ov07_0221BFC0(void *ctx);

void ov07_02231B00(s32 a, u32 *b, s32 c, s32 d);

void ov07_02231C20(void *ctx, void *param1, void *param2) {
    u8 v0 = sub_02015530(ov07_0221C478(ctx));
    u8 v1 = ov07_02231924(ctx, param1);

    ov07_02231B00(v1, (u32 *)param2, ov07_0221BFC0(ctx), v0);
}
