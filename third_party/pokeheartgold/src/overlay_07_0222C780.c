#include "global.h"

#include "heap.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "pokemon_types_def.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_acid_armor_context.h"
#include "overlay_07_confusion_context.h"

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void ov07_0222C780(BattleAnimSystem *system) {
    ConfusionContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(ConfusionContext));

    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(ctx->battleAnimSys);
    ctx->state = 0;
    ctx->unused = 0;
    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    ctx->defenderX = Pokepic_GetAttr(ctx->defenderSprite, 0);
    ctx->defenderY = Pokepic_GetAttr(ctx->defenderSprite, 1);
    ctx->defenderShadowHeight = Pokepic_GetAttr(ctx->defenderSprite, 0x29);
    ov07_022227A8(&ctx->shake, 2, 0, 1, 6);
    ctx->monSprite = ov07_0221C4E8(ctx->battleAnimSys, 0);
    ManagedSprite_SetOamMode(ctx->monSprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(ctx->monSprite, 2U);
    ManagedSprite_SetDrawPriority(ctx->monSprite, 0x64U);
    ManagedSprite_SetPriority(ctx->monSprite, 1);
    ov07_02222590(&ctx->scale, 0xA, 0xC, 0xA, 0xF, 0xA, 7);
    ctx->jitterDir = 1;
    ov07_02231E08(ctx->battleAnimSys, 0x1C, 0xF);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222C6A8, ctx);
}

void ov07_0222C850(AcidArmorContext *ctx, void *buffer) {
    u32 value = ((-ctx->attackerY & 0xFFFF) << 16) | (-ctx->attackerX & 0xFFFF);
    MI_CpuFill32(buffer, value, sizeof(u32) * 192);
}
