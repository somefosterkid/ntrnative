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

void ov72_02238164(void);

extern const CharTransferTemplate ov72_0223B344;

void ov72_02238164(void) {
    CharTransferTemplate v0 = ov72_0223B344;

    ObjCharTransfer_Init(&v0);

    ObjPlttTransfer_Init(20, (enum HeapID)67);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
