#include "global.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0203A3B0.h"
#include "yes_no_prompt.h"
#include "overlay_72_wfc_connect.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov72_022383DC(SpriteTemplate *, NintendoWfcConnect *, SpriteResourcesHeader *, u32);
#else
void ov72_022383DC(SpriteTemplate *, void *, u8 *, u32);
#endif

extern u32 ov72_0223B338;

extern u32 ov72_0223B364;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov72_02238408(NintendoWfcConnect *param_1);
#else
void ov72_02238408(u8 *param_1);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov72_02238408(NintendoWfcConnect *param_1) {
    Sprite *pSVar1;
    u16 *puVar2;
    s32 iVar4;
    SpriteTemplate SStack_44;

    CreateSpriteResourcesHeader(&param_1->spriteHeaders[0], 0, 0, 0, 0, -1, -1, 0, 0, param_1->resMan[0], param_1->resMan[1], param_1->resMan[2], param_1->resMan[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    CreateSpriteResourcesHeader(&param_1->spriteHeaders[1], 1, 1, 1, 1, -1, -1, 0, 0, param_1->resMan[0], param_1->resMan[1], param_1->resMan[2], param_1->resMan[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    CreateSpriteResourcesHeader(&param_1->spriteHeaders[2], 2, 2, 2, 2, -1, -1, 0, 0, param_1->resMan[0], param_1->resMan[1], param_1->resMan[2], param_1->resMan[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    ov72_022383DC(&SStack_44, param_1, &param_1->spriteHeaders[0], 2);
    puVar2 = (u16 *)&ov72_0223B364;
    iVar4 = 0;
    do {
        SStack_44.position.x = (u32)*puVar2 << 0xc;
        SStack_44.position.y = (u32)puVar2[1] * 0x1000 + 0x100000;
        pSVar1 = Sprite_CreateAffine(&SStack_44);
        param_1->sprites[iVar4] = pSVar1;
        Sprite_SetAnimActiveFlag(param_1->sprites[iVar4], 1);
        Sprite_SetAnimCtrlSeq(param_1->sprites[iVar4], 0);
        Sprite_SetDrawFlag(param_1->sprites[iVar4], 0);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
    } while (iVar4 < 3);
    ov72_022383DC(&SStack_44, param_1, &param_1->spriteHeaders[1], 2);
    SStack_44.position.x = 0x20000;
    SStack_44.position.y = 0x151000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    param_1->sprites[3] = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->sprites[3], 1);
    Sprite_SetAnimCtrlSeq(param_1->sprites[3], 1);
    Sprite_SetDrawFlag(param_1->sprites[3], 0);
    SStack_44.position.x = 0x60000;
    SStack_44.position.y = 0x129000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    param_1->sprites[4] = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->sprites[4], 1);
    Sprite_SetAnimCtrlSeq(param_1->sprites[4], 0);
    Sprite_SetDrawFlag(param_1->sprites[4], 0);
    puVar2 = (u16 *)&ov72_0223B338;
    iVar4 = 0;
    do {
        SStack_44.position.x = (u32)*puVar2 << 0xc;
        SStack_44.position.y = (u32)puVar2[1] * 0x1000 + 0x100000;
        pSVar1 = Sprite_CreateAffine(&SStack_44);
        param_1->sprites[iVar4 + 5] = pSVar1;
        Sprite_SetAnimActiveFlag(param_1->sprites[iVar4 + 5], 1);
        Sprite_SetAnimCtrlSeq(param_1->sprites[iVar4 + 5], 3);
        Sprite_SetPriority(param_1->sprites[iVar4 + 5], 1);
        Sprite_SetDrawFlag(param_1->sprites[iVar4 + 5], 0);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
    } while (iVar4 < 3);
    ov72_022383DC(&SStack_44, param_1, &param_1->spriteHeaders[2], 2);
    SStack_44.position.x = 0x80000;
    SStack_44.position.y = 0x151000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    param_1->sprites[8] = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->sprites[8], 1);
    Sprite_SetAnimCtrlSeq(param_1->sprites[8], 1);
    Sprite_SetDrawFlag(param_1->sprites[8], 0);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    sub_0203A880();
    return;
}
#else
void ov72_02238408(u8 *param_1) {
    Sprite *pSVar1;
    u16 *puVar2;
    u8 *iVar3;
    s32 iVar4;
    SpriteTemplate SStack_44;

    CreateSpriteResourcesHeader((SpriteResourcesHeader *)(param_1 + 0xd64), 0, 0, 0, 0, -1, -1, 0, 0, *(GF_2DGfxResMan **)(param_1 + 0xd24), *(GF_2DGfxResMan **)(param_1 + 0xd28), *(GF_2DGfxResMan **)(param_1 + 0xd2c), *(GF_2DGfxResMan **)(param_1 + 0xd30), (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)(param_1 + 0xd88), 1, 1, 1, 1, -1, -1, 0, 0, *(GF_2DGfxResMan **)(param_1 + 0xd24), *(GF_2DGfxResMan **)(param_1 + 0xd28), *(GF_2DGfxResMan **)(param_1 + 0xd2c), *(GF_2DGfxResMan **)(param_1 + 0xd30), (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)(param_1 + 0xdac), 2, 2, 2, 2, -1, -1, 0, 0, *(GF_2DGfxResMan **)(param_1 + 0xd24), *(GF_2DGfxResMan **)(param_1 + 0xd28), *(GF_2DGfxResMan **)(param_1 + 0xd2c), *(GF_2DGfxResMan **)(param_1 + 0xd30), (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    ov72_022383DC(&SStack_44, param_1, param_1 + 0xd64, 2);
    puVar2 = (u16 *)&ov72_0223B364;
    iVar4 = 0;
    iVar3 = param_1;
    do {
        SStack_44.position.x = (u32)*puVar2 << 0xc;
        SStack_44.position.y = (u32)puVar2[1] * 0x1000 + 0x100000;
        pSVar1 = Sprite_CreateAffine(&SStack_44);
        *(Sprite **)(iVar3 + 0xdf4) = pSVar1;
        Sprite_SetAnimActiveFlag(*(Sprite **)(iVar3 + 0xdf4), 1);
        Sprite_SetAnimCtrlSeq(*(Sprite **)(iVar3 + 0xdf4), 0);
        Sprite_SetDrawFlag(*(Sprite **)(iVar3 + 0xdf4), 0);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
        iVar3 = iVar3 + 4;
    } while (iVar4 < 3);
    ov72_022383DC(&SStack_44, param_1, param_1 + 0xd88, 2);
    SStack_44.position.x = 0x20000;
    SStack_44.position.y = 0x151000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    *(Sprite **)(param_1 + 0xe00) = pSVar1;
    Sprite_SetAnimActiveFlag(*(Sprite **)(param_1 + 0xe00), 1);
    Sprite_SetAnimCtrlSeq(*(Sprite **)(param_1 + 0xe00), 1);
    Sprite_SetDrawFlag(*(Sprite **)(param_1 + 0xe00), 0);
    SStack_44.position.x = 0x60000;
    SStack_44.position.y = 0x129000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    *(Sprite **)(param_1 + 0xe04) = pSVar1;
    Sprite_SetAnimActiveFlag(*(Sprite **)(param_1 + 0xe04), 1);
    Sprite_SetAnimCtrlSeq(*(Sprite **)(param_1 + 0xe04), 0);
    Sprite_SetDrawFlag(*(Sprite **)(param_1 + 0xe04), 0);
    puVar2 = (u16 *)&ov72_0223B338;
    iVar4 = 0;
    iVar3 = param_1;
    do {
        SStack_44.position.x = (u32)*puVar2 << 0xc;
        SStack_44.position.y = (u32)puVar2[1] * 0x1000 + 0x100000;
        pSVar1 = Sprite_CreateAffine(&SStack_44);
        *(Sprite **)(iVar3 + 0xe08) = pSVar1;
        Sprite_SetAnimActiveFlag(*(Sprite **)(iVar3 + 0xe08), 1);
        Sprite_SetAnimCtrlSeq(*(Sprite **)(iVar3 + 0xe08), 3);
        Sprite_SetPriority(*(Sprite **)(iVar3 + 0xe08), 1);
        Sprite_SetDrawFlag(*(Sprite **)(iVar3 + 0xe08), 0);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
        iVar3 = iVar3 + 4;
    } while (iVar4 < 3);
    ov72_022383DC(&SStack_44, param_1, param_1 + 0xdac, 2);
    SStack_44.position.x = 0x80000;
    SStack_44.position.y = 0x151000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    *(Sprite **)(param_1 + 0xe14) = pSVar1;
    Sprite_SetAnimActiveFlag(*(Sprite **)(param_1 + 0xe14), 1);
    Sprite_SetAnimCtrlSeq(*(Sprite **)(param_1 + 0xe14), 1);
    Sprite_SetDrawFlag(*(Sprite **)(param_1 + 0xe14), 0);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    sub_0203A880();
    return;
}
#endif

enum YesNoResponse ov72_02238668(YesNoPrompt *arg0);
void ov72_02238680(NintendoWfcConnect *arg0, s32 arg1, s32 arg2);

void ov72_02238688(Sprite *sprite, int x, int y);

enum YesNoResponse ov72_02238668(YesNoPrompt *arg0) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput(arg0);
    if (v0 != YESNORESPONSE_WAIT) {
        YesNoPrompt_Destroy(arg0);
    }
    return v0;
}

void ov72_02238680(NintendoWfcConnect *arg0, s32 arg1, s32 arg2) {
    *((s32 *)&arg0->state) = arg1;
    *((s32 *)&arg0->nextState) = arg2;
}

void ov72_02238688(Sprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = FX32_CONST(x);
    pos.y = FX32_CONST(y);
    pos.z = 0;

    Sprite_SetMatrix(sprite, &pos);
}
