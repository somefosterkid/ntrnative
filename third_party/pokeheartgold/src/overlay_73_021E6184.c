#include "global.h"

#include "constants/heap.h"

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

extern u32 ov73_021EA560;

void ov73_021E6184(UnkStruct_020961E8 *param_1);

s32 ov73_021E63BC(void *, void *, s32);
s32 ov73_021E735C(void *, s32, s32, void *);
void ov73_021E629C(UnkStruct_020961E8 *arg0);

void ov73_021E6184(UnkStruct_020961E8 *param_1) {
    Sprite *pSVar1;
    s32 iVar3;
    u8 *puVar2;
    SpriteTemplate SStack_44;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u8 *pu8;
#endif

    CreateSpriteResourcesHeader(&param_1->unk_268, 2, 2, 2, 2, -1, -1, 0, 1, param_1->unk_1E0[0], param_1->unk_1E0[1], param_1->unk_1E0[2], param_1->unk_1E0[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_44.spriteList = param_1->unk_50;
    SStack_44.header = &param_1->unk_268;
    SStack_44.position.z = 0;
    SStack_44.scale.x = 0x1000;
    SStack_44.scale.y = 0x1000;
    SStack_44.scale.z = 0x1000;
    SStack_44.rotation = 0;
    SStack_44.drawPriority = 1;
    SStack_44.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_44.heapID = HEAP_ID_50;
    SStack_44.position.x = 0x54000;
    SStack_44.position.y = 0x68000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    param_1->unk_28C[0] = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->unk_28C[0], 1);
    Sprite_SetDrawFlag(param_1->unk_28C[0], 1);
    Sprite_SetDrawPriority(param_1->unk_28C[0], 100);
    puVar2 = (unsigned char *)(&ov73_021EA560);
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    pu8 = (u8 *)param_1;
#endif
    iVar3 = 0;
    do {
        SStack_44.position.x = (u32) * (u16 *)(puVar2 + 4) << 0xc;
        SStack_44.position.y = (u32) * (u16 *)(puVar2 + 6) << 0xc;
        pSVar1 = Sprite_CreateAffine(&SStack_44);
#if defined(SDK_PORT) || defined(NONMATCHING)
        param_1->unk_28C[iVar3 + 1] = pSVar1;
        Sprite_SetAnimActiveFlag(param_1->unk_28C[iVar3 + 1], 1);
        Sprite_SetAnimCtrlSeq(param_1->unk_28C[iVar3 + 1], (iVar3 + -1) * 2 + 0x1b);
        Sprite_SetDrawFlag(param_1->unk_28C[iVar3 + 1], 0);
#else
        *(Sprite **)(pu8 + 0x22c) = pSVar1;
        Sprite_SetAnimActiveFlag(*(Sprite **)(pu8 + 0x22c), 1);
        Sprite_SetAnimCtrlSeq(*(Sprite **)(pu8 + 0x22c), (iVar3 + -1) * 2 + 0x1b);
        Sprite_SetDrawFlag(*(Sprite **)(pu8 + 0x22c), 0);
#endif
        iVar3 = iVar3 + 1;
        puVar2 = puVar2 + 4;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
        pu8 = pu8 + 4;
#endif
    } while (iVar3 < 5);
    SStack_44.position.x = 0x18000;
    SStack_44.position.y = 0x1a0000;
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    return;
}

void ov73_021E629C(UnkStruct_020961E8 *arg0) {
    AddWindowParameterized(arg0->unk_00, &arg0->unk_2FC[1], 0U, 2U, 1U, 0x1BU, 4U, 0xCU, 0x5BU);
    FillWindowPixelBuffer(&arg0->unk_2FC[1], 0xFU);
    AddWindowParameterized(arg0->unk_00, &arg0->unk_2FC[2], 0U, 0x10U, 0x15U, 8U, 2U, 9U, 0xC7U);
    FillWindowPixelBuffer(&arg0->unk_2FC[2], 0U);
    AddWindowParameterized(arg0->unk_00, &arg0->unk_2FC[3], 0U, 0x18U, 0x15U, 8U, 2U, 9U, 0x133U);
    FillWindowPixelBuffer(&arg0->unk_2FC[3], 0U);
    AddWindowParameterized(arg0->unk_00, &arg0->unk_2FC[4], 4U, 3U, 1U, 0x1AU, 2U, 9U, 1U);
    ov73_021E63BC(&arg0->unk_2FC[4], arg0->unk_48, 0);
    AddWindowParameterized(arg0->unk_00, arg0->unk_2FC, 4U, 8U, 7U, 0x10U, 0xBU, 9U, 0x35U);
    FillWindowPixelBuffer(arg0->unk_2FC, 0U);
    ov73_021E735C(arg0->unk_2FC, 0, 0x10300, arg0);
    arg0->unk_384 = YesNoPrompt_Create(HEAP_ID_50);
}
