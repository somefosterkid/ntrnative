#include "global.h"

#include "battle/battle_system.h"
#include "battle/trainer_intro_data.h"

#include "heap.h"
#include "overlay_12_022378C0.h"
#include "overlay_manager.h"

s32 ov12_0223A3A8(OverlayManager *man) {
    s32 v1;
    BattleSystem *v0;
    String *ptr;

    v0 = OverlayManager_GetData(man);
    if (v0->unk1C4->isDone != 0) {
        v1 = 0;
        do {
            ptr = v0->unk1C4->trainerNames[v1];
            if (ptr != NULL) {
                Heap_Free(ptr);
            }
            v1 += 1;
        } while (v1 < 4);
        Heap_Free(v0->unk1C4);
        return 1;
    }
    return 0;
}
