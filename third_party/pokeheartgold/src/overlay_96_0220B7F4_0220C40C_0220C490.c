#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"

void ov96_0220C490(u32 *param_1, SpriteSystem *param_2, SpriteManager *param_3);

void ov96_0220C490(u32 *param_1, SpriteSystem *param_2, SpriteManager *param_3) {
    ManagedSprite *pMVar1;
    s32 iVar2;
    u32 sVar3;
    ManagedSpriteTemplate tmpl = { 0 };

    if (param_2 == NULL) {
        GF_AssertFail();
    }
    if (param_3 == NULL) {
        GF_AssertFail();
    }
    tmpl.resIdList[0] = 0x2712;
    tmpl.resIdList[1] = tmpl.resIdList[0] + 1;
    tmpl.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    tmpl.resIdList[2] = 0x2712;
    tmpl.resIdList[3] = 0x2712;
    tmpl.x = 0x88;
    tmpl.y = 0x70;
    tmpl.drawPriority = 0x1b;
    tmpl.bgPriority = 2;

    pMVar1 = SpriteSystem_NewSpriteWithYOffset(param_2, param_3, &tmpl, FX32_CONST(256));
    *param_1 = (u32)(pMVar1);
    ManagedSprite_SetAnimateFlag(pMVar1, 1);
    iVar2 = 0;
    sVar3 = 0x30;
    do {
        tmpl.resIdList[0] = iVar2 + 0x2713;
        tmpl.resIdList[1] = iVar2 + 0x2714;
        tmpl.resIdList[2] = 0x2712;
        tmpl.resIdList[3] = 0x2712;
        tmpl.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        tmpl.x = 0x28;
        tmpl.y = sVar3;
        tmpl.drawPriority = iVar2 + 1;
        pMVar1 = SpriteSystem_NewSpriteWithYOffset(param_2, param_3, &tmpl, FX32_CONST(256));
        param_1[5] = (u32)(pMVar1);
        ManagedSprite_SetAnimateFlag(pMVar1, 1);
        iVar2 = iVar2 + 1;
        sVar3 = sVar3 + 0x58;
        param_1 = param_1 + 1;
    } while (iVar2 < 2);
}
