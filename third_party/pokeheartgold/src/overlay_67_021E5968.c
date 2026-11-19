#include "global.h"

#include "heap.h"
#include "overlay_67_app.h"
#include "overlay_manager.h"

BOOL ov67_021E5968(OverlayManager *appMan, int *state);

BOOL ov67_021E5968(OverlayManager *appMan, int *state) {
    Ov67App *app = OverlayManager_GetData(appMan);

    OverlayManager_FreeData(appMan);
    Heap_Destroy(app->heapID);
    return 1;
}
