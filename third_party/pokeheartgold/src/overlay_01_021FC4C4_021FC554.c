#include "global.h"
#include "overlay_01_021FC4C4.h"

struct ResourceHeap;
struct ResourceHeapItemHeader;
void *ov01_021FC5FC(ResourceHeap *resHeap, u32 size, BOOL allocAtEnd);
ResourceHeapItemHeader *ov01_021FC644(ResourceHeap *resHeap);
void ov01_021FC65C(ResourceHeapItemHeader *item, u32 id, void *data);

void *ov01_021FC554(ResourceHeap *resHeap, u32 id, u32 dataSize, BOOL allocAtEnd);

void *ov01_021FC554(ResourceHeap *resHeap, u32 id, u32 dataSize, BOOL allocAtEnd) {
    ResourceHeapItemHeader *item = ov01_021FC644(resHeap);
    ((item != 0L) ? (void)0 : GF_AssertFail());

    void *data = ov01_021FC5FC(resHeap, dataSize, allocAtEnd);

    ov01_021FC65C(item, id, data);
    return data;
}
