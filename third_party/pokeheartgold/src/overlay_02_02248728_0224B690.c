#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "map_object.h"
#include "overworld_anim_manager.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

void *ov02_0224B690(enum HeapID arg0, u32 arg1);

struct HMCutIn;
int ov02_0224B6D0(OverworldAnimManager *param0, void *hmCutInWithPlayer2);

void *ov02_0224B690(enum HeapID heapID, u32 size) {
    void *v0;

    v0 = Heap_AllocAtEnd(heapID, size);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    memset(v0, 0, size);
    return v0;
}

void ov02_0224B6B0(HMCutIn *arg0, s32 arg1);

void ov02_0224B6B0(HMCutIn *arg0, s32 set) {
    MapObject_UnpauseMovement(arg0->playerObject);
    MapObject_SetVisible(arg0->playerObject, set);
}

int ov02_0224B6D0(OverworldAnimManager *param0, void *hmCutInWithPlayer2) {
    const HMCutInWithPlayer2 *wrapper = sub_02068D98(param0);
    HMCutInWithPlayer2 *destWrapper = hmCutInWithPlayer2;

    *destWrapper = *wrapper;
    return 1;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_02_02248728_0224AC38_022533FC.s (same label), with the context size from the type, not the DS literal.
void ov02_0224B6E4(OverworldAnimManager *param0, void *hmCutInWithPlayer2);
extern const OverworldAnimManagerFuncs ov02_02253440;
const OverworldAnimManagerFuncs ov02_02253440 = OVERWORLD_ANIM_MANAGER_FUNCS(HMCutInWithPlayer2, ov02_0224B6D0, sub_02068DD4, ov02_0224B6E4, sub_02068DD0);
#endif
