#include "global.h"
#include "constants/heap.h"

struct CharTransferTemplate;

typedef struct CharTransferTemplate {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    enum HeapID heapID;
} CharTransferTemplate;

void ObjCharTransfer_Init(const CharTransferTemplate *template);

void ObjCharTransfer_ClearBuffers(void);

void ObjPlttTransfer_Init(int capacity, enum HeapID heapID);

void ObjPlttTransfer_Reset(void);

void ov73_021E6060(void);

extern const CharTransferTemplate ov73_021EA550;

void ov73_021E6060(void) {
    CharTransferTemplate v0 = ov73_021EA550;

    ObjCharTransfer_Init(&v0);

    ObjPlttTransfer_Init(20, HEAP_ID_50);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
