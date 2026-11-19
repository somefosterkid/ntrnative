#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "sprite_system.h"
#include "overlay_98.h"
#include "overlay_99_obj99.h"

extern u8 ov99_021E9D88;

extern ManagedSpriteTemplate ov99_021E9E2C;

void ov99_021E6A70(Obj99 *param_1) {
    u32 uVar1;

    NARC_Delete(param_1->narc);
    uVar1 = 0;
    do {
        FreeBgTilemapBuffer(param_1->bgConfig, (&ov99_021E9D88)[uVar1]);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 4);
    Heap_Free(param_1->bgConfig);
    return;
}

void ov99_021E6A9C(Obj99 *arg0) {
    SpriteManager *spriteManager;
    SpriteSystem *spriteSystem;

    spriteSystem = ov98_0221E5C0(arg0->spriteContext);
    spriteManager = ov98_0221E5D0(arg0->spriteContext);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x11, 1, 1, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x10, 0, 2, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x12, 1, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x13, 1, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 9, 1, 2, 4);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 8, 0, 1, 2, 4);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 9, 1, 1, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 8, 0, 1, 1, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xA, 1, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xB, 1, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 1, 1, 1, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0, 0, 1, 1, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 1, 1, 2, 3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0, 0, 1, 2, 3);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 2, 1, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 3, 1, 2);
}

void ov99_021E6C14(Obj99 *arg0) {
    ManagedSprite *managedSprite;

    managedSprite = ov98_0221E6E0(arg0->spriteContext, &ov99_021E9E2C);
    arg0->sprites[0] = managedSprite;
    ManagedSprite_SetAnimateFlag(managedSprite, 1);
}
