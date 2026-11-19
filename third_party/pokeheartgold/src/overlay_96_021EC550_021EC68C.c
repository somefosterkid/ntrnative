#include "global.h"

#include "sprite_system.h"
#include "overlay_96_app_021ed1d0.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021EC68C(Ov96App_021ED1D0 *arg0);

void ov96_021EC68C(Ov96App_021ED1D0 *arg0) {
    ManagedSpriteTemplate sp4 = { 0 };
    s32 sp0;
    ManagedSprite *temp_r0;
    s32 var_r4;
    u32 var_r5;

    var_r4 = 0;
    var_r5 = 0x30;
    sp0 = 2;
    do {
        sp4.resIdList[0] = var_r4 + 0x6D;
        sp4.resIdList[1] = 0x6A;
        sp4.resIdList[2] = 0x68;
        sp4.resIdList[3] = 0x68;
        sp4.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        sp4.x = var_r5;
        sp4.y = 0x70;
        sp4.bgPriority = 0;
        sp4.drawPriority = 1;
        arg0->sprites[var_r4 + 21] = SpriteSystem_NewSpriteWithYOffset(arg0->spriteSystem, arg0->spriteManager, &sp4, FX32_CONST(524));
        temp_r0 = arg0->sprites[var_r4 + 21];
        ManagedSprite_SetPaletteOverride(temp_r0, sp0);
        ManagedSprite_SetDrawFlag(arg0->sprites[var_r4 + 21], 0);
        var_r4 += 1;
        var_r5 += 0x50;
        sp0 += 3;
    } while (var_r4 < 3);
}
#else
void ov96_021EC68C(void *arg0);

void ov96_021EC68C(void *arg0) {
    ManagedSpriteTemplate sp4 = { 0 };
    s32 sp0;
    ManagedSprite *temp_r0;
    s32 var_r4;
    u32 var_r5;
    void *temp_r6;

    var_r4 = 0;
    var_r5 = 0x30;
    sp0 = 2;
    do {
        sp4.resIdList[0] = var_r4 + 0x6D;
        sp4.resIdList[1] = 0x6A;
        sp4.resIdList[2] = 0x68;
        sp4.resIdList[3] = 0x68;
        sp4.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        sp4.x = var_r5;
        sp4.y = 0x70;
        sp4.bgPriority = 0;
        sp4.drawPriority = 1;
        temp_r6 = arg0 + ((var_r4 + 0x15) * 4);
        temp_r0 = SpriteSystem_NewSpriteWithYOffset(*(SpriteSystem **)((u8 *)arg0 + 0x18), *(SpriteManager **)((u8 *)arg0 + 0x1C), &sp4, FX32_CONST(524));
        *(ManagedSprite **)((u8 *)temp_r6 + 0x20) = temp_r0;
        ManagedSprite_SetPaletteOverride(temp_r0, sp0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)temp_r6 + 0x20), 0);
        var_r4 += 1;
        var_r5 += 0x50;
        sp0 += 3;
    } while (var_r4 < 3);
}
#endif
