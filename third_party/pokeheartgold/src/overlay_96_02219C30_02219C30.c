#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_digit_work.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
ManagedSprite *ov96_022199A8(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u8 param_4, u16 param_5, u16 param_6);
#else
ManagedSprite *ov96_022199A8(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u32 param_4, u16 param_5, u16 param_6);
#endif
void ov96_02219DA8(PokeathlonCourseData *param_1, enum HeapID param_2, ManagedSprite *param_3, s32 param_4, u8 param_5);
void ov96_02219DD0(Ov96DigitWork *work, s32 param_2, s32 param_3);
void ov96_02219E60(Ov96DigitWork *work);

void ov96_02219C30(Ov96DigitWork *work);

void ov96_02219C30(Ov96DigitWork *work) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    ManagedSprite *pMVar1;
    s32 iVar2;
    s32 sVar4;
    s16 px;
    ManagedSpriteTemplate MStack_4c = { 0 };

    pMVar1 = ov96_022199A8(work->spriteSystem, work->spriteManager, 0x86, 0xa0, work->playerIdx + 0xc, 6);
    work->sprites[0] = pMVar1;
    ManagedSprite_SetPaletteOverride(pMVar1, 1);
    iVar2 = 0;
    sVar4 = 0x78;
    do {
        px = sVar4;
        pMVar1 = ov96_022199A8(work->spriteSystem, work->spriteManager, px, 0x24, iVar2 + 1, 5);
        work->sprites[iVar2 + 2] = pMVar1;
        ov96_02219DD0(work, (u8)iVar2, 0);
        pMVar1 = ov96_022199A8(work->spriteSystem, work->spriteManager, px, 0x24, 10, 4);
        work->sprites[iVar2 + 6] = pMVar1;
        ManagedSprite_SetDrawFlag(pMVar1, 0);
        pMVar1 = ov96_022199A8(work->spriteSystem, work->spriteManager, (s16)sVar4, 0x18, 6, 0);
        work->sprites[iVar2 + 10] = pMVar1;
        ManagedSprite_SetDrawFlag(work->sprites[iVar2 + 10], 0);
        sVar4 = sVar4 + 0x20;
        iVar2 = iVar2 + 1;
    } while ((s32)iVar2 < 4);
    iVar2 = 0;
    sVar4 = 0x30;
    do {
        MStack_4c.resIdList[0] = iVar2 + 0x2712;
        MStack_4c.resIdList[1] = iVar2 + 0x2712;
        MStack_4c.resIdList[2] = 0x2712;
        MStack_4c.resIdList[3] = 0x2712;
        MStack_4c.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        MStack_4c.x = 0x28;
        MStack_4c.y = sVar4;
        MStack_4c.drawPriority = iVar2 + 2;
        MStack_4c.bgPriority = 2;
        pMVar1 = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &MStack_4c, FX32_CONST(480));
        work->sprites[iVar2 + 14] = pMVar1;
        ManagedSprite_SetAnimateFlag(work->sprites[iVar2 + 14], 1);
        iVar2 = iVar2 + 1;
        sVar4 = sVar4 + 0x58;
    } while (iVar2 < 2);
    iVar2 = 0;
    sVar4 = 0x18;
    do {
        ov96_02219DA8(work->course, work->heapId, work->sprites[iVar2 + 14], (u32)work->playerIdx, 2 - iVar2);
        pMVar1 = ov96_022199A8(work->spriteSystem, work->spriteManager, 0x28, (s16)sVar4, 5, 1);
        work->sprites[iVar2 + 16] = pMVar1;
        ManagedSprite_SetDrawFlag(work->sprites[iVar2 + 16], 0);
        iVar2 = iVar2 + 1;
        sVar4 = sVar4 + 0x58;
    } while (iVar2 < 2);
    ov96_02219E60(work);
    return;
#else
    u32 *param_1 = (u32 *)work;
    ManagedSprite *pMVar1;
    s32 iVar2;
    u32 *puVar3;
    s32 sVar4;
    s16 px;
    ManagedSpriteTemplate MStack_4c = { 0 };

    pMVar1 = ov96_022199A8((SpriteSystem *)param_1[2], (SpriteManager *)param_1[3], 0x86, 0xa0, *(u8 *)((u8 *)param_1 + 0x22) + 0xc, 6);
    param_1[0x18] = (u32)(pMVar1);
    ManagedSprite_SetPaletteOverride(pMVar1, 1);
    iVar2 = 0;
    sVar4 = 0x78;
    puVar3 = param_1;
    do {
        px = sVar4;
        pMVar1 = ov96_022199A8((SpriteSystem *)param_1[2], (SpriteManager *)param_1[3], px, 0x24, iVar2 + 1, 5);
        puVar3[0x1a] = (u32)(pMVar1);
        ov96_02219DD0(work, (u8)iVar2, 0);
        pMVar1 = ov96_022199A8((SpriteSystem *)param_1[2], (SpriteManager *)param_1[3], px, 0x24, 10, 4);
        puVar3[0x1e] = (u32)(pMVar1);
        ManagedSprite_SetDrawFlag(pMVar1, 0);
        pMVar1 = ov96_022199A8((SpriteSystem *)param_1[2], (SpriteManager *)param_1[3], (s16)sVar4, 0x18, 6, 0);
        puVar3[0x22] = (u32)(pMVar1);
        ManagedSprite_SetDrawFlag((ManagedSprite *)puVar3[0x22], 0);
        sVar4 = sVar4 + 0x20;
        puVar3 = puVar3 + 1;
        iVar2 = iVar2 + 1;
    } while ((s32)iVar2 < 4);
    iVar2 = 0;
    sVar4 = 0x30;
    puVar3 = param_1;
    do {
        MStack_4c.resIdList[0] = iVar2 + 0x2712;
        MStack_4c.resIdList[1] = iVar2 + 0x2712;
        MStack_4c.resIdList[2] = 0x2712;
        MStack_4c.resIdList[3] = 0x2712;
        MStack_4c.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        MStack_4c.x = 0x28;
        MStack_4c.y = sVar4;
        MStack_4c.drawPriority = iVar2 + 2;
        MStack_4c.bgPriority = 2;
        pMVar1 = SpriteSystem_NewSpriteWithYOffset((SpriteSystem *)param_1[2], (SpriteManager *)param_1[3], &MStack_4c, FX32_CONST(480));
        puVar3[0x26] = (u32)(pMVar1);
        ManagedSprite_SetAnimateFlag((ManagedSprite *)puVar3[0x26], 1);
        iVar2 = iVar2 + 1;
        sVar4 = sVar4 + 0x58;
        puVar3 = puVar3 + 1;
    } while (iVar2 < 2);
    iVar2 = 0;
    puVar3 = param_1;
    sVar4 = 0x18;
    do {
        ov96_02219DA8((PokeathlonCourseData *)*param_1, (enum HeapID)param_1[1], (ManagedSprite *)puVar3[0x26], (u32)((u8 *)param_1)[0x22], 2 - iVar2);
        pMVar1 = ov96_022199A8((SpriteSystem *)param_1[2], (SpriteManager *)param_1[3], 0x28, (s16)sVar4, 5, 1);
        puVar3[0x28] = (u32)(pMVar1);
        ManagedSprite_SetDrawFlag((ManagedSprite *)puVar3[0x28], 0);
        iVar2 = iVar2 + 1;
        puVar3 = puVar3 + 1;
        sVar4 = sVar4 + 0x58;
    } while (iVar2 < 2);
    ov96_02219E60(work);
    return;
#endif
}
