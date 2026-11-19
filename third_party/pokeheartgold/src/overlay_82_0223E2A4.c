#include "global.h"
struct BattleHallApp;

#include "heap.h"
#include "overlay_manager.h"
#include "overlay_82_battle_hall_app.h"

s32 ov82_0223E974(void *);

s32 ov82_0223E2A4(struct BattleHallApp *arg0);

s32 ov82_0223E2A4(BattleHallApp *arg0) {
    if ((arg0->subState == 0) && (OverlayManager_Run(*((OverlayManager * *)&arg0->monSummaryAppMan)) == 1)) {
        *((s32 *)&arg0->monIndex) = (s32) * (u8 *)((u8 *)(*((void * *)&arg0->monSummary)) + 0x14);
        Heap_Free(*((void * *)&arg0->monSummary));
        Heap_Free(*((OverlayManager * *)&arg0->monSummaryAppMan));
        *((OverlayManager * *)&arg0->monSummaryAppMan) = NULL;
        ov82_0223E974(arg0);
        *((s16 *)&arg0->summaryScreenOpen) = 0;
        return 1;
    }
    return 0;
}
