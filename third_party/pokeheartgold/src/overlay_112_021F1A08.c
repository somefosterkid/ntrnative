#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

void ov112_021F1FD0(Ov112SpriteApp *arg0);

void ov112_021F1A08(Ov112SpriteApp *arg0);

void ov112_021F1A40(Ov112SpriteApp *arg0);

void ov112_021F1A08(Ov112SpriteApp *arg0) {
    SpriteManager *spriteManager;
    SpriteSystem *spriteSystem;
    s32 v0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    spriteSystem = arg0->spriteSystem;
    spriteManager = arg0->spriteManager;
    ov112_021F1FD0(arg0);
    for (v0 = 0; v0 < 0xA; v0++) {
        if (arg0->sprites[v0] == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(arg0->sprites[v0]);
    }
#else
    struct SpriteSystem **v1;

    v1 = (struct SpriteSystem **)arg0;
    spriteSystem = *(SpriteSystem **)((u8 *)v1 + offsetof(Ov112SpriteApp, spriteSystem));
    spriteManager = *(SpriteManager **)((u8 *)v1 + offsetof(Ov112SpriteApp, spriteManager));
    ov112_021F1FD0(arg0);
    v0 = 0;
    do {
        if (*(ManagedSprite **)((u8 *)v1 + offsetof(Ov112SpriteApp, sprites[0])) == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)v1 + offsetof(Ov112SpriteApp, sprites[0])));
        v0 += 1;
        v1 += 1;
    } while (v0 < 0xA);
#endif
    SpriteSystem_FreeResourcesAndManager(spriteSystem, spriteManager);
    SpriteSystem_Free(spriteSystem);
}

void ov112_021F1A40(Ov112SpriteApp *arg0) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;

    spriteSystem = arg0->spriteSystem;
    spriteManager = arg0->spriteManager;
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xC, 0, 0xC, 1, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xD, 1, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xF, 1, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xE, 1, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x10, 0, 1, 1, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x11, 1, 1, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x13, 1, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x12, 1, 1);
}
