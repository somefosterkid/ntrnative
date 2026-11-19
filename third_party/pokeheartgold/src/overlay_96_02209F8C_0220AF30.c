#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"

void ov96_0220AF30(s32 *param_1);

void ov96_0220AF30(s32 *param_1) {
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (*param_1 == 0) {
        GF_AssertFail();
    }
    if (param_1[1] == 0) {
        GF_AssertFail();
    }
    Sprite_DeleteAndFreeResources((ManagedSprite *)*param_1);
    Sprite_DeleteAndFreeResources((ManagedSprite *)param_1[1]);
    Heap_Free(param_1);
    return;
}
