#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"

SysTask *ov99_021E5B74(void *);

FS_EXTERN_OVERLAY(OVY_98);

s32 PokeathlonCourseRecord_Exit(OverlayManager *man, s32 *state);

s32 PokeathlonCourseRecord_Exit(OverlayManager *man, s32 *state) {
    void **v0;

    v0 = OverlayManager_GetData(man);
    SetKeyRepeatTimers(4, 8);
    ov99_021E5B74(*v0);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return 1;
}
