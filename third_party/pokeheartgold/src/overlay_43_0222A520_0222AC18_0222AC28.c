#include "global.h"

#include "sprite.h"
#include "overlay_43_palpad.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
extern SpriteTemplate ov43_0222F14C[2];

extern SpriteTemplate ov43_0222F1AC;

void ov43_0222AC28(PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222AC28(PalPadGfx *gfx, enum HeapID heapID) {
    s32 i;
    s32 seq;
    SpriteTemplate *template;

    template = ov43_0222F14C;
    seq = 5;
    for (i = 0; i < 2; i++) {
        template->spriteList = gfx->spriteList;
        template->header = &gfx->spriteHeader;
        template->heapID = heapID;
        gfx->arrows[i] = Sprite_CreateAffine(template);
        Sprite_SetAnimActiveFlag(gfx->arrows[i], 1);
        Sprite_SetDrawFlag(gfx->arrows[i], 0);
        Sprite_SetAnimCtrlSeq(gfx->arrows[i], seq);
        template++;
        seq += 2;
    }
    ov43_0222F1AC.spriteList = gfx->spriteList;
    ov43_0222F1AC.header = &gfx->spriteHeader;
    ov43_0222F1AC.heapID = heapID;
    gfx->cursor = Sprite_CreateAffine(&ov43_0222F1AC);
    Sprite_SetDrawFlag(gfx->cursor, 0);
    Sprite_SetAnimCtrlSeq(gfx->cursor, 1);
}
#else
extern u32 ov43_0222F14C;

extern u8 ov43_0222F160;

extern u32 ov43_0222F1AC;

void ov43_0222AC28(u32 param_1, u32 param_2);

void ov43_0222AC28(u32 param_1, u32 param_2) {
    Sprite *pSVar1;
    u32 *puVar2;
    s32 iVar3;
    s32 seq;
    SpriteTemplate *template;
    s32 iStack_20;

    iStack_20 = 0;
    puVar2 = (u32 *)&ov43_0222F14C;
    template = (SpriteTemplate *)&ov43_0222F14C;
    seq = 5;
    iVar3 = param_1;
    do {
        *puVar2 = *(u32 *)(param_1 + 4);
        puVar2[1] = param_1 + 0x88;
        puVar2[0xb] = param_2;
        pSVar1 = Sprite_CreateAffine(template);
        *(Sprite **)(iVar3 + 500) = pSVar1;
        Sprite_SetAnimActiveFlag(*(Sprite **)(iVar3 + 500), 1);
        Sprite_SetDrawFlag(*(Sprite **)(iVar3 + 500), 0);
        Sprite_SetAnimCtrlSeq(*(Sprite **)(iVar3 + 500), seq);
        puVar2 = ((void *)puVar2) + 0x30;
        template = ((void *)template) + 0x30;
        iVar3 = iVar3 + 4;
        seq = seq + 2;
        iStack_20 = iStack_20 + 1;
    } while (iStack_20 < 2);
    ((u32 *)&ov43_0222F160)[0x13] = *(u32 *)(param_1 + 4);
    ((s32 *)&ov43_0222F160)[0x14] = param_1 + 0x88;
    ((u32 *)&ov43_0222F160)[0x1e] = param_2;
    pSVar1 = Sprite_CreateAffine((SpriteTemplate *)&ov43_0222F1AC);
    *(Sprite **)(param_1 + 0x1fc) = pSVar1;
    Sprite_SetDrawFlag(*(Sprite **)(param_1 + 0x1fc), 0);
    Sprite_SetAnimCtrlSeq(*(Sprite **)(param_1 + 0x1fc), 1);
    return;
}
#endif
