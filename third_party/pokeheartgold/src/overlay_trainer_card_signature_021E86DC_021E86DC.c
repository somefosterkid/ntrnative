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

void ov52_021E86DC(void);

void ov52_021E86DC(void) {
    {
        CharTransferTemplate v0 = {
            20, 2048, 2048, HEAP_ID_39
        };

        ObjCharTransfer_Init(&v0);
    }

    ObjPlttTransfer_Init(20, HEAP_ID_39);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
