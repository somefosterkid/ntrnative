#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "overlay_59_juice_feed_app.h"

s32 ov59_0223AABC();

s32 ov59_0223ACD0(OverlayManager *);

s32 ov59_0223A8E0(OverlayManager *arg0, s32 *arg1);

s32 ov59_0223A8E0(OverlayManager *arg0, s32 *arg1) {
    s32 temp_r1;
    Ov59JuiceFeedApp *temp_r0;

    temp_r1 = *arg1;
    switch (temp_r1) {
    case 0:
        ov59_0223ACD0(arg0);
        Heap_Create(HEAP_ID_3, HEAP_ID_134, 0x90000U);
        temp_r0 = OverlayManager_CreateAndGetData(arg0, sizeof(Ov59JuiceFeedApp), HEAP_ID_134);
        MI_CpuFill8(temp_r0, 0U, sizeof(Ov59JuiceFeedApp));
        temp_r0->heapId = HEAP_ID_134;
        temp_r0->parent = OverlayManager_GetArgs(arg0);
        ov59_0223A9E4(temp_r0);
        *arg1 += 1;
    default:
        break;
    case 1:
        OverlayManager_GetData(arg0);
        if (ov59_0223AABC() != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
