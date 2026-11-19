#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"

typedef struct UnkStruct_ov07_02230960_ptr {
    void *unk_0;
    SpriteManager *unk_4;
    u8 unk_8;
    u8 unk_9[0x23];
    ManagedSprite *unk_2C;
} UnkStruct_ov07_02230960_ptr;

s32 ov07_022222F0(void *, ManagedSprite *);

void ov07_02230960(SysTask *arg0, void *param);

void ov07_02230960(SysTask *arg0, void *param) {
    UnkStruct_ov07_02230960_ptr *ptr = param;
    if (ov07_022222F0((void *)&ptr->unk_8, ptr->unk_2C) == 0) {
        Sprite_DeleteAndFreeResources(ptr->unk_2C);
        Heap_Free(ptr);
        ov07_0221C448(ptr->unk_0, arg0);
        return;
    }
    SpriteSystem_DrawSprites(ptr->unk_4);
}
