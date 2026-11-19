#include "global.h"
#include "heap.h"
#include "overlay_manager.h"
#include "overlay_59_juice_feed_app.h"

s32 ov59_0223AA6C(void *);

s32 ov59_0223AB00(Ov59JuiceFeedApp *arg0);

s32 ov59_0223A9B8(OverlayManager *arg0);

s32 ov59_0223A9B8(OverlayManager *arg0) {
    Ov59JuiceFeedApp *temp_r4;

    temp_r4 = OverlayManager_GetData(arg0);
    if (ov59_0223AB00(temp_r4) == 0) {
        return 0;
    }
    ov59_0223AA6C(temp_r4);
    OverlayManager_FreeData(arg0);
    Heap_Destroy(HEAP_ID_134);
    return 1;
}
