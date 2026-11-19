#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_spite_context.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
int ov07_0221FAE8(BattleAnimSystem *system);

u32 ov07_0221FAEC(BattleAnimSystem *system, int bg);

u32 ov07_02222D88(s32 x, s32 y);
#else
u8 ov07_0221FAE8(BattleAnimSystem *system);

u8 ov07_0221FAEC(BattleAnimSystem *system, int bg);

s16 ov07_02222D88(u16 x, u16 y);
#endif

u32 ov07_0221FAF8(BattleAnimSystem *system, int bg);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_0222D148(BattleAnimSystem *system);

void ov07_0222D148(BattleAnimSystem *system) {
    u32 sp1C;
    s32 sp18;
    BattleAnimSystem *sp14;
    Pokepic *temp_r0;
    s16 var_r5;
    s16 var_r7;
    s32 temp_r0_2;
    s32 var_r6;
    SpiteContext *ctx;

    sp14 = system;
    ctx = ov07_022324D8(system, sizeof(SpiteContext));
    ctx->battleAnimSys = system;
    ctx->paletteData = ov07_0221FA78(system);
    ov07_02231E74(ctx->battleAnimSys, 0, 0x10);
    ov07_02222AC4(&ctx->alpha, 0, 8, 0x10, 8, 0x18);
    temp_r0 = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    ctx->defenderSprite = temp_r0;
    var_r7 = Pokepic_GetAttr(temp_r0, POKEPIC_X);
    var_r5 = Pokepic_GetAttr(ctx->defenderSprite, POKEPIC_Y);
    var_r5 = (s16)(var_r5 - Pokepic_GetAttr(ctx->defenderSprite, 0x29)) - 0x28;
    ctx->monSprite = ov07_0221C4E8(ctx->battleAnimSys, 0);
    temp_r0_2 = ov07_02231924(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    if ((temp_r0_2 == 5) || (temp_r0_2 == 2)) {
        ManagedSprite_SetDrawFlag(ctx->monSprite, 1);
        ManagedSprite_SetPriority(ctx->monSprite, 2);
    } else {
        var_r6 = ov07_0221FAEC(ctx->battleAnimSys, 1);
        SetBgPriority(var_r6, (u16)ov07_0221FAE8(ctx->battleAnimSys));
        SetBgPriority(0U, (u8)(ov07_0221FAE8(ctx->battleAnimSys) + 1));
        ManagedSprite_SetDrawFlag(ctx->monSprite, 0);
    }
    var_r6 = (s32)(s16)(var_r5 + 0x50);
    sp18 = (s32)var_r5;
    if ((s32)var_r5 < 0) {
        var_r5 = 0;
    }
    if ((s32)var_r6 > 0xBF) {
        var_r6 = 0xBF;
    }
    var_r7 -= 0x28;
    sp1C = ov07_0221FAF8(sp14, 1);
    var_r7 = ov07_02222D88((u16)(0 - var_r7), (u16)(0 - sp18));
    ctx->bgScroll = ov07_02222CCC((u8)var_r5, var_r6, 0x38EU, 0x5000, 0x64, sp1C, 0, (u32)var_r7, ov07_0221BFD0(sp14));
    PaletteData_BlendPalettes(ctx->paletteData, PLTTBUF_MAIN_BG, 0x100U, 8U, 0U);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222D050, ctx);
}
