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

void ov102_021E9198(void);

void ov102_021E9198(void) {
    CharTransferTemplate *new_var;
    CharTransferTemplate v0 = { 2, 0xC80, 0, HEAP_ID_35 };
    new_var = &v0;
    ObjCharTransfer_Init(new_var);
    ObjCharTransfer_ClearBuffers();
}
