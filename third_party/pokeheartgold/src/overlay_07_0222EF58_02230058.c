#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "overlay_07_grass_whistle_sprite_context.h"
#include "sys_task.h"

s32 ov07_0222FFD0(void *);

void ov07_02230058(SysTask *arg0, void *ptr);

void ov07_02230058(SysTask *arg0, void *ptr) {
    s32 v0;

    if (ov07_0222FFD0(ptr) != 0) {
        v0 = 0;
        do {
            Sprite_DeleteAndFreeResources(((GrassWhistleSpriteContext *)ptr)->sprites[v0]);
            v0 += 1;
        } while (v0 < 0xF);
        Heap_Free(ptr);
        ov07_0221C448(*(void **)ptr, arg0);
        return;
    }
    SpriteSystem_DrawSprites(((GrassWhistleSpriteContext *)ptr)->spriteMan);
}
