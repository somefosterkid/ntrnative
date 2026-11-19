#include "global.h"

#include "sprite_system.h"
#include "unk_02005D10.h"
#include "application/pokedex/pokedex_internal.h"










void ov18_021F12C8(PokedexAppData *app, s32 idx, s16 *a2, s16 *a3, s32 a4);

BOOL ov18_021F5CC4(PokedexAppData_UnkSub0868_State77_Sub4 *ctx);

BOOL ov18_021F5CC4(PokedexAppData_UnkSub0868_State77_Sub4 *ctx) {
    s16 out[2];
    u32 rot;

    switch (ctx->state) {
    case 0:
        PlaySE(0x8ee);
        ov18_021F5180(ctx, 0x800, 0);
        ctx->state++;

    case 1:
        ManagedSprite_OffsetAffineZRotation(ctx->pokedexApp->unk_0670[7], ctx->rotation);
        rot = ManagedSprite_GetRotation(ctx->pokedexApp->unk_0670[7]);
        ov18_021F5000(ctx->pokedexApp, rot);
        ctx->rotation = ctx->rotation + ctx->rotationStep;
        if (ctx->direction == 0) {
            if (rot > 0xf600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(ctx->pokedexApp->unk_0670[7], 0xf600);
            ov18_021F5000(ctx->pokedexApp, 0xf600);
            ctx->state++;
        } else {
            if (rot < 0xa00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(ctx->pokedexApp->unk_0670[7], 0xa00);
            ov18_021F5000(ctx->pokedexApp, 0xa00);
            ctx->state++;
        }

    case 2:
        ov18_021F54C0(ctx, -0x18, 0);
        ctx->state++;

    case 3:
        if (ov18_021F55D8(ctx) == 0) {
            return 0;
        }
        ov18_021F12C8(ctx->pokedexApp, ctx->index, &out[1], &out[0], 1);
        if (out[0] <= -0x100) {
            return 0;
        }
        break;
    default:
        break;
    }
    return 1;
}
