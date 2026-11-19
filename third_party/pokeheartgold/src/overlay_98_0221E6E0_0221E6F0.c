#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_98_sprite_ctx.h"

void ov98_0221E6F0(PokeathlonSpriteContext *param_1, ManagedSprite **sprites, const int *resIds, u32 count, int start, s16 x, s16 y, BOOL isMain) {
    ManagedSpriteTemplate tmpl = { 0 };
    ManagedSprite **out;
    u32 i;
    int curX;

    GF_ASSERT(count <= 10);
    curX = x;
    out = sprites + start;
    for (i = 0; i < 10; i++) {
        tmpl.animation = i < count;
        tmpl.resIdList[0] = resIds[0];
        tmpl.resIdList[1] = resIds[1];
        tmpl.resIdList[2] = resIds[2];
        tmpl.resIdList[3] = resIds[3];
        tmpl.vram = isMain ? NNS_G2D_VRAM_TYPE_2DMAIN : NNS_G2D_VRAM_TYPE_2DSUB;
        tmpl.x = curX;
        tmpl.y = y;
        *out = ov98_0221E6E0(param_1, &tmpl);
        ManagedSprite_SetAnimateFlag(*out++, 1);
        curX += 16;
    }
}
