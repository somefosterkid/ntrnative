#include "global.h"

#include "math_util.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_07_palette_fade.h"

typedef BallRotation UnkStruct_ov07_022335B4;

int ov07_02232540(int type);

int ov07_02232580(int type);

void ov07_02222338(void *a, void *b, s16 c, s16 d, s16 e, s16 f, u16 g, int h);

int ov07_022223CC(void *a, void *b, void *sprite);

void ov07_022344B4(void *ctx, u16 a);

void ov07_022344C0(void *ctx, int a);

int ov07_02222EE8(PaletteFadeContext *p);

void ov07_02222EF8(PaletteFadeContext *p);

PaletteFadeContext *ov07_02222F10(PaletteData *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k);

BOOL ov07_022335B4(UnkStruct_ov07_022335B4 *ctx);

BOOL ov07_022335B4(UnkStruct_ov07_022335B4 *ctx) {
    switch (ctx->unk_08) {
    case 0:
        ov07_02222338(&ctx->unk_48[0], &ctx->unk_48[1], ctx->unk_B8.unk_00, ctx->unk_B8.unk_04, ctx->unk_B8.unk_02, ctx->unk_B8.unk_06, (u16)ctx->unk_B8.unk_08, ctx->unk_B8.unk_10 << 12);
        ctx->unk_B8.unk_0C = 0;
        ctx->unk_08++;
        ctx->unk_D4 = NULL;
        if (ov07_02232540(ctx->unk_90.type) == 1) {
            int r = LCRandom() % 20 + 10;
            ManagedSprite_OffsetAffineZRotation(ctx->unk_30, r << 13);
        }
        break;
    case 1:
        if (ov07_02232540(ctx->unk_90.type) == 1) {
            ManagedSprite_OffsetAffineZRotation(ctx->unk_30, 0x2000);
            if (ctx->unk_B8.unk_0C > ctx->unk_B8.unk_08 / 2 + 10) {
                ManagedSprite_OffsetAffineZRotation(ctx->unk_30, 0x2000);
            }
            if (ov07_02232580(ctx->unk_90.type) == 1) {
                if (ctx->unk_B8.unk_0C == ctx->unk_B8.unk_08 / 2 + 10) {
                    int v = ManagedSprite_GetPaletteOverrideOffset(ctx->unk_30);
                    ctx->unk_D4 = ov07_02222F10(ctx->unk_90.paletteData, ctx->unk_90.heapID, 2, (u16)(v << 4), 16, -2, 2, 0, 14, 0xffff, 0x3ea);
                }
            }
        }
        ctx->unk_B8.unk_0C++;
        if (ctx->unk_90.type >= 6 && ctx->unk_90.type <= 8) {
            int half = ctx->unk_B8.unk_08 / 2;
            if (ctx->unk_B8.unk_0C > half && ctx->unk_B8.unk_0C < half + 5) {
                break;
            }
        }
        if (ov07_022223CC(&ctx->unk_48[0], &ctx->unk_48[1], ctx->unk_30) == 0) {
            ov07_022344B4(ctx, 0);
            ctx->unk_08++;
        }
        break;
    case 2:
        if (ov07_02232580(ctx->unk_90.type) == 1 && ctx->unk_D4 != NULL) {
            if (ov07_02222EE8(ctx->unk_D4) == 1) {
                break;
            }
            ov07_02222EF8(ctx->unk_D4);
            ctx->unk_28 = 1;
            return 0;
        } else {
            if (ctx->unk_90.type >= 15) {
                ctx->unk_28 = 1;
                return 0;
            }
            if (ctx->unk_90.ball == 0x401) {
                sub_0200602C(0x7e8, 0x75);
            }
            ctx->unk_08++;
        }
        break;
    case 3:
        ManagedSprite_SetAnim(ctx->unk_30, 1);
        ov07_022344C0(ctx, 1);
        ctx->unk_B8.unk_0C = 0;
        ctx->unk_08++;
        break;
    default:
        if (ctx->unk_B8.unk_0C == 5) {
            if (ctx->unk_90.type != 12 && ctx->unk_90.type != 13) {
                int v = ManagedSprite_GetPaletteOverrideOffset(ctx->unk_30);
                ctx->unk_D4 = ov07_02222F10(ctx->unk_90.paletteData, ctx->unk_90.heapID, 2, (u16)(v << 4), 16, -2, 2, 0, 14, 0xffff, 0x3ea);
            }
            ctx->unk_28 = 1;
        }
        ctx->unk_B8.unk_0C++;
        if (ctx->unk_B8.unk_0C > 15) {
            if (ctx->unk_90.type != 12 && ctx->unk_90.type != 13) {
                if (ov07_02222EE8(ctx->unk_D4) == 1) {
                    break;
                }
                ov07_02222EF8(ctx->unk_D4);
            }
            return 0;
        }
        break;
    }
    return 1;
}
