#include "global.h"

#include "overlay_07_earthquake_context.h"
#include "overlay_07_battle_anim_funcs.h"

#include "heap.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

typedef struct UnkStruct_ov07_02228D08_ptr {
    u8 unk_0;
    u8 unk_1[0xB];
    void *unk_C;
    SpriteManager *unk_10;
    u8 unk_14[0x4];
    ManagedSprite *unk_18;
    u8 unk_1C;
    u8 unk_1D[0x23];
    s16 unk_40;
    s16 unk_42;
    u8 unk_44[0x20];
    u8 unk_64;
} UnkStruct_ov07_02228D08_ptr;

void ov07_02232020(BattleAnimSystem *system, int targets, BattleAnimSpriteInfo *sprites, int *count);

void ov07_02228C8C(BattleAnimSystem *system);

void ov07_02228C8C(BattleAnimSystem *system) {
    s32 targets;
    s32 i;
    EarthquakeContext *ctx;

    ctx = ov07_022324D8(system, sizeof(EarthquakeContext));
    ctx->state = 0;
    ctx->shakeCount = 0;
    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(system);
    ctx->bgConfig = ov07_0221C4A0(ctx->battleAnimSys);
    if (ov07_0221FAB0(ctx->battleAnimSys) == 1) {
        targets = 0x40;
    } else {
        targets = 0xA;
    }
    ov07_02232020(system, targets, ctx->sprites, &ctx->spriteCount);
    i = 0;
    if (ctx->spriteCount > 0) {
        do {
            ctx->sprites[i].basePos.x = (s16)Pokepic_GetAttr(ctx->sprites[i].monSprite, 0);
            ctx->sprites[i].basePos.y = (s16)Pokepic_GetAttr(ctx->sprites[i].monSprite, 1);
            i += 1;
        } while (i < ctx->spriteCount);
    }
    ov07_0221C410(ctx->battleAnimSys, ov07_02228B38, ctx);
}

s32 ov07_022222B4(void *);

s32 ov07_022227D8(void *);

int ov07_02222AF4(void *);

void ov07_02228D08(SysTask *arg0, void *param);

void ov07_02228D08(SysTask *arg0, void *param) {
    UnkStruct_ov07_02228D08_ptr *ptr = param;
    if (ptr->unk_0 == 0) {
        if (ov07_022222B4((void *)&ptr->unk_40) == 0) {
            ptr->unk_0 = (u8)(ptr->unk_0 + 1);
        }
        ov07_022227D8((void *)&ptr->unk_1C);
        ov07_02222AF4((void *)&ptr->unk_64);
        ManagedSprite_SetPositionXY(ptr->unk_18, ptr->unk_40, ptr->unk_42);
        ManagedSprite_TickFrame(ptr->unk_18);
        SpriteSystem_DrawSprites(ptr->unk_10);
        return;
    }
    ov07_0221C448(ptr->unk_C, arg0);
    Heap_Free(ptr);
}
