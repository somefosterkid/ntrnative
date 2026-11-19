#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "heap.h"
#include "overlay_manager.h"

s32 ov59_02237E94(Ov59ApricornApp *);

s32 ov59_02237F3C();

s32 ov59_02238624(OverlayManager *);

s32 ov59_02237D40(OverlayManager *arg0, s32 *arg1);

s32 ov59_02237D40(OverlayManager *arg0, s32 *arg1) {
    s32 temp_r1;
    Ov59ApricornApp *temp_r0;

    temp_r1 = *arg1;
    switch (temp_r1) {
    case 0:
        ov59_02238624(arg0);
        Heap_Create(HEAP_ID_3, HEAP_ID_134, 0x20000U);
        temp_r0 = OverlayManager_CreateAndGetData(arg0, sizeof(Ov59ApricornApp), HEAP_ID_134);
        MI_CpuFill8(temp_r0, 0U, sizeof(Ov59ApricornApp));
        temp_r0->heapId = HEAP_ID_134;
        temp_r0->parent = OverlayManager_GetArgs(arg0);
        ov59_02237E94(temp_r0);
        *arg1 += 1;
    default:
        break;
    case 1:
        OverlayManager_GetData(arg0);
        if (ov59_02237F3C() != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
