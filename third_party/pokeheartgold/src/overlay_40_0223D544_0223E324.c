#include "global.h"

#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

s32 ov40_0223E324(UnkStruct_0208C06C *arg0);

s32 ov40_0223E324(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov62_02241204 *v0 = arg0->unk_860;

    TouchHitboxController_IsTriggered(v0->unk_608[0]);
    return 0;
}
