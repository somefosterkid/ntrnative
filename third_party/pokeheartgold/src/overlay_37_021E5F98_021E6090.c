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

void ov37_021E6090(void);

extern const CharTransferTemplate ov37_021E7988;

void ov37_021E6090(void) {
    {
        CharTransferTemplate v0 = ov37_021E7988;

        ObjCharTransfer_Init(&v0);
    }

    ObjPlttTransfer_Init(20, HEAP_ID_39);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
