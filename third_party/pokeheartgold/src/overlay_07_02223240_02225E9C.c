#include "global.h"

#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_shake_bg_context.h"

typedef struct UnkStruct_ov07_02225F18_ptr {
    u8 unk_0[0x4];
    void *unk_4;
    u8 unk_8[0x4];
    SpriteManager *unk_C;
    u8 unk_10[0xC];
    ManagedSprite *unk_1C;
    s16 unk_20;
    u8 unk_22[0x2];
    s32 unk_24;
    s32 unk_28;
    s32 unk_2C;
} UnkStruct_ov07_02225F18_ptr;

void ov07_02225E9C(BattleAnimSystem *system);

void ov07_02225E9C(BattleAnimSystem *system) {
    s32 v1;
    ShakeBgContext *ctx;

    ctx = ov07_022324D8(system, sizeof(ShakeBgContext));
    ctx->state = 0;
    ctx->iteration = 0;
    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(system);
    ctx->bgConfig = ov07_0221C4A0(ctx->battleAnimSys);
    ctx->extentX = ov07_0221C4A8(system, 0);
    ctx->extentY = ov07_0221C4A8(system, 1);
    ctx->interval = ov07_0221C4A8(system, 2);
    ctx->amount = ov07_0221C4A8(system, 3);
    ctx->cycles = ov07_0221C4A8(system, 4);
    if (ov07_0221C4A8(system, 5) == 0) {
        v1 = 3;
    } else {
        v1 = 2;
    }
    ctx->bgLayer = v1;
    ov07_0221C410(ctx->battleAnimSys, ov07_02225DC4, ctx);
}

void ov07_02225F18(SysTask *arg0, void *param);

void ov07_02225F18(SysTask *arg0, void *param) {
    UnkStruct_ov07_02225F18_ptr *ptr = param;
    s16 v1;
    s32 v0;
    s32 v2;
    s32 v3;
    s32 v4;

    v0 = ptr->unk_2C;
    v4 = ptr->unk_24;
    if ((v4 == v0) || (ptr->unk_28 == v0)) {
        ov07_0221C448(ptr->unk_4, arg0);
        Heap_Free(ptr);
        return;
    }
    v1 = ptr->unk_20;
    if ((s32)v1 < 0) {
        v2 = v4 + v1;
        if (v2 > 0) {
            ptr->unk_24 = v2;
            ptr->unk_28 = (s32)(ptr->unk_28 + ptr->unk_20);
        } else {
            ptr->unk_24 = v0;
            ptr->unk_28 = (s32) ptr->unk_2C;
        }
    } else {
        v3 = v4 + v1;
        if (v3 < 0xF) {
            ptr->unk_24 = v3;
            ptr->unk_28 = (s32)(ptr->unk_28 + ptr->unk_20);
        } else {
            ptr->unk_24 = v0;
            ptr->unk_28 = (s32) ptr->unk_2C;
        }
    }
    ManagedSprite_TickFrame(ptr->unk_1C);
    *((s8 *)REG_MOSAIC_ADDR + 1) = ptr->unk_24 | (ptr->unk_28 * 0x10);
    SpriteSystem_DrawSprites(ptr->unk_C);
}
