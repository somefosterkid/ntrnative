#include "global.h"

#include "bg_window.h"
#include "math_util.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_set_pokemon_sprite_priority_context.h"

struct BattleAnimSystem;

void ov07_02226CB0(SetPokemonSpritePriorityContext *ctx);

void ov07_02226CB0(SetPokemonSpritePriorityContext *ctx) {
    switch (ctx->common.state) {
    case 0:
        if (ctx->windowType == 0) {
            GX_SetVisibleWnd(GX_WNDMASK_W0);
            G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, 1);
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
            G2_SetWnd0Position(0, 160, 256 / 2, 192);
            ctx->sinkFullyStartFrame = 35 + (LCRandom() % 5);
        } else {
            GX_SetVisibleWnd(GX_WNDMASK_W0);
            G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, 1);
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
            G2_SetWnd0Position(256 / 2, (192 / 2) - 10, 256, 192);
            ctx->sinkFullyStartFrame = 35 + (LCRandom() % 5);
        }
        break;
    case 5:
    case 6:
        if (LCRandom() % 2) {
            if (ctx->stepCount == 0) {
                ctx->stepCount++;
                ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 4);
            }
        }
        break;
    case 7:
        if (ctx->stepCount != 1) {
            ctx->stepCount++;
            ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 4);
        }
        break;
    case 10:
    case 11:
        if (LCRandom() % 2) {
            if (ctx->stepCount == 1) {
                ctx->stepCount++;
                ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 4);
            }
        }
        break;
    case 12:
        if (ctx->stepCount != 2) {
            ctx->stepCount++;
            ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 4);
        }
        break;
    case 15:
    case 16:
        if (LCRandom() % 2) {
            if (ctx->stepCount == 2) {
                ctx->stepCount++;
                ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 4);
            }
        }
        break;
    case 17:
        if (ctx->stepCount != 3) {
            ctx->stepCount++;
            ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 4);
        }
        break;
    case 22:
    case 23:
        if (LCRandom() % 2) {
            if (ctx->stepCount == 3) {
                ctx->stepCount++;
                ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 8);
            }
        }
        break;
    case 24:
        if (ctx->stepCount != 4) {
            ctx->stepCount++;
            ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 8);
        }
        break;
    default:
        if (ctx->common.state > ctx->sinkFullyStartFrame) {
            if (ctx->stepCount < 20) {
                ManagedSprite_OffsetPositionXY(ctx->sprite, 0, 4);

                s16 x, y;
                ManagedSprite_GetPositionXY(ctx->sprite, &x, &y);

                if (y > 130) {
                    ManagedSprite_SetDrawFlag(ctx->sprite, 0);
                }

                ctx->stepCount++;
            } else {
                ManagedSprite_SetDrawFlag(ctx->sprite, 0);
            }
        }

        break;
    }
}
