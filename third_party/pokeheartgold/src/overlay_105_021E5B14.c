#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "overlay_manager.h"

s32 ov105_021E5B14(OverlayManager *man, s32 *state);

s32 ov105_021E5B14(OverlayManager *man, s32 *state) {
    NNSFndAllocator *sp4;
    OverlayManager *sp0;
    NNSFndAllocator *temp_r0;
    NNSFndAllocator *var_r7;
    u8 var_r4;
    void **var_r5;
    void **var_r6;

    sp0 = man;
    temp_r0 = OverlayManager_GetData(man);
    var_r7 = temp_r0;
    sp4 = temp_r0;
    var_r6 = &temp_r0->pHeap;
    var_r4 = 0;
    var_r7 = (void *)((u8 *)var_r7 + (0x80));
    do {
        var_r5 = &var_r6[var_r4];
        NNS_G3dFreeAnmObj(var_r7, *(NNSG3dAnmObj **)((u8 *)var_r5 + 0x6C));
        Heap_Free(*(void **)((u8 *)var_r5 + 0x5C));
        var_r4 += 1;
    } while ((u32)var_r4 < 4U);
    Heap_Free(*(void **)((u8 *)var_r6 + 0x58));
    Camera_Delete((Camera *)sp4->pFunc);
    OverlayManager_FreeData(sp0);
    GF3dRender_DeleteSimpleManager();
    Heap_Destroy(HEAP_ID_151);
    return 1;
}
