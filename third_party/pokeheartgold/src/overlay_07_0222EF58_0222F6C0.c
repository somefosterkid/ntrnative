#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "overlay_07_memento_context.h"
#include "sys_task.h"

s32 ov07_022222F0(void *, ManagedSprite *);

s32 ov07_0222260C(void *);

s32 ov07_02222644(void *, f32 *, f32 *);

static s32 ov07_0222F6C0(void *arg0);

static s32 ov07_0222F6C0(void *arg0) {
    f32 x;
    f32 y;
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = *((s32 *)&((MementoContext *)arg0)->spriteState);
    v2 = 0;
    switch (v0) {
    case 0:
        *((s32 *)&((MementoContext *)arg0)->spriteDelay) = (s32)(*((s32 *)&((MementoContext *)arg0)->spriteDelay) - 1);
        ov07_022222F0(&((MementoContext *)arg0)->defenderPos, ((MementoContext *)arg0)->defenderSprite);
        if ((s32) *((s32 *)&((MementoContext *)arg0)->spriteDelay) < 0) {
            *((s32 *)&((MementoContext *)arg0)->spriteState) = (s32)(*((s32 *)&((MementoContext *)arg0)->spriteState) + 1);
        }
        break;
    case 1:
        ov07_022222F0(&((MementoContext *)arg0)->defenderPos, ((MementoContext *)arg0)->defenderSprite);
        v1 = ov07_0222260C(&((MementoContext *)arg0)->scale);
        ov07_02222644(&((MementoContext *)arg0)->scale, &x, &y);
        if (*((s32 *)&((MementoContext *)arg0)->defenderFlipped) == 1) {
            x = 0.0f - x;
        }
        ManagedSprite_SetAffineScale(((MementoContext *)arg0)->defenderSprite, x, y);
        if (v1 == 0) {
            *((s32 *)&((MementoContext *)arg0)->spriteState) = (s32)(*((s32 *)&((MementoContext *)arg0)->spriteState) + 1);
            v2 = 1;
        }
        break;
    case 2:
        v2 = 1;
        break;
    }
    return v2;
}


s32 ov07_0222F434(void *);

s32 ov07_0222F4D4(void *);

s32 ov07_0222F600(void *);

s32 ov07_02231E08(void *, s32, s32);

void ov07_0222F764(SysTask *arg0, void *ptr);

void ov07_0222F764(SysTask *arg0, void *ptr) {
    s32 v1;
    u32 v0;

    v0 = *((u32 *)&((MementoContext *)ptr)->state);
    switch (v0) {
    case 0:
        ov07_02231E08(*(void **)ptr, 0, 0x10);
        ov07_02222AC4(&((MementoContext *)ptr)->alpha, 0, 8, 0x10, 8, 8);
        *((u32 *)&((MementoContext *)ptr)->state) = (u32)(*((u32 *)&((MementoContext *)ptr)->state) + 1);
        break;
    case 1:
        if (ov07_02222AF4(&((MementoContext *)ptr)->alpha) != 0) {
            *((u32 *)&((MementoContext *)ptr)->state) = (u32)(*((u32 *)&((MementoContext *)ptr)->state) + 1);
            ov07_0222F434(ptr);
        }
        break;
    case 2:
        if (ov07_0222F4D4(ptr) != 0) {
            *((u32 *)&((MementoContext *)ptr)->state) = (u32)(*((u32 *)&((MementoContext *)ptr)->state) + 1);
            *((s32 *)&((MementoContext *)ptr)->delay) = 2;
        }
        break;
    case 3:
        v1 = *((s32 *)&((MementoContext *)ptr)->delay) - 1;
        *((s32 *)&((MementoContext *)ptr)->delay) = v1;
        if (v1 < 0) {
            ov07_0222F600(ptr);
            *((u32 *)&((MementoContext *)ptr)->state) = (u32)(*((u32 *)&((MementoContext *)ptr)->state) + 1);
        }
        break;
    case 4:
        if (ov07_0222F6C0(ptr) != 0) {
            *((u32 *)&((MementoContext *)ptr)->state) = (u32)(*((u32 *)&((MementoContext *)ptr)->state) + 1);
            ov07_02222AC4(&((MementoContext *)ptr)->alpha, 8, 0, 8, 0x10, 8);
        }
        break;
    case 5:
        if (ov07_02222AF4(&((MementoContext *)ptr)->alpha) != 0) {
            *((u32 *)&((MementoContext *)ptr)->state) = (u32)(*((u32 *)&((MementoContext *)ptr)->state) + 1);
        }
        break;
    case 6:
        ov07_0221C448(*(void **)ptr, arg0);
        Heap_Free(ptr);
        break;
    }
    SpriteSystem_DrawSprites(((MementoContext *)ptr)->pokemonSpriteManager);
}
