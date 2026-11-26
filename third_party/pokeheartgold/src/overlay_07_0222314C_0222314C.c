#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"

// the layout of SpriteExampleContext (overlay_07_022231BC.c); unk_14 / unk_18 / unk_1C are sprites[0..2]
typedef struct UnkStruct_ov07_0222314C_ptr {
    u8 state;
    BattleAnimSystem *battleAnimSystem;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMgr;
    ManagedSprite *mainSprite;
    ManagedSprite *sprites[3];
} UnkStruct_ov07_0222314C_ptr;

void ov07_0222314C(SysTask *arg0, void *param);

void ov07_0222314C(SysTask *arg0, void *param) {
    UnkStruct_ov07_0222314C_ptr *ptr = param;
    void *v1;
    u8 v0;

    v0 = ptr->state;
    switch (v0) {
    case 0:
        Sprite_TickFrame(*(Sprite **)ptr->sprites[0]);
        Sprite_TickFrame(*(Sprite **)ptr->sprites[1]);
        Sprite_TickFrame(*(Sprite **)ptr->sprites[2]);
        Sprite_OffsetPositionXY(*(Sprite **)ptr->sprites[0], 1, 0);
        Sprite_OffsetPositionXY(*(Sprite **)ptr->sprites[1], -1, 0);
        Sprite_OffsetPositionXY(*(Sprite **)ptr->sprites[2], 1, 1);
        SpriteSystem_DrawSprites(ptr->spriteMgr);
        return;
    case 1:
        v1 = ptr->battleAnimSystem;
        Sprite_DeleteAndFreeResources(ptr->mainSprite);
        Heap_Free(ptr);
        ov07_0221C448(v1, arg0);
        return;
    }
}
