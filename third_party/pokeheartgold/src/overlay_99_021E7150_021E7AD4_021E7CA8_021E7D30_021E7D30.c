#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov99_021E7D30 {
    BgConfig *bgConfig;
    u32 unk04[2];
    enum HeapID heapID;
} UnkStruct_ov99_021E7D30;

typedef struct ov99_021EA0BC_Array {
    BgTemplate t[7];
} ov99_021EA0BC_Array;

extern const struct GraphicsModes ov99_021E9F80;

extern const ov99_021EA0BC_Array ov99_021EA0BC;

extern const u8 ov99_021EA348[7];

void ov99_021E7D30(UnkStruct_ov99_021E7D30 *arg0);

void ov99_021E7D30(UnkStruct_ov99_021E7D30 *arg0) {
    struct GraphicsModes v0;
    ov99_021EA0BC_Array v1;
    int i;

    arg0->bgConfig = BgConfig_Alloc(arg0->heapID);

    v0 = ov99_021E9F80;
    SetBothScreensModesAndDisable(&v0);

    v1 = ov99_021EA0BC;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(arg0->bgConfig, ov99_021EA348[i], &v1.t[i], 0);
        BgClearTilemapBufferAndCommit(arg0->bgConfig, ov99_021EA348[i]);
        BG_ClearCharDataRange(ov99_021EA348[i], 0x20, 0, arg0->heapID);
    }
}
