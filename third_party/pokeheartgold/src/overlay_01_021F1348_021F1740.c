#include "global.h"
#include "overlay_01_021F1348.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "error_handling.h"
#include "gf_3d_loader.h"
#include "sprite.h"

Billboard *ov01_021F16EC(FieldEffectManager *param0, const BillboardResources *param1, const VecFx32 *param2);
u32 *ov01_021FC5A4(struct ResourceHeap *resHeap, u32 id);
void *ov01_021F141C(const FieldEffectManager *, s32, s32);
void ov01_021F1448(void *a0);

Billboard *ov01_021F1740(FieldEffectManager *fieldEffMan, u32 id, const VecFx32 *position);
void ov01_021F17BC(FieldEffectManager *fieldEffMan, struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, s32 count);
void ov01_021F17F0(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc);

Billboard *ov01_021F1740(FieldEffectManager *fieldEffMan, u32 id, const VecFx32 *position) {
    void *uVar1;

    uVar1 = ov01_021F18A8(fieldEffMan->unk_20, id);
    return ov01_021F16EC(fieldEffMan, uVar1, position);
}

BillboardResources *ov01_021F1758(FieldEffectManager *fieldEffMan, s32 arg1, s32 arg2, s32 arg3, s32 id, s32 arg5, void *arg6) {
    BillboardGfxSequence gfxSequence;
    GF_3DGfxRawResObj *obj;
    NNSG3dResTex *nNSG3dResTex;
    BillboardResources *v0;
    u32 *v2;
    struct UnkStruct_ov5_021DF8FC_t *v1;

    v1 = fieldEffMan->unk_20;
    v2 = ov01_021FC5A4(v1->modelResHeap, arg2);
    BillboardGfxSequence_SetData(ov01_021FC5A4(v1->metadataResHeap, arg3), &gfxSequence);
    obj = GF3dGfxRawResMan_GetObjById(v1->texResMan, id);
    if (obj == NULL) {
        GF_AssertFail();
    }
    nNSG3dResTex = GF3dGfxRawResObj_GetTex(obj);
    if (arg5 == 1) {
        obj = NULL;
    }
    v0 = ov01_021F1824(v1, arg1, v2, &gfxSequence, nNSG3dResTex, obj, arg6);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    return v0;
}

void ov01_021F17BC(FieldEffectManager *fieldEffMan, struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, s32 count) {
    s32 v2;
    struct UnkStruct_ov5_021DF8C8_t *v0;
    BillboardResources *v1;

    v2 = count;
    v1 = ov01_021F141C(fieldEffMan, sizeof(BillboardResources) * v2, 0);
    billboardAlloc->unk_20 = v1;
    v0 = ov01_021F141C(fieldEffMan, v2 * sizeof(struct UnkStruct_ov5_021DF8C8_t), 0);
    billboardAlloc->unk_1C = v0;
    do {
        v0->unk_00 = 0xF;
        v0->unk_04 = v1;
        v0++;
        v1++;
        v2 -= 1;
    } while (v2 != 0);
}

void ov01_021F17F0(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc) {
    ov01_021F1448(billboardAlloc->unk_1C);
    ov01_021F1448(billboardAlloc->unk_20);
}

void ov01_021F1804(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, u32 id) {
    u32 i;
    struct UnkStruct_ov5_021DF8C8_t *iter;

    i = billboardAlloc->unk_04;
    iter = billboardAlloc->unk_1C;
    do {
        if (iter->unk_00 == id) {
            iter->unk_00 = 0xF;
            return;
        }
        iter++;
        i--;
    } while (i != 0);
    GF_AssertFail();
}
