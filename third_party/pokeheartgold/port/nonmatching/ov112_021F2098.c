/* PORTOK hk39 */
#include "filesystem.h"
#include "follow_mon.h"
#include "heap.h"
#include "pokemon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02013FDC.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "pokemon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02013FDC.h"
s32 ov112_021F0E14(s32, s32, s32);
void ov112_021F1D70(Sprite *, void *, u32, s32);
typedef struct Ov112FollowMonSprite {
    u32 isLarge;            // 0x00
    u32 species;            // 0x04
    ManagedSprite *small;   // 0x08
    ManagedSprite *large;   // 0x0C
    ManagedSprite *shown;   // 0x10
} Ov112FollowMonSprite;     // 0x14

void ov112_021F2098(Ov112FollowMonSprite *param_1, u16 param_2, u8 param_3, u32 param_4, u8 param_5, enum HeapID param_6) {
    u8 buf[8];
    s16 posX;
    NARC *narc;
    u8 uVar1;
    s32 iVar2;
    s32 iVar2_2;
    s32 iVar7;
    s32 iStack_34;
    u32 uVar3;
    u32 uVar6;
    NNSG3dResTex *pNVar4;
    Sprite *sprite;
    u8 *pBase;
    void *pvVar5;
    NNSG3dResFileHeader *header;
    NNSG2dImagePaletteProxy *pImg;
    s16 posY;

    iVar2 = SpeciesToOverworldModelIndexOffset(param_2);
    ReadWholeNarcMemberByIdPair(buf, NARC_fielddata_tsurepoke_tp_param, iVar2);
    if (buf[1] != 0) {
        sprite = param_1->large->sprite;
        uVar6 = 0x800;
        iStack_34 = 8;
        param_1->isLarge = 1;
        ManagedSprite_GetPositionXY(param_1->large, &posX, &posY);
        ManagedSprite_SetPositionXY(param_1->shown, posX, posY);
    } else {
        sprite = param_1->small->sprite;
        iStack_34 = 4;
        param_1->isLarge = 0;
        uVar6 = 0x200;
        ManagedSprite_GetPositionXY(param_1->small, &posX, &posY);
        ManagedSprite_SetPositionXY(param_1->shown, posX, posY);
    }
    param_1->species = param_2;
    narc = NARC_New(NARC_data_mmodel_mmodel, param_6);
    uVar1 = sub_02070438(param_2, param_3);
    if (uVar1 == 0) {
        param_3 = 0;
    }
    uVar3 = ov112_021F0E14(param_2, param_3, param_4);
    header = NARC_AllocAndReadWholeMember(narc, uVar3, param_6);
    pNVar4 = NNS_G3dGetTex(header);
    uVar3 = (pNVar4->texInfo).ofsTex;
    pBase = (u8 *)pNVar4 + uVar3;
    pvVar5 = Heap_AllocAtEnd(param_6, uVar6);
    iVar7 = 0;
    iVar2_2 = 0;
    do {
        sub_020145B4(pBase + iVar2_2, iStack_34, 0, 0, iStack_34, iStack_34, pvVar5);
        ov112_021F1D70(sprite, pvVar5, uVar6, iVar2_2);
        iVar2_2 = iVar2_2 + uVar6;
        iVar7 = iVar7 + 1;
    } while (iVar7 < 8);
    Heap_Free(pvVar5);
    Sprite_GetVramType(sprite);
    pvVar5 = (u8 *)pNVar4 + (pNVar4->plttInfo).ofsPlttData;
    if (param_5 != 0) {
        pvVar5 = (u8 *)pvVar5 + 0x20;
    }
    DC_FlushRange(pvVar5, 0x20);
    pImg = Sprite_GetPaletteProxy(sprite);
    uVar6 = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DMAIN);
    GX_LoadOBJPltt(pvVar5, uVar6, 0x20);
    Heap_Free(header);
    NARC_Delete(narc);
    thunk_Sprite_SetDrawFlag(sprite, 1);
    uVar3 = 1;
    if (param_1->species - 0x32 <= 1) {
        uVar3 = 0;
    }
    ManagedSprite_SetDrawFlag(param_1->shown, uVar3);
    return;
}
