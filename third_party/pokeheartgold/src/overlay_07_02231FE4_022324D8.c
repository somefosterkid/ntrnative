#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"

enum HeapID ov07_0221BFD0(BattleAnimSystem *);

void *ov07_022324D8(BattleAnimSystem *arg0, u32 size) {
    void *v0;

    if (arg0 == 0) {
        GF_AssertFail();
    }
    v0 = Heap_Alloc(ov07_0221BFD0(arg0), size);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    memset(v0, 0, size);
    return v0;
}

void ov07_02232508(void *arg0);

void ov07_02232508(void *ptr) {
    if (ptr == NULL) {
        GF_AssertFail();
    }
    Heap_Free(ptr);
}

s32 ov07_0223251C(s32 arg0);

s32 ov07_0223251C(s32 arg0) {
    if ((arg0 >= 1) && (arg0 <= 0x18)) {
        return arg0 - 1;
    }
    if ((arg0 >= 0x3E8) && (arg0 <= 0x403)) {
        return arg0 - 0x3E8;
    }
    return 3;
}
