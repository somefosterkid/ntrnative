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

void ov70_022388D0(void);

extern const CharTransferTemplate ov70_02245208;

void ov70_022388D0(void) {
    CharTransferTemplate v0 = ov70_02245208;

    ObjCharTransfer_Init(&v0);

    ObjPlttTransfer_Init(20, (enum HeapID)61);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
