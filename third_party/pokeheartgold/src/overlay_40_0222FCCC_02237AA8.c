#include "global.h"

#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_02237AA8(UnkStruct_0208C06C *arg0) {
    TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)(arg0->unk_860) + 0x338));
    return 0;
}
