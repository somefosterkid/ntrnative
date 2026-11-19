#include "global.h"

#include "heap.h"
#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0223F830(UnkStruct_0208C06C *arg0) {
    TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)(arg0->unk_860) + 0x608));
    return 0;
}

s32 ov40_0222BF80(void *, s32);

s32 ov40_0224222C(UnkStruct_0208C06C *param0);

s32 ov40_0223F848(void *arg0) {
    if (ov40_0224222C(arg0) != 0) {
        ov40_0222BF80(arg0, 0x12);
    }
    return 0;
}

s32 ov40_02242CFC(UnkStruct_0208C06C *param0);

s32 ov40_0223F860(void *arg0) {
    void *ptr;

    ptr = ((UnkStruct_0208C06C *)arg0)->unk_860;
    if (ov40_02242CFC(arg0) != 0) {
        Heap_Free(ptr);
        return 1;
    }
    return 0;
}
