#include "global.h"

#include "frontier/overlay_80_02238034.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov80_0222A6B8(u8, u16, u16, u16 *, void **, u8 *, u32 *, u8);
#else
s32 ov80_0222A6B8(u8, u16, u16, u16 *, void **, u8 *, u32 *, s32);
#endif

s32 ov80_022380A0(u8, u32, u16 *, u8);

extern u8 ov80_0223BE98;

void ov80_02234390(ArcadeContext *arg0);

void ov80_02234390(ArcadeContext *ctx) {
    u32 v0;
    u32 v3;
    u8 *v4;
    u8 v1;
    u8 v2;

    ov80_022380A0(ctx->type, ov80_02238498(ctx), ctx->unk74, 0xE);
    v1 = ctx->unk11;
    v2 = BattleArcade_GetOpponentMonCount(ctx->type, 1);
    ov80_0222A6B8(v2, ctx->unk74[v1], ctx->unk74[v1 + 7], ctx->unk314, ctx->unk330, ctx->unk31C, ctx->unk320, (s32)(u8)BattleArcade_MultiplayerCheck((s32)ctx->type));
    v0 = ov80_02238498(ctx);
    v4 = &ov80_0223BE98;
    v3 = 0;
loop_1:
    if ((s32)v0 >= (s32)*v4) {
        ctx->cursorSpeed = *(u8 *)(&ov80_0223BE98 + 1 + (v3 * 2));
        return;
    }
    v3 += 1;
    v4 += 2;
    if (v3 >= 3U) {
        return;
    }
    goto loop_1;
}
