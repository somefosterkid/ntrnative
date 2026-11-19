#include "global.h"
#include "overlay_01_021FC4C4.h"

void ov01_021FC65C(ResourceHeapItemHeader *item, u32 id, void *data);
void ov01_021FC664(ResourceHeapItemHeader *item);

void ov01_021FC65C(ResourceHeapItemHeader *item, u32 id, void *data) {
    item->data = data;
    item->id = id;
}

void ov01_021FC664(ResourceHeapItemHeader *item) {
    item->data = 0L;
    item->id = 0;
}
