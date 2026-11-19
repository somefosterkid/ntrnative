#include "global.h"

#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_113.h"

extern UnmanagedSpriteTemplate ov113_021E6CF8;

void ov113_021E69CC(UnownReportData *arg0);

void ov113_021E69CC(UnownReportData *arg0) {
    ManagedSpriteTemplate sp4;
    s32 sp0;
    UnmanagedSpriteTemplate *var_r6;
    u32 var_r7_2;
    s32 var_r4_2;
    s32 var_r7;
    struct ManagedSprite **var_r6_2;

    var_r6 = &ov113_021E6CF8;
    var_r7 = 0;
    do {
        arg0->sprites[var_r7] = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager1, var_r6);
        Sprite_SetDrawFlag(arg0->sprites[var_r7], 1);
        Sprite_SetAnimActiveFlag(arg0->sprites[var_r7], 1);
        Sprite_SetPriority(arg0->sprites[var_r7], 2U);
        var_r6 = (void *)((u8 *)var_r6 + (0x28));
        var_r7 += 1;
    } while (var_r7 < 3);
    MI_CpuFill8(&sp4, 0U, 0x34U);
    sp4.bgPriority = 1;
    sp4.pal = (s32)GetMonIconPaletteEx(0xC9U, 0U, 0U);
    var_r4_2 = 0;
    sp4.vramTransfer = 0;
    sp4.x = 0x28;
    sp4.resIdList[1] = 1;
    sp4.resIdList[2] = 1;
    sp4.resIdList[3] = 1;
    sp4.resIdList[4] = -1;
    sp4.resIdList[5] = -1;
    var_r7_2 = 0x1D;
    sp0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    var_r6_2 = arg0->monIcons;
#else
    var_r6_2 = (struct ManagedSprite **)arg0;
#endif
    do {
        if (var_r4_2 < 7) {
            sp4.y = var_r7_2;
            sp4.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        } else {
            sp4.y = sp0 - 0x9B;
            sp4.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        }
        sp4.resIdList[0] = var_r4_2 + 1;
#if defined(SDK_PORT) || defined(NONMATCHING)
        *var_r6_2 = SpriteSystem_NewSprite(arg0->spriteSystem, arg0->spriteManager2, &sp4);
#else
        *(ManagedSprite **)((u8 *)var_r6_2 + offsetof(UnownReportData, monIcons)) = SpriteSystem_NewSprite(arg0->spriteSystem, arg0->spriteManager2, &sp4);
#endif
#if defined(SDK_PORT) || defined(NONMATCHING)
        ManagedSprite_SetDrawFlag(*var_r6_2, 0);
#else
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)var_r6_2 + offsetof(UnownReportData, monIcons)), 0);
#endif
        var_r4_2 += 1;
        var_r7_2 += 0x18;
        var_r6_2 += 1;
        sp0 += 0x18;
    } while (var_r4_2 < 0xE);
}
