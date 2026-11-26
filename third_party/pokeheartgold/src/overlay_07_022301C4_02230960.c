#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"

// the layout of MegahornContext (overlay_07_022301C4_02230994_02230994.c)
typedef struct UnkStruct_ov07_02230960_ptr {
    BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    XYTransformContext pos;
    ManagedSprite *hornSprite;
} UnkStruct_ov07_02230960_ptr;

s32 ov07_022222F0(void *, ManagedSprite *);

void ov07_02230960(SysTask *arg0, void *param);

void ov07_02230960(SysTask *arg0, void *param) {
    UnkStruct_ov07_02230960_ptr *ptr = param;
    if (ov07_022222F0((void *)&ptr->pos, ptr->hornSprite) == 0) {
        Sprite_DeleteAndFreeResources(ptr->hornSprite);
        Heap_Free(ptr);
        ov07_0221C448(ptr->battleAnimSys, arg0);
        return;
    }
    SpriteSystem_DrawSprites(ptr->spriteManager);
}
