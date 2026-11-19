#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "unk_0200A090.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "heap.h"
#include "overlay_02_hmcut_in.h"
#include "gf_gfx_planes.h"

NARC *ov02_0224A074(void);

void ov02_0224A4D0(HMCutIn *cutIn);

void ov02_0224A570(NARC *narc, u32 memberIndex, NNSG2dPaletteData **paletteData);

void ov02_0224A4D0(HMCutIn *cutIn) {
    int i;
    NARC *narc = ov02_0224A074();

    for (i = 0; i < 4; i++) {
        if (cutIn->charSource[i] == NULL) {
            cutIn->charSource[i] = AddCharResObjFromOpenNarc(cutIn->charLocation, narc, 0xB, 0, 0, 1, HEAP_ID_FIELD1);
            break;
        }
    }
    GF_ASSERT(i < 4);

    for (i = 0; i < 4; i++) {
        if (cutIn->cellSource[i] == NULL) {
            cutIn->cellSource[i] = AddCellOrAnimResObjFromOpenNarc(cutIn->cellLocation, narc, 0xC, 0, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
            break;
        }
    }
    GF_ASSERT(i < 4);

    NARC_Delete(narc);
}

void ov02_0224A570(NARC *narc, u32 memberIndex, NNSG2dPaletteData **paletteData) {
    void *nclrFile = NARC_AllocAndReadWholeMember(narc, memberIndex, HEAP_ID_FIELD1);

    NNS_G2dGetUnpackedPaletteData(nclrFile, paletteData);

    BG_LoadPlttData(GF_BG_LYR_MAIN_3, (*paletteData)->pRawData, ((16) * sizeof(u16)), 32 * 12);
    Heap_Free(nclrFile);
}
