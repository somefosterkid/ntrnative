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

void ov81_02242B90(void);

void ov81_02242B90(void) {
    CharTransferTemplate transferTemplate = {
        .maxTasks = 0x20,
        .sizeMain = 0x800,
        .sizeSub = 0x800,
        .heapID = (enum HeapID)100
    };

    ObjCharTransfer_InitEx(&transferTemplate, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K);

    ObjPlttTransfer_Init(8, (enum HeapID)100);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
