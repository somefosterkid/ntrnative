#include "global.h"

#include "field/overlay_01_021FB878.h"

void ov02_02246048(void *arg0);

void ov02_02246048(void *arg0) {
    f32 var_r0;
    s32 var_r4;
    s32 var_r6;
    u16 temp_r0;

    var_r4 = 0;
    if ((s32) * (u8 *)((u8 *)arg0 + 0x9C) > 0) {
        var_r6 = 0;
        do {
            temp_r0 = *(u16 *)((u8 *)arg0 + 0x9E);
            if (temp_r0 != 0) {
                var_r0 = 0.5f + (f32)(temp_r0 << 0xC);
            } else {
                var_r0 = (f32)(temp_r0 << 0xC) - 0.5f;
            }
            Field3dModelAnimation_FrameAdvanceAndLoop((struct Field3DModelAnimation *)(*(u8 **)((u8 *)arg0 + 0x88) + var_r6), (s32)var_r0);
            var_r4 += 1;
            var_r6 += 0x14;
        } while (var_r4 < (s32) * (u8 *)((u8 *)arg0 + 0x9C));
    }
    Field3dObject_Draw(arg0 + 0x10);
}
