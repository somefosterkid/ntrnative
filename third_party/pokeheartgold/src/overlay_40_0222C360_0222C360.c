#include "global.h"

#include "constants/heap.h"

#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222C23C(u32 index, u32 event, void *arg);

void ov40_0222C360(UnkStruct_0208C06C *callbackArg) {
    s32 v0;
    void *v1;
    void *v2;

    v0 = 0;
    v1 = callbackArg->unk_14.unk_1C;
    v2 = callbackArg;
    do {
        v0 += 1;
        *(void **)((u8 *)v2 + 0x60C) = v1;
        v1 += 4;
        v2 += 0x28;
    } while (v0 < 5);
    callbackArg->unk_14.unk_18 = TouchHitboxController_Create(callbackArg->unk_14.unk_1C, 5U, ov40_0222C23C, callbackArg, HEAP_ID_109);
}
