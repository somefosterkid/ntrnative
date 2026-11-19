#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_99_obj99.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
ManagedSprite *ov98_0221E6E0(struct PokeathlonSpriteContext *, ManagedSpriteTemplate *);
#else
ManagedSprite *ov98_0221E6E0(void *, u32 *);
#endif

u32 ov98_0221F120(u32, u32);

u8 ov98_0221F150(u32);

extern u32 ov99_021E9DBC;

extern u32 ov99_021E9DD4;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov99_021E6C30(Obj99 *param_1, u32 param_2, s32 param_3, u32 param_4, s16 param_5, u16 param_6, u8 param_7, s32 param_8) {
    int new_var2;
    u16 uVar1;
    s32 iVar2;
    u8 *src;
    ManagedSprite *managedSprite;
    u32 uVar3;
    ManagedSprite **iVar4;
    s32 sVar5;
    ManagedSpriteTemplate tmpl = { 0 };

    iVar2 = ov98_0221F150(param_2);
    uVar3 = 0;
    new_var2 = 0;
    if (param_4 > new_var2) {
        s32 p5;
        u32 limit;
        u32 bStep;
        bStep = param_7;
        sVar5 = new_var2;
        iVar4 = &param_1->sprites[param_3];
        p5 = param_5;
        limit = param_4 - iVar2;
        do {
            uVar1 = ov98_0221F120(param_2, (u8)(param_4 - uVar3));
            tmpl.animation = uVar1;
            if (param_8 != new_var2) {
                src = (unsigned char *)(&ov99_021E9DBC);
            } else {
                src = (unsigned char *)(&ov99_021E9DD4);
            }
            MI_CpuCopy8(src, tmpl.resIdList, 0x18);
            tmpl.vram = (param_8 != new_var2) ? NNS_G2D_VRAM_TYPE_2DMAIN : NNS_G2D_VRAM_TYPE_2DSUB;
            tmpl.x = (u16)(p5 + sVar5);
            tmpl.y = param_6;
            managedSprite = ov98_0221E6E0(param_1->spriteContext, &tmpl);
            *iVar4 = managedSprite;
            ManagedSprite_SetAnimateFlag(managedSprite, 1);
            if (limit > uVar3) {
                ManagedSprite_SetDrawFlag(*iVar4, new_var2);
            }
            uVar3 = uVar3 + 1;
            sVar5 = sVar5 + bStep;
            iVar4 = iVar4 + 1;
        } while (uVar3 < param_4);
    }
    return;
}
#else
void ov99_021E6C30(Obj99 *param_1, u32 param_2, s32 param_3, u32 param_4, s16 param_5, u16 param_6, u8 param_7, s32 param_8) {
    int new_var2;
    u16 uVar1;
    s32 iVar2;
    u8 *src;
    ManagedSprite *managedSprite;
    u32 uVar3;
    u8 *iVar4;
    s32 sVar5;
    u32 uStack_4c[13] = { 0 };

    iVar2 = ov98_0221F150(param_2);
    uVar3 = 0;
    new_var2 = 0;
    if (param_4 > new_var2) {
        s32 p5;
        u32 limit;
        u32 bStep;
        bStep = param_7;
        sVar5 = new_var2;
        iVar4 = (u8 *)param_1 + (param_3 * 4);
        p5 = param_5;
        limit = param_4 - iVar2;
        do {
            uVar1 = ov98_0221F120(param_2, (u8)(param_4 - uVar3));
            ((u16 *)uStack_4c)[3] = uVar1;
            if (param_8 != new_var2) {
                src = (unsigned char *)(&ov99_021E9DBC);
            } else {
                src = (unsigned char *)(&ov99_021E9DD4);
            }
            MI_CpuCopy8(src, &uStack_4c[5], 0x18);
            uStack_4c[4] = (param_8 != new_var2) ? 1 : 2;
            ((u16 *)uStack_4c)[new_var2] = (u16)(p5 + sVar5);
            ((u16 *)uStack_4c)[1] = param_6;
            managedSprite = ov98_0221E6E0(param_1->spriteContext, uStack_4c);
            *((ManagedSprite **)(iVar4 + 0x18)) = managedSprite;
            ManagedSprite_SetAnimateFlag(managedSprite, 1);
            if (limit > uVar3) {
                ManagedSprite_SetDrawFlag(*((ManagedSprite **)(iVar4 + 0x18)), new_var2);
            }
            uVar3 = uVar3 + 1;
            sVar5 = sVar5 + bStep;
            iVar4 = iVar4 + 4;
        } while (uVar3 < param_4);
    }
    return;
}
#endif
