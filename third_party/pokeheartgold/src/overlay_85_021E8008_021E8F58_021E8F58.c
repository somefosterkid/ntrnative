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

void ov85_021E8F58(void);

extern const CharTransferTemplate ov85_021EA904;

void ov85_021E8F58(void) {
    CharTransferTemplate v0 = ov85_021EA904;

    ObjCharTransfer_Init(&v0);

    ObjPlttTransfer_Init(20, HEAP_ID_102);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
