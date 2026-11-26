#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_112_sprite_app.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "system.h"

s32 ov112_021F0DC0(void);

void ov112_021F0DF4(void);

s32 ov112_021F0F90(void *);

s32 ov112_021F1288(void *);

void ov112_021F179C(Ov112SpriteApp *work);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021F196C(void *);
#else
s32 ov112_021F196C(void *);
#endif

void ov112_021F2EB0(void *);

void ov112_021F2DE4(OverlayManager *arg0);

void ov112_021F2DE4(OverlayManager *man) {
    void *v0;
    Ov112SpriteApp *v1;

    v0 = OverlayManager_GetArgs(man);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    Heap_Create(HEAP_ID_3, HEAP_ID_155, 0x60000U);
    v1 = OverlayManager_CreateAndGetData(man, sizeof(Ov112SpriteApp), HEAP_ID_155);
    MI_CpuFill8(v1, 0U, sizeof(Ov112SpriteApp));
    v1->heapID = HEAP_ID_155;
    v1->args = v0;
    ov112_021F0F90(v1);
    ov112_021F0DC0();
    ov112_021F0DF4();
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    v1->narc = NARC_New(NARC_a_2_4_9, v1->heapID);
    ov112_021F179C(v1);
    ov112_021F1288(v1);
    ov112_021F196C(v1);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB(ov112_021F2EB0, v1);
}

void ov112_021F1324(Ov112SpriteApp *param_1);

s32 ov112_021F1814(void *);

s32 ov112_021F1A08(void *);

void ov112_021F2E70(OverlayManager *arg0);

void ov112_021F2E70(OverlayManager *man) {
    Ov112SpriteApp *v0;

    v0 = OverlayManager_GetData(man);
    ov112_021F1324(v0);
    ov112_021F1814(v0);
    ov112_021F1A08(v0);
    NARC_Delete(v0->narc);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    ov112_021F0DC0();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_155);
}
