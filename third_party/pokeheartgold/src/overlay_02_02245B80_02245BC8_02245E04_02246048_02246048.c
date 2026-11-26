#include "global.h"

#include "field/overlay_01_021FB878.h"
#include "overlay_02_app_02245e68.h"

void ov02_02246048(Ov02App_02245E68 *arg0);

void ov02_02246048(Ov02App_02245E68 *arg0) {
    f32 var_r0;
    s32 var_r4;
    s32 var_r6;
    u16 temp_r0;

    var_r4 = 0;
    if ((s32) * &arg0->animCount > 0) {
        var_r6 = 0;
        do {
            temp_r0 = arg0->animSpeed;
            if (temp_r0 != 0) {
                var_r0 = 0.5f + (f32)(temp_r0 << 0xC);
            } else {
                var_r0 = (f32)(temp_r0 << 0xC) - 0.5f;
            }
            Field3dModelAnimation_FrameAdvanceAndLoop((struct Field3DModelAnimation *)((u8 *)arg0->anims + var_r6), (s32)var_r0);
            var_r4 += 1;
            var_r6 += 0x14;
        } while (var_r4 < (s32) * &arg0->animCount);
    }
    Field3dObject_Draw(&arg0->object);
}
