#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "overlay_manager.h"

s32 ov104_021E5B14(OverlayManager *man, s32 *state);

s32 ov104_021E5B14(OverlayManager *man, s32 *state) {
    void *sp8;
    s32 sp4;
    OverlayManager *sp0;
    Camera **temp_r7;
    u8 temp_r0;
    u8 var_r4;
    void *temp_r5;
    void *temp_r6;

    sp0 = man;
    temp_r7 = OverlayManager_GetData(man);
    sp4 = 0;
    sp8 = temp_r7 + 1;
    do {
        var_r4 = 0;
        temp_r6 = sp8 + (sp4 * 0x6C);
loop_2:
        temp_r5 = temp_r6 + (var_r4 * 4);
        NNS_G3dFreeAnmObj((struct NNSFndAllocator *)(temp_r7 + 82), *(NNSG3dAnmObj **)((u8 *)temp_r5 + 0x64));
        Heap_Free(*(void **)((u8 *)temp_r5 + 0x5C));
        var_r4 += 1;
        if ((u32)var_r4 < 2U) {
            goto loop_2;
        }
        Heap_Free(*(void **)((u8 *)temp_r6 + 0x58));
        temp_r0 = sp4 + 1;
        sp4 = (s32)temp_r0;
    } while ((u32)temp_r0 < 3U);
    Camera_Delete(*temp_r7);
    OverlayManager_FreeData(sp0);
    GF3dRender_DeleteSimpleManager();
    Heap_Destroy(HEAP_ID_149);
    return 1;
}
