#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"

typedef struct UnkStruct_ov07_0222314C_ptr {
    u8 unk_0;
    u8 unk_1[0x3];
    void *unk_4;
    u8 unk_8[0x4];
    SpriteManager *unk_C;
    ManagedSprite *unk_10;
    Sprite **unk_14;
    Sprite **unk_18;
    Sprite **unk_1C;
} UnkStruct_ov07_0222314C_ptr;

void ov07_0222314C(SysTask *arg0, void *param);

void ov07_0222314C(SysTask *arg0, void *param) {
    UnkStruct_ov07_0222314C_ptr *ptr = param;
    void *v1;
    u8 v0;

    v0 = ptr->unk_0;
    switch (v0) {
    case 0:
        Sprite_TickFrame(*ptr->unk_14);
        Sprite_TickFrame(*ptr->unk_18);
        Sprite_TickFrame(*ptr->unk_1C);
        Sprite_OffsetPositionXY(*ptr->unk_14, 1, 0);
        Sprite_OffsetPositionXY(*ptr->unk_18, -1, 0);
        Sprite_OffsetPositionXY(*ptr->unk_1C, 1, 1);
        SpriteSystem_DrawSprites(ptr->unk_C);
        return;
    case 1:
        v1 = ptr->unk_4;
        Sprite_DeleteAndFreeResources(ptr->unk_10);
        Heap_Free(ptr);
        ov07_0221C448(v1, arg0);
        return;
    }
}
