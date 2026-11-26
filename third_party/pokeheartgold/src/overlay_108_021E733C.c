#include "global.h"
struct SafariAreaCustomizer;

#include "sprite_system.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E756C(SafariAreaCustomizer *param_1);

#if defined(SDK_PORT) || defined(NONMATCHING)
ManagedSprite *ov108_021E8540(void *, s32, s32, u8, u8, u8, u8, s32);
#else
ManagedSprite *ov108_021E8540(void *, s32, s32, s32, s32, s32, s32, s32);
#endif

extern UnmanagedSpriteTemplate ov108_021EA7F8;

extern UnmanagedSpriteTemplate ov108_021EA820;

extern UnmanagedSpriteTemplate ov108_021EA848;

extern UnmanagedSpriteTemplate ov108_021EA870;

void ov108_021E733C(SafariAreaCustomizer *arg0);

void ov108_021E7650(SafariAreaCustomizer *arg0);
void ov108_021E8674(ManagedSprite *managedSprite);
void ov108_021E7510(SafariAreaCustomizer *arg0);

void ov108_021E733C(SafariAreaCustomizer *arg0) {
    s32 v0;
    s32 v1;
    s32 v2;
    v1 = 0;
    do {
        v0 = v1 % 3;
        arg0->sprites[v1] = ov108_021E8540(arg0->unk_348, (u16)((v0 * 0x48) + 0x38), (u16)((0x48 * (v1 / 3)) + 0x38), 2, 0x80, (s32)((u8)v1), (s32)(arg0->areaSet.areas[v1].area_no), 1);
        v1 += 1;
    } while (v1 < 6);
    v2 = 0;
    do {
        arg0->sprites[6 + v2] = ov108_021E8540(arg0->unk_348, (u16)((0x50 * (v2 % 3)) + 0x30), (u16)((0x48 * (v2 / 3)) + 0x38), 3, 0x80, (s32)((u8)(v2 + 6)), (s32)((u8)(v2 + (6 * (arg0->page)))), 1);
        v2 += 1;
    } while (v2 < 6);
    arg0->unk_354[0] = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov108_021EA7F8);
    arg0->unk_354[1] = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov108_021EA7F8);
    arg0->unk_354[3] = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov108_021EA7F8);
    arg0->unk_354[2] = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov108_021EA820);
    arg0->unk_364 = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov108_021EA848);
    arg0->unk_368 = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov108_021EA870);
    thunk_Sprite_SetPriority(arg0->unk_354[1], 3);
    thunk_Sprite_SetPriority(arg0->unk_364, 3);
    thunk_Sprite_SetPriority(arg0->unk_368, 3);
    thunk_Sprite_SetPriority(arg0->unk_354[3], 0);
    thunk_Sprite_SetDrawPriority(arg0->unk_354[3], 5U);
    thunk_Sprite_SetDrawFlag(arg0->unk_354[0], 0);
    thunk_Sprite_SetDrawFlag(arg0->unk_354[1], 0);
    thunk_Sprite_SetDrawFlag(arg0->unk_354[3], 0);
    thunk_Sprite_SetDrawFlag(arg0->unk_354[2], 0);
    ov108_021E756C(arg0);
}

void ov108_021E7510(SafariAreaCustomizer *arg0) {
    s32 v0;

    ov108_021E7650(arg0);
    thunk_Sprite_Delete(arg0->unk_354[0]);
    thunk_Sprite_Delete(arg0->unk_354[1]);
    thunk_Sprite_Delete(arg0->unk_354[2]);
    thunk_Sprite_Delete(arg0->unk_354[3]);
    thunk_Sprite_Delete(arg0->unk_364);
    thunk_Sprite_Delete(arg0->unk_368);
    v0 = 0;
    do {
        ov108_021E8674(arg0->sprites[v0]);
        v0 += 1;
    } while (v0 < 0xC);
}
