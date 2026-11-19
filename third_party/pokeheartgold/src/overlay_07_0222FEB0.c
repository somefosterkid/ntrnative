#include "global.h"

#include "sprite_system.h"
#include "sprite.h"
#include "overlay_07_battle_anim_common.h"

struct XYTransformContext;

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

void ov07_0222FEB0(ManagedSprite *sprite, XYTransformContext *pos, int dirX, int dirY);

BOOL ov07_022222F0(XYTransformContext *ctx, ManagedSprite *sprite);
BOOL ov07_0222FF04(ManagedSprite *sprite, XYTransformContext *pos);

void ov07_0222FEB0(ManagedSprite *sprite, XYTransformContext *pos, int dirX, int dirY) {
    s16 startX, startY;
    ManagedSprite_GetPositionXY(sprite, &startX, &startY);

    ov07_02222268(
        pos,
        startX + 0 * dirX,
        startX + 256 * dirX,
        startY + 0 * dirY,
        startY + (-128) * dirY,
        59);

    ManagedSprite_SetDrawFlag(sprite, 1);
}

BOOL ov07_0222FF04(ManagedSprite *sprite, XYTransformContext *pos) {
    if (ov07_022222F0(pos, sprite)) {
        s16 x, y;
        ManagedSprite_GetPositionXY(sprite, &x, &y);

        u16 degX = x * (((2) * 0xFFFF) / 360);
        s16 offsetY = FX_MulInline(FX_SinIdx(degX), ((fx32)(((14) > 0) ? ((14) * (1 << 12) + 0.5f) : ((14) * (1 << 12) - 0.5f)))) >> 12;

        ManagedSprite_SetPositionXY(sprite, x, y + offsetY);

        fx32 deltaScale = FX_MulInline(FX_SinIdx(degX), ((fx32)(((0.17f) > 0) ? ((0.17f) * (1 << 12) + 0.5f) : ((0.17f) * (1 << 12) - 0.5f))));
        deltaScale *= -1;

        f32 affineScale = 1.0f + ((f32)((deltaScale) / (f32)(1 << 12)));
        ManagedSprite_SetAffineScale(sprite, affineScale, affineScale);
        return 0;
    }

    return 1;
}
