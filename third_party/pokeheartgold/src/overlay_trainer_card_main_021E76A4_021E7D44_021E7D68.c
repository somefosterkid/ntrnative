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

void ov51_021E7D68(void);

void thunk_ClearMainOAM(enum HeapID heapID);

void thunk_ClearSubOAM(enum HeapID heapID);

void ov51_021E7D68(void) {
    CharTransferTemplate template = {
        .maxTasks = 2,
        .sizeMain = 0,
        .sizeSub = 0,
        .heapID = HEAP_ID_25
    };

    ObjCharTransfer_Init(&template);

    ObjPlttTransfer_Init(2, HEAP_ID_25);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    thunk_ClearMainOAM(HEAP_ID_25);
    thunk_ClearSubOAM(HEAP_ID_25);
}
