#include "global.h"
#include "overlay_01_021F1348.h"

#include "filesystem.h"

void *ov01_021FC5CC(struct ResourceHeap *resHeap, u32 id, NARC *narc, u32 memberIndex, BOOL allocAtEnd);
void ov01_021FC588(struct ResourceHeap *resHeap, u32 id);

void ov01_021F1908(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

void ov01_021F1908(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex) {
    ov01_021FC5CC(fieldEffMan->unk_20->metadataResHeap, id, fieldEffMan->fldEffNARC, memberIndex, FALSE);
}

void ov01_021F1924(FieldEffectManager *fieldEffMan, u32 slotID);

void ov01_021F1924(FieldEffectManager *fieldEffMan, u32 slotID) {
    ov01_021FC588(fieldEffMan->unk_20->metadataResHeap, slotID);
}
