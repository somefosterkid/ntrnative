#include "global.h"

#include "error_handling.h"
#include "overlay_112_sprite_app.h"

void ov112_021F1EFC(Ov112SpriteRec *, BOOL);

void ov112_021F1F3C(Ov112SpriteRec *, u32);

void ov112_021F1F54(Ov112SpriteRec *, s16, s16, u32);

extern s8 ov112_021FF4C8[];

void ov112_021F238C(Ov112SpriteApp *ctx);

void ov112_021F238C(Ov112SpriteApp *ctx) {
    if (ctx->bits148.active) {
        switch (ctx->bits148.step) {
        case 0:
            ov112_021F1F54(&ctx->spriteRecs[1], 0x98, 0x48 - ov112_021FF4C8[ctx->bits148.timer], 1);
            ctx->bits148.timer++;
            if (ov112_021FF4C8[ctx->bits148.timer] == 0x6F) {
                ctx->bits148.timer = 0;
                ctx->bits148.loop++;
                if (ctx->bits148.loop >= 2) {
                    ov112_021F1F3C(&ctx->spriteRecs[1], 3);
                    ctx->bits148.loop = 0;
                    ctx->bits148.step++;
                    return;
                }
            }
            break;
        case 1: {
            s16 y = ctx->bits148.timer * 8 + 0x98;
            ctx->bits148.timer++;
            if (ctx->bits148.timer >= 5) {
                ov112_021F1EFC(&ctx->spriteRecs[1], (ctx->bits148.timer & 1) == 1);
            }
            if (ctx->bits148.timer >= 7) {
                ov112_021F1EFC(&ctx->spriteRecs[1], 0);
                ctx->bits148.timer = 0;
                ctx->bits148.step++;
                return;
            }
            ov112_021F1F54(&ctx->spriteRecs[1], y, 0x48, 0);
            break;
        }
        case 2:
            break;
        default:
            GF_AssertFail();
            break;
        }
    }
}
