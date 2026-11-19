#include "global.h"

#include "battle/trainer_intro_data.h"
#include "heap.h"
#include "overlay_12_022378C0.h"
#include "overlay_manager.h"

s32 ov12_0223A5E4(OverlayManager *man) {
    s32 i;
    BattleSetup *setup;
    TrainerIntroData *intro;

    setup = OverlayManager_GetArgs(man);
    intro = setup->trainerIntroData;
    if (intro->isDone != 0) {
        for (i = 0; i < 4; i++) {
            if (intro->trainerNames[i] != NULL) {
                Heap_Free(intro->trainerNames[i]);
            }
        }
        Heap_Free(intro);
        return 1;
    }
    return 0;
}
