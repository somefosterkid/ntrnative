#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void ov112_021EF17C(void);

extern const GraphicsModes ov112_021FF0DC;

void ov112_021EF17C(void) {
    GraphicsModes modes = ov112_021FF0DC;

    SetBothScreensModesAndDisable(&modes);
}

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

void ov112_021EF19C(void);

extern const CharTransferTemplate ov112_021FF0EC;

void ov112_021EF19C(void) {
    CharTransferTemplate v0 = ov112_021FF0EC;

    ObjCharTransfer_Init(&v0);

    ObjPlttTransfer_Init(20, (enum HeapID)154);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
