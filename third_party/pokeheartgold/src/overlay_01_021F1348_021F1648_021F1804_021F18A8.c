#include "global.h"
#include "overlay_01_021F1348.h"

#include "error_handling.h"

void *ov01_021FC5CC(struct ResourceHeap *resHeap, u32 id, NARC *narc, u32 memberIndex, BOOL allocAtEnd);

BillboardResources *ov01_021F18A8(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, u32 id);

BillboardResources *ov01_021F18A8(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, u32 id) {
    u32 v0 = billboardAlloc->unk_04;
    struct UnkStruct_ov5_021DF8C8_t *v1 = billboardAlloc->unk_1C;

    do {
        if (v1->unk_00 == id) {
            return v1->unk_04;
        }

        v1++;
        v0--;
    } while (v0);

    GF_ASSERT(FALSE);
    return NULL;
}

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id) {
    ov01_021F1804(fieldEffMan->unk_20, id);
}

void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex) {
    ov01_021FC5CC(fieldEffMan->unk_20->modelResHeap, id, fieldEffMan->fldEffNARC, memberIndex, FALSE);
}
