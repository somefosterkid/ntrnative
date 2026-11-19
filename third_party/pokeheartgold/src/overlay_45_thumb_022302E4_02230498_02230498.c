#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_02023694.h"

void ov45_0222D740(NNSG3dResFileHeader *);

void ov45_022309E8(u32 *, NARC *, u32, enum HeapID);

void ov45_02230AB4(s32);

u32 sub_020237EC(s32 *);

extern u32 ov45_02254C38;

extern u32 ov45_02254C48;

u32 *ov45_02230498(s32 param_1, s32 param_2, enum HeapID param_3, enum HeapID param_4);

u32 *ov45_02230498(s32 param_1, s32 param_2, enum HeapID param_3, enum HeapID param_4) {
    u32 *puVar1;
    void *pvVar2;
    GF_2DGfxRawResMan *pGVar3;
    GF_3DGfxRawResMan *pGVar4;
    u32 uVar5;
    NARC *narc;
    NNSG3dResFileHeader *header;
    NNSG3dResMdl *pMdl;
    GF_3DGfxRawResObj *obj;
    s32 iVar8;
    u16 *puVar9;
    u8 *puVar10;
    u32 uVar11;
    s32 iVar12;
    u32 shouldCopyWithoutTex;
    NARC *narc_00;
    u32 uStack_30;
    s32 iStack_20[2];
    puVar1 = Heap_Alloc(param_3, 0x30);
    memset(puVar1, 0, 0x30);
    pvVar2 = Heap_Alloc(param_3, param_1 * 0x90);
    puVar1[4] = (u32)pvVar2;
    uVar11 = 0;
    puVar1[5] = param_1;
    if (uVar11 < ((u32)param_1)) {
        iVar8 = 0;
        do {
            ov45_02230AB4(puVar1[4] + iVar8);
            uVar11 = uVar11 + 1;
            iVar8 = iVar8 + 0x90;
        } while (uVar11 < ((u32)puVar1[5]));
    }
    pGVar3 = GF2dGfxRawResMan_Create(1, param_3);
    *puVar1 = (u32)pGVar3;
    pGVar3 = GF2dGfxRawResMan_Create(2, param_3);
    puVar1[1] = (u32)pGVar3;
    pGVar4 = GF3dGfxRawResMan_Create(0x14, param_3);
    puVar1[2] = (u32)pGVar4;
    BillboardLists_Create(1, param_3);
    iStack_20[0] = param_1;
    iStack_20[1] = param_3;
    uVar5 = sub_020237EC(iStack_20);
    puVar1[3] = uVar5;
    narc = NARC_New(NARC_data_mmodel_mmodel, param_3);
    narc_00 = NARC_New(NARC_a_2_0_7, param_3);
    header = GfGfxLoader_LoadFromOpenNarc(narc_00, 0x7f, 0, param_4, 0);
    GF2dGfxRawResMan_AllocObj((GF_2DGfxRawResMan *)(*puVar1), header, 0x7f);
    ov45_0222D740(header);
    pMdl = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(header), 0);
    iVar8 = NNS_G3dMdlGetMdlLightEnableFlag(pMdl, 0);

    iVar12 = 0;
    puVar9 = (u16 *)(&ov45_02254C38);
    puVar1[0xb] = iVar8;
    do {
        pvVar2 = GfGfxLoader_LoadFromOpenNarc(narc, (u32)(*puVar9), 0, param_4, 0);
        GF2dGfxRawResMan_AllocObj((GF_2DGfxRawResMan *)puVar1[1], pvVar2, (u32)(*puVar9));
        iVar12 = iVar12 + 1;
        puVar9 = puVar9 + 1;
    } while (iVar12 < 2);
    uStack_30 = (u32)(param_2 == 0);
    puVar10 = (unsigned char *)(&ov45_02254C48);
    uVar11 = 0;
    do {
        if (uStack_30 != uVar11) {
            shouldCopyWithoutTex = ((((u32)(*((u16 *)(puVar10 + 2)))) << 0x10) >> 31) == 0;
            pvVar2 = GfGfxLoader_LoadFromOpenNarc(narc, ((u32)(((u16)(*((s16 *)(puVar10 + 2)))) << 17)) >> 17, 0, param_4, 0);
            uVar5 = ((u16)(*((u16 *)(puVar10 + 2)))) << 17;
            obj = GF3dGfxRawResMan_AllocObj((GF_3DGfxRawResMan *)puVar1[2], pvVar2, uVar5 >> 17, shouldCopyWithoutTex, param_4);
            if (shouldCopyWithoutTex == 1) {
                GF3dGfxRawResObj_AllocVramAndGetKeys(obj);
                GF3dGfxRawResObj_LoadTex(obj);
                GF3dGfxRawResObj_FreeVramAndSecondaryHeader(obj);
            }
        }
        puVar10 = puVar10 + 4;
        uVar11 = uVar11 + 1;
    } while (((s32)uVar11) < 0x14);
    ov45_022309E8(puVar1 + 6, narc_00, 0x80, param_4);
    NARC_Delete(narc);
    NARC_Delete(narc_00);
    return puVar1;
}
