#include "global.h"

#include "constants/heap.h"

#include "gf_3d_loader.h"
#include "overlay_01_021F1348.h"

enum HeapID ov01_021F1478(FieldEffectManager *fieldEffMan);

void ov01_021F17BC(FieldEffectManager *fieldEffMan, struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, s32 count);

struct ResourceHeap *ov01_021FC4C4(enum HeapID parentHeapID, enum HeapID heapID, u32 itemPoolSize, u32 itemCount);

BillboardList *sub_020237EC(const BillboardListParams *params);

void ov01_021F1648(FieldEffectManager *fieldEffMan, enum HeapID heapID, u32 billboardCount, u32 modelCount, u32 metadataCount, s32 texCount, u32 modelPoolSize, u32 metadataPoolSize, u32 arg8);

void ov01_021F17F0(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc);
void ov01_021FC520(struct ResourceHeap *resHeap);
BOOL sub_02023874(BillboardList *list);
void ov01_021F16B8(FieldEffectManager *fieldEffMan);

void ov01_021F1648(FieldEffectManager *fieldEffMan, enum HeapID heapID, u32 billboardCount, u32 modelCount, u32 metadataCount, s32 texCount, u32 modelPoolSize, u32 metadataPoolSize, u32 arg8) {
    struct UnkStruct_ov5_021DF8FC_t *billboardAlloc;
    BillboardListParams params;

    billboardAlloc = ov01_021F1430(fieldEffMan, sizeof(struct UnkStruct_ov5_021DF8FC_t), 0, 0);
    fieldEffMan->unk_20 = billboardAlloc;
    billboardAlloc->heapID = heapID;
    billboardAlloc->unk_04 = billboardCount;
    billboardAlloc->unk_06 = modelCount;
    billboardAlloc->unk_08 = metadataCount;
    billboardAlloc->unk_0A = texCount;
    billboardAlloc->modelResHeap = ov01_021FC4C4(heapID, HEAP_ID_68, modelPoolSize, modelCount);
    billboardAlloc->metadataResHeap = ov01_021FC4C4(heapID, HEAP_ID_69, metadataPoolSize, metadataCount);
    billboardAlloc->texResMan = GF3dGfxRawResMan_Create(texCount, heapID);
    ov01_021F17BC(fieldEffMan, billboardAlloc, billboardCount);
    params.maxElements = billboardCount;
    params.heapID = ov01_021F1478(fieldEffMan);
    billboardAlloc->unk_0C = sub_020237EC(&params);
}

void ov01_021F16B8(FieldEffectManager *fieldEffMan) {
    struct UnkStruct_ov5_021DF8FC_t *billboardAlloc;

    billboardAlloc = fieldEffMan->unk_20;
    if (billboardAlloc != NULL) {
        sub_02023874(billboardAlloc->unk_0C);
        ov01_021F17F0(billboardAlloc);
        ov01_021FC520(billboardAlloc->modelResHeap);
        ov01_021FC520(billboardAlloc->metadataResHeap);
        GF3dGfxRawResMan_Destroy(billboardAlloc->texResMan);
        ov01_021F1448(billboardAlloc);
        fieldEffMan->unk_20 = NULL;
    }
}
