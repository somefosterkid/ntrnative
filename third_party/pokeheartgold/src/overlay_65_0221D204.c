#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"

void ov65_0221D204(NARC *param_1, s32 param_2, BgConfig *param_3, u16 param_4, u32 param_5, u32 param_6, BOOL param_7, enum HeapID param_8);

void ov65_0221D25C(BgConfig *bgConfig, int sLotoId, int unused2);

void ov65_0221D204(NARC *param_1, s32 param_2, BgConfig *param_3, u16 param_4, u32 param_5, u32 param_6, BOOL param_7, enum HeapID param_8) {
    void *pNscrFile;
    BOOL BVar1;
    void *pvVar2;
    NNSG2dScreenData *pNStack_14;

    pNscrFile = GfGfxLoader_LoadFromOpenNarc(param_1, param_2, param_7, param_8, 1);
    if (pNscrFile != NULL) {
        BVar1 = NNS_G2dGetUnpackedScreenData(pNscrFile, &pNStack_14);
        if (BVar1 != 0) {
            if (param_6 == 0) {
                param_6 = pNStack_14->szByte;
            }
            pvVar2 = GetBgTilemapBuffer(param_3, param_4);
            if (pvVar2 != NULL) {
                BG_LoadScreenTilemapData(param_3, param_4, pNStack_14->rawData, param_6);
            }
        }
        Heap_Free(pNscrFile);
    }
    return;
}

void ov65_0221D25C(BgConfig *bgConfig, int sLotoId, int unused2) {
    BgCopyOrUncompressTilemapBufferRangeToVram(bgConfig, 2, GetBgTilemapBuffer(bgConfig, 2), (8 * (8 / 2)) * 24 * 2, 0);
}
