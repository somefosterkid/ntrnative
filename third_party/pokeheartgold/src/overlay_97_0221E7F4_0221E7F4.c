#include "global.h"
#include "bg_window.h"

struct GXBanks;

void GfGfx_SetBanks(const GraphicsBanks *banks);

void ov97_0221E7F4(void);

extern const GraphicsBanks ov97_0221FCCC;

void ov97_0221E7F4(void) {
    GraphicsBanks banks = ov97_0221FCCC;

    GfGfx_SetBanks(&banks);
}

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void ov97_0221E814(void);

extern const GraphicsModes ov97_0221FCA8;

void ov97_0221E814(void) {
    GraphicsModes modes = ov97_0221FCA8;

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

void ov97_0221E834(void);

extern const CharTransferTemplate _0221FC98;

void ov97_0221E834(void) {
    CharTransferTemplate v0 = _0221FC98;

    ObjCharTransfer_Init(&v0);

    ObjPlttTransfer_Init(20, HEAP_ID_POKEATHLON);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
