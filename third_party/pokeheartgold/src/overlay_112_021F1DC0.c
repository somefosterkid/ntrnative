#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

u32 ov112_021F1DC0(Ov112SpriteApp *app);

void ov112_021F1EFC(Ov112SpriteRec *rec, s32 flag);
void ov112_021F1F3C(Ov112SpriteRec *rec, s32 seqno);
void ov112_021F1F54(Ov112SpriteRec *rec, s16 x, s16 y, s32 arg3);
void ov112_021F2000(Ov112SpriteRec *rec, SpriteSystem *, SpriteManager *, s16 x, s16 y, u8 anim, int resBase);
void ov112_021F1F80(Ov112SpriteApp *app);
void ov112_021F1FD0(Ov112SpriteApp *app);

u32 ov112_021F1DC0(Ov112SpriteApp *app) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;

    spriteSystem = app->spriteSystem;
    spriteManager = app->spriteManager;
    switch (app->loadStep) {
    case 0:
        SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0xc, 0, 1, 1, 2);
        SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0xc, 0, 1, 1, 3);
        app->loadStep++;
        break;
    case 1:
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x14, 1, 1, 2);
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x14, 1, 1, 3);
        SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x15, 1, 2);
        SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x16, 1, 2);
        app->loadStep++;
        break;
    case 2:
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x17, 1, 1, 4);
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x17, 1, 1, 5);
        SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x18, 1, 3);
        SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, app->narc, 0x19, 1, 3);
        app->loadStep++;
        break;
    case 3:
        return 1;
    default:
        GF_AssertFail();
    }
    return 0;
}

void ov112_021F1EFC(Ov112SpriteRec *rec, s32 flag) {
    s32 v0;

    v0 = flag;
    if (v0 == 0) {
        ManagedSprite_SetDrawFlag(rec->sprites[1], flag);
        ManagedSprite_SetDrawFlag(rec->sprites[0], v0);
    } else if (rec->unk_0 != 0) {
        ManagedSprite_SetDrawFlag(rec->sprites[1], flag);
    } else {
        ManagedSprite_SetDrawFlag(rec->sprites[0], flag);
    }
    if ((u32)(rec->unk_4 - 0x32) <= 1U) {
        v0 = 0;
    }
    ManagedSprite_SetDrawFlag(rec->sprites[2], v0);
}

void ov112_021F1F3C(Ov112SpriteRec *rec, s32 seqno) {
    ManagedSprite_SetAnimNoRestart(rec->sprites[1], seqno);
    ManagedSprite_SetAnimNoRestart(rec->sprites[0], seqno);
}

void ov112_021F1F54(Ov112SpriteRec *rec, s16 x, s16 y, s32 arg3) {
    ManagedSprite_SetPositionXY(rec->sprites[1], x, y);
    ManagedSprite_SetPositionXY(rec->sprites[0], x, y);
    if (arg3 == 0) {
        ManagedSprite_SetPositionXY(rec->sprites[2], x, y);
    }
}

void ov112_021F1F80(Ov112SpriteApp *app) {
    ov112_021F2000(&app->spriteRecs[0], app->spriteSystem, app->spriteManager, 0x68, 0x48, 0, 0);
    ov112_021F1F3C(&app->spriteRecs[0], 3);
    ov112_021F2000(&app->spriteRecs[1], app->spriteSystem, app->spriteManager, 0x98, 0x48, 0, 1);
    ov112_021F1F3C(&app->spriteRecs[1], 1);
}

void ov112_021F1FD0(Ov112SpriteApp *app) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            Sprite_DeleteAndFreeResources(app->spriteRecs[i].sprites[j]);
        }
        Sprite_DeleteAndFreeResources(app->spriteRecs[i].sprites[2]);
    }
#else
    s32 v0;
    s32 v3;
    struct ManagedSprite **v1;
    void *v2;

    v2 = app;
    v3 = 0;
    do {
        v0 = 0;
        v1 = (struct ManagedSprite **)v2;
loop_2:
        Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)v1 + offsetof(Ov112SpriteApp, spriteRecs[0].sprites[0])));
        v0 += 1;
        v1 += 1;
        if (v0 < 2) {
            goto loop_2;
        }
        Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)v2 + offsetof(Ov112SpriteApp, spriteRecs[0].sprites[2])));
        v3 += 1;
        v2 += sizeof(Ov112SpriteRec);
    } while (v3 < 2);
#endif
}
