#include "global.h"
struct SafariAreaCustomizer;

#include "sprite_system.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_108_safari_area_customizer.h"
#include "overlay_108_safari_area_customizer.h"

extern void *_021EA720;

s32 ov108_021E82E0(SafariAreaCustomizer *arg0, s32 arg1);

extern OamCharTransferParam ov108_021EA98C;
extern OamManagerParam ov108_021EA9A0;
void ov108_021E838C(SafariAreaCustomizer *arg0);
void ov108_021E83C8(SafariAreaCustomizer *arg0);
s32 ov108_021E853C(void *, SpriteList *);
extern ResdatIdList ov108_021EA97C;
void ov108_021E83F0(SafariAreaCustomizer *arg0);
void ov108_021E844C(SafariAreaCustomizer *arg0);
void ov108_021E852C(void *arg0);
void ov108_021E846C(SafariAreaCustomizer *arg0);

s32 ov108_021E82E0(SafariAreaCustomizer *arg0, s32 arg1) {
    u8 v2[4];
    void *v1;
    s32 v0;
    s32 v3;
    s32 v6;
    u8 *v4;
    u8 *v5;
    v0 = arg1;
    v3 = 6 * arg0->unk_184E2_2;
    v2[2] = (s8)((arg0->areaIdx) + v3);
    v2[3] = (s8)((arg0->cursor) + v3);
    v2[0] = (u8)(((s32)(arg0->areaIdx)) / 3);
    v2[1] = (s8)(((s32)(arg0->cursor)) / 3);
    v6 = 0;
    v4 = &v2[0];
    v5 = &v2[2];
    v1 = &_021EA720;
    v1 = v1 + (v0 * 2);
    do {
        ManagedSprite_OffsetPositionXY(arg0->sprites[(*v5)], 0, (s16)(0xC * (*((s8 *)(v1 + (*v4))))));
        v4 += 1;
        v5 += 1;
        v6 += 1;
    } while (v6 < 2);
    arg0->unk_184E1 = (u8)((arg0->unk_184E1) + 1);
    if (((u32)(arg0->unk_184E1)) >= 8U) {
        arg0->unk_184E1 = 0U;
        return 1;
    }
    return 0;
}

void ov108_021E838C(SafariAreaCustomizer *arg0) {
    GF_CreateVramTransferManager(0x20U, arg0->heapID);
    arg0->spriteSystem = SpriteSystem_Alloc(arg0->heapID);
    SpriteSystem_Init(arg0->spriteSystem, &ov108_021EA9A0, &ov108_021EA98C, 4);
    thunk_ClearMainOAM(arg0->heapID);
    thunk_ClearSubOAM(arg0->heapID);
}

void ov108_021E83C8(SafariAreaCustomizer *arg0) {
    SpriteSystem_Free(arg0->spriteSystem);
    arg0->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(arg0->heapID);
    thunk_ClearSubOAM(arg0->heapID);
}

void ov108_021E83F0(SafariAreaCustomizer *arg0) {
    SpriteSystem *spriteSystem;

    spriteSystem = arg0->spriteSystem;
    if (spriteSystem != NULL) {
        arg0->spriteManager = SpriteManager_New(spriteSystem);
        SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 0x8C);
        sub_0200D2A4(arg0->spriteSystem, arg0->spriteManager, &ov108_021EA97C, 2, 0);
        ov108_021E853C(arg0->unk_348, SpriteManager_GetSpriteList(arg0->spriteManager));
    }
}

void ov108_021E844C(SafariAreaCustomizer *arg0) {
    SpriteManager *spriteManager;

    spriteManager = arg0->spriteManager;
    if (spriteManager != NULL) {
        SpriteSystem_DestroySpriteManager(arg0->spriteSystem, spriteManager);
        arg0->spriteManager = NULL;
    }
}

void ov108_021E846C(SafariAreaCustomizer *arg0) {
    SpriteManager *spriteManager;

    if (arg0->unk_348 != NULL) {
        ov108_021E852C(arg0->unk_348);
    }
    spriteManager = arg0->spriteManager;
    if (spriteManager != NULL) {
        SpriteSystem_DrawSprites(spriteManager);
    }
}
