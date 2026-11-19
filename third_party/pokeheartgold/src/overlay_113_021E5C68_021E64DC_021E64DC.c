#include "global.h"
#include "overlay_113.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"

void ov113_021E64DC(UnownReportScreen *param_1, NARC *param_2, s32 param_3, enum HeapID param_4);

void ov113_021E64DC(UnownReportScreen *param_1, NARC *param_2, s32 param_3, enum HeapID param_4) {
    u32 new_var;
    void *pvVar1;

    pvVar1 = GfGfxLoader_GetScrnDataFromOpenNarc(param_2, param_3, 1, &param_1->scrnData, param_4);
    param_1->buf = pvVar1;
    new_var = param_1->scrnData->screenWidth;
    param_1->width = (u8)(new_var >> 3);
    param_1->height = (u8)((new_var = param_1->scrnData->screenHeight) >> 3);
    param_1->tilemap = &param_1->scrnData->rawData;
    return;
}
