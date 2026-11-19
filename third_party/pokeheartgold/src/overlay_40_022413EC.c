#include "global.h"

#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"


s32 ov40_0222BF80(void *, s32);
s32 ov40_02242AEC(UnkStruct_0208C06C *param0);
s32 ov40_022428D4();
s32 ov40_02242490(UnkStruct_0208C06C *param0);
s32 ov40_0224253C(UnkStruct_0208C06C *param0);
s32 ov40_022425E8();

s32 ov40_022413EC(void *arg0) {
    TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)(*(void **)((u8 *)arg0 + 0x860)) + 0x60C));
    return 0;
}

s32 ov40_02241404(void *arg0) {
    if (ov40_02242AEC(arg0) != 0) {
        ov40_0222BF80(arg0, 8);
    }
    return 0;
}

s32 ov40_0224141C(void *arg0) {
    if (ov40_022428D4() != 0) {
        ov40_0222BF80(arg0, 8);
    }
    return 0;
}

s32 ov40_02241434(void *arg0) {
    if (ov40_02242490(arg0) != 0) {
        ov40_0222BF80(arg0, 0x10);
    }
    return 0;
}

s32 ov40_0224144C(void *arg0) {
    TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)(*(void **)((u8 *)arg0 + 0x860)) + 0x610));
    return 0;
}

s32 ov40_02241464(void *arg0) {
    if (ov40_0224253C(arg0) != 0) {
        ov40_0222BF80(arg0, 0xD);
    }
    return 0;
}

s32 ov40_0224147C(void) {
    ov40_022425E8();
    return 0;
}
