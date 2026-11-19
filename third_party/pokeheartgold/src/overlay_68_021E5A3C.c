#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
struct MoveRelearner;

s32 ov68_021E5B14(struct MoveRelearner *arg0);

s32 MoveRelearner_Exit(OverlayManager *man, s32 *state);

s32 MoveRelearner_Exit(OverlayManager *man, s32 *state) {
    ov68_021E5B14(OverlayManager_GetData(man));
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_66);
    return 1;
}
