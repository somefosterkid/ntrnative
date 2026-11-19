#include "global.h"

#include "error_handling.h"
#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;
struct PokemonSprite;

s32 ov07_022222B4(void *arg0);

s32 ov07_022222B4(void *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (*(s32 *)((u8 *)arg0 + 4) != 0) {
        *(s32 *)((u8 *)arg0 + 0x10) = (s32)(*(s32 *)((u8 *)arg0 + 0x10) + *(s32 *)((u8 *)arg0 + 8));
        *(s32 *)((u8 *)arg0 + 0x14) = (s32)(*(s32 *)((u8 *)arg0 + 0x14) + *(s32 *)((u8 *)arg0 + 0xC));
        *(s16 *)arg0 = (s16)((s32) * (s32 *)((u8 *)arg0 + 0x10) >> 0xC);
        *(s16 *)((u8 *)arg0 + 2) = (s16)((s32) * (s32 *)((u8 *)arg0 + 0x14) >> 0xC);
        *(s32 *)((u8 *)arg0 + 4) = (s32)(*(s32 *)((u8 *)arg0 + 4) - 1);
        return 1;
    }
    return 0;
}

void ov07_0222207C(XYTransformContext *ctx, struct ManagedSprite *sprite, s16 cx, s16 cy);

s32 ov07_022222F0(XYTransformContext *arg0, struct ManagedSprite *arg1);

s32 ov07_022222F0(XYTransformContext *arg0, struct ManagedSprite *arg1) {
    if (ov07_022222B4(arg0) != 0) {
        ov07_0222207C(arg0, arg1, 0, 0);
        return 1;
    }
    return 0;
}

void ov07_022220B8(XYTransformContext *ctx, struct PokemonSprite *sprite, s16 cx, s16 cy);

s32 ov07_02222314(XYTransformContext *arg0, struct PokemonSprite *arg1);

s32 ov07_02222314(XYTransformContext *arg0, struct PokemonSprite *arg1) {
    if (ov07_022222B4(arg0) != 0) {
        ov07_022220B8(arg0, arg1, 0, 0);
        return 1;
    }
    return 0;
}
