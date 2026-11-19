#include "global.h"

#include "error_handling.h"
#include "overlay_01_021FC4C4.h"
#include "filesystem.h"

void ov01_021FC61C(void *ptr);

void ov01_021FC664(ResourceHeapItemHeader *item);

void *ov01_021FC554(ResourceHeap *resHeap, u32 id, u32 dataSize, BOOL allocAtEnd);
void *ov01_021FC5CC(ResourceHeap *resHeap, u32 id, NARC *narc, u32 memberIndex, BOOL allocAtEnd);

void ov01_021FC588(ResourceHeap *resHeap, u32 id) {
    ResourceHeapItemHeader *v0;

    v0 = ov01_021FC624(resHeap, id);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    ov01_021FC61C(v0->data);
    ov01_021FC664(v0);
}

void *ov01_021FC5A4(ResourceHeap *resHeap, u32 id) {
    ResourceHeapItemHeader *v0;

    v0 = ov01_021FC624(resHeap, id);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    return v0->data;
}

BOOL ov01_021FC5B8(ResourceHeap *resHeap, u32 id) {
    if (ov01_021FC624(resHeap, id) != 0) {
        return 1;
    }
    return 0;
}

void *ov01_021FC5CC(ResourceHeap *resHeap, u32 id, NARC *narc, u32 memberIndex, BOOL allocAtEnd) {
    u32 memberSize = NARC_GetMemberSize(narc, memberIndex);
    void *buffer = ov01_021FC554(resHeap, id, memberSize, allocAtEnd);

    NARC_ReadWholeMember(narc, memberIndex, buffer);
    return buffer;
}
