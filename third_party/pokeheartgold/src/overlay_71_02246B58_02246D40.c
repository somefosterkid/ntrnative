#include "global.h"
struct PaletteShimmerEffect;

#include "heap.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "overlay_71_palette_shimmer_effect.h"

void ov71_02246D40(struct PaletteShimmerEffect *arg0);

void ov71_02246D40(PaletteShimmerEffect *arg0) {
    if (*arg0->isAlive == 1) {
        arg0->stopping = 1;
    }
}

void ov71_02246F60(PaletteShimmerEffect *shimmerEffect, int slotIdx);

void ov71_02246D54(struct PaletteShimmerEffect *ptr);

void ov71_02246D54(PaletteShimmerEffect *ptr) {
    s32 v0;

    if (*ptr->isAlive == 1) {
        v0 = 0;
        do {
            if (ptr->sparkTasks[v0] != NULL) {
                ov71_02246F60(ptr, v0);
            }
            v0 += 1;
        } while (v0 < 8);
        *ptr->isAlive = 0;
        SysTask_Destroy(ptr->vblankTask);
        Heap_Free(ptr);
    }
}

void ov71_02246EAC(PaletteShimmerEffect *shimmerEffect);

void ov71_02246D9C(SysTask *arg0, void *arg1);

void ov71_02246D9C(SysTask *task, void *param) {
    PaletteShimmerEffect *pSrc = param;

    if (pSrc->stopping == 0) {
        pSrc->spawnTimer = pSrc->spawnTimer - 1;
        if (pSrc->spawnTimer <= 0) {
            PlaySE(0x6ADU);
            pSrc->spawnTimer = 0x1E;
            ov71_02246EAC(pSrc);
        }
        if (pSrc->dirty != 0) {
            GX_BeginLoadBGExtPltt();
            GXS_BeginLoadBGExtPltt();
            DC_FlushRange(pSrc->paletteBuffer, 0x60U);
            GX_LoadBGExtPltt(pSrc->paletteBuffer, 0x6000U, 0x60U);
            GXS_LoadBGExtPltt(pSrc->paletteBuffer, 0x6000U, 0x60U);
            GX_EndLoadBGExtPltt();
            GXS_EndLoadBGExtPltt();
            pSrc->dirty = 0;
        }
    } else {
        if (pSrc->dirty != 0) {
            GX_BeginLoadBGExtPltt();
            GXS_BeginLoadBGExtPltt();
            DC_FlushRange(pSrc->paletteBuffer, 0x60U);
            GX_LoadBGExtPltt(pSrc->paletteBuffer, 0x6000U, 0x60U);
            GXS_LoadBGExtPltt(pSrc->paletteBuffer, 0x6000U, 0x60U);
            GX_EndLoadBGExtPltt();
            GXS_EndLoadBGExtPltt();
            pSrc->dirty = 0;
        }
        if (pSrc->activeSparkCount == 0) {
            *pSrc->isAlive = 0;
            Heap_Free(pSrc);
            SysTask_Destroy(task);
        }
    }
}
