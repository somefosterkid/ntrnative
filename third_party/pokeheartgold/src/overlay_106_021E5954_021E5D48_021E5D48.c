#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"

struct CharTransferTemplate;

typedef struct CharTransferTemplate {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    enum HeapID heapID;
} CharTransferTemplate;

void ObjCharTransfer_ClearBuffers(void);

void ObjPlttTransfer_Reset(void);

void ov106_021E5D48(void);

void ov106_021E5D48(void) {
    int new_var;
    CharTransferTemplate v0 = { 1, 0, 0, HEAP_ID_1 };
    SetBothScreensModesAndDisable((const struct GraphicsModes *)(&v0));
    new_var = 4;
    BG_SetMaskColor(new_var, HEAP_ID_DEFAULT);
}
