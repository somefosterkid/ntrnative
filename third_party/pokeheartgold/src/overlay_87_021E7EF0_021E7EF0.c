#include "global.h"
#include "constants/heap.h"
#include "overlay_87_scratch_off_card_app.h"

struct CharTransferTemplate;

struct ScratchOffCardsAppSpriteManager;

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

void ov87_021E7C50(ScratchOffCardsAppSpriteManager *spriteMan, int resourceID, int resourceID2, enum NNS_G2D_VRAM_TYPE vRamType);

void ov87_021E7EF0(void);

void ov87_021E7EF0(void) {
    CharTransferTemplate transferTemplate = {
        .maxTasks = 5,
        .sizeMain = 3000,
        .sizeSub = 5120,
        .heapID = HEAP_ID_122
    };

    ObjCharTransfer_InitEx(&transferTemplate, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K);

    ObjPlttTransfer_Init(14, (enum HeapID)(122));
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
