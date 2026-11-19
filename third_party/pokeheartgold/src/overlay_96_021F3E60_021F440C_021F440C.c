#include "global.h"

#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_96_ov96_sprite_mgr.h"

extern u8 ov96_0221BFF4;

void ov96_021F440C(void *param_1, u8 *param_2, u32 param_3, u32 param_4);

void ov96_021F440C(void *param_1, u8 *param_2, u32 param_3, u32 param_4) {
    u32 *puVar1;
    SpriteResource *charResObj;
    SpriteResource *plttResObj;
    NNSG2dImageProxy *imageProxy;
    NNSG2dImagePaletteProxy *pImg;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    u32 auStack_20[2];

    auStack_20[0] = ((u32 *)&ov96_0221BFF4)[8];
    auStack_20[1] = ((u32 *)&ov96_0221BFF4)[9];
    uVar3 = 0;
    do {
        uVar4 = auStack_20[uVar3];
        puVar1 = (u32 *)ov96_021EB5EC(param_1, uVar4, 0);
        charResObj = (SpriteResource *)*puVar1;
        puVar1 = (u32 *)ov96_021EB5EC(param_1, uVar4, 1);
        plttResObj = (SpriteResource *)*puVar1;
        imageProxy = SpriteTransfer_GetCharProxy(charResObj);
        pImg = SpriteTransfer_GetPaletteProxy(plttResObj, imageProxy);
        uVar2 = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        *(u32 *)(param_2 + uVar3 * 4 + 0x88) = uVar2;
        uVar2 = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DSUB);
        *(u32 *)(param_2 + uVar3 * 4 + 0x90) = uVar2;
        uVar3 = uVar3 + 1 & 0xff;
    } while (uVar3 < 2);
    return;
}
