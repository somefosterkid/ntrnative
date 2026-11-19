#include "global.h"

#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_02242490(UnkStruct_0208C06C *param0);

s32 ov40_0223FD38(void *arg0) {
    if (ov40_02242490(arg0) != 0) {
        ov40_0222BF80(arg0, 0x1A);
    }
    return 0;
}

s32 ov40_0223FD50(UnkStruct_0208C06C *arg0) {
    TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)(arg0->unk_860) + 0x610));
    return 0;
}

s32 ov40_0224253C(UnkStruct_0208C06C *param0);

s32 ov40_0223FD68(void *arg0) {
    if (ov40_0224253C(arg0) != 0) {
        ov40_0222BF80(arg0, 0x17);
    }
    return 0;
}

s32 ov40_022425E8();

s32 ov40_0223FD80(void) {
    ov40_022425E8();
    return 0;
}
