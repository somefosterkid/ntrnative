#include "global.h"

#include "frontier.h"
#include "overlay_manager.h"
#include "frontier_work.h"

s32 Frontier_FreeMap(void *);

s32 Frontier_UnloadOverlays(void);

s32 ov80_0222A920(void *);

s32 Frontier_Exit(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    ov80_0222A920(*((void **)&((FrontierWork *)v0)->system));
    Frontier_FreeMap(v0);
    OverlayManager_FreeData(man);
    Frontier_UnloadOverlays();
    return 1;
}
