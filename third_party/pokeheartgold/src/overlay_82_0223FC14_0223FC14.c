#include "global.h"
#include "constants/heap.h"

struct CharTransferTemplate;

typedef struct CharTransferTemplate {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    enum HeapID heapID;
} CharTransferTemplate;

void ObjCharTransfer_InitEx(const CharTransferTemplate *template, GXOBJVRamModeChar modeMain, GXOBJVRamModeChar modeSub);

void ObjCharTransfer_ClearBuffers(void);

void ObjPlttTransfer_Init(int capacity, enum HeapID heapID);

void ObjPlttTransfer_Reset(void);

void ov82_0223FC14(void);

void ov82_0223FC14(void) {
    CharTransferTemplate transferTemplate = {
        3, 2048, 2048, HEAP_ID_105
    };

    ObjCharTransfer_InitEx(&transferTemplate, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);

    ObjPlttTransfer_Init(4, HEAP_ID_105);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
