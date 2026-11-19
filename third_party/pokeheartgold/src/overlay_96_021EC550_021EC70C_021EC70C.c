#include "global.h"

#include "sprite_system.h"
#include "overlay_96_app_021ed1d0.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
struct ManagedSprite;

struct ManagedSpriteTemplate;

struct SpriteManager;

struct SpriteSystem;

void ov96_021EC70C(Ov96App_021ED1D0 *param0);

void ov96_021EC70C(Ov96App_021ED1D0 *param0) {
    ManagedSpriteTemplate tmpl = { 0 };
    int i;
    int x;
    int id;
    int off;
    ManagedSprite *sprite;
    ManagedSprite **slot;

    x = 0x60;
    for (i = 0; i < 2; i++) {
        id = i + 0x6b;
        tmpl.resIdList[0] = id;
        off = i + 0x18;
        tmpl.resIdList[1] = id;
        tmpl.resIdList[2] = 0x67;
        tmpl.resIdList[3] = 0x67;
        tmpl.vram = (NNS_G2D_VRAM_TYPE)1;
        tmpl.bgPriority = 0;
        tmpl.drawPriority = 2;
        tmpl.x = x;
        tmpl.y = 0x38;
        if (param0->courseId < 5) {
            tmpl.x = 0x80;
        }
        slot = &param0->sprites[off];
        sprite = SpriteSystem_NewSpriteWithYOffset(param0->spriteSystem, param0->spriteManager, &tmpl, 0x20c000);
        *slot = sprite;
        ManagedSprite_SetAnimateFlag(sprite, 1);
        ManagedSprite_SetDrawFlag(*slot, 0);
        x += 0x40;
    }
}
#else
struct ManagedSprite;

struct ManagedSpriteTemplate;

struct SpriteManager;

struct SpriteSystem;

void ov96_021EC70C(u8 *param0);

void ov96_021EC70C(u8 *param0) {
    ManagedSpriteTemplate tmpl = { 0 };
    int i;
    int x;
    int id;
    int off;
    ManagedSprite *sprite;
    u8 *slot;

    x = 0x60;
    for (i = 0; i < 2; i++) {
        id = i + 0x6b;
        tmpl.resIdList[0] = id;
        off = i + 0x18;
        tmpl.resIdList[1] = id;
        tmpl.resIdList[2] = 0x67;
        tmpl.resIdList[3] = 0x67;
        tmpl.vram = (NNS_G2D_VRAM_TYPE)1;
        tmpl.bgPriority = 0;
        tmpl.drawPriority = 2;
        tmpl.x = x;
        tmpl.y = 0x38;
        if (param0[0xb1] < 5) {
            tmpl.x = 0x80;
        }
        slot = param0 + off * 4;
        sprite = SpriteSystem_NewSpriteWithYOffset(*(SpriteSystem **)(param0 + 0x18), *(SpriteManager **)(param0 + 0x1c), &tmpl, 0x20c000);
        *(ManagedSprite **)(slot + 0x20) = sprite;
        ManagedSprite_SetAnimateFlag(sprite, 1);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(slot + 0x20), 0);
        x += 0x40;
    }
}
#endif
