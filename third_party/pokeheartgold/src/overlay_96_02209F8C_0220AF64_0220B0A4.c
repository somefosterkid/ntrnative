#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"

extern s32 ov96_0221CDA0;

extern s32 ov96_0221DC68;

s32 ov96_0220B0A4(void *arg0, s32 arg1);

s32 ov96_0220B0A4(void *arg0, s32 arg1) {
    s32 var_r0;
    s32 sp4[3];
    s16 sp0[2];
    s16 temp_r3;
    s16 temp_r2;
    s8 *var_r1;
    s8 *var_r7;
    u8 temp_r5;
    u8 temp_r6;
    void *var_r4;

    var_r1 = (s8 *)&sp4[0];
    var_r1[0] = 0;
    var_r1[1] = 0;
    var_r1[2] = 0;
    var_r1[3] = 0;
    var_r1[4] = 0;
    var_r1[5] = 0;
    var_r1[6] = 0;
    var_r1[7] = 0;
    var_r1[8] = 0;
    var_r1[9] = 0;
    var_r1[10] = 0;
    var_r1[11] = 0;
    if (arg1 >= 3) {
        GF_AssertFail();
    }
    var_r4 = (void *)(*(void **)((u8 *)&ov96_0221DC68 + (arg1 * 4)));
    temp_r5 = *(u8 *)((u8 *)&ov96_0221CDA0 + arg1) - 1;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)arg0 + 4), &sp0[1], &sp0[0]);
    var_r0 = 0;
    if ((s32)temp_r5 > 0) {
        temp_r2 = sp0[0];
        temp_r3 = sp0[1];
        var_r7 = (s8 *)&sp4[0];
        do {
            var_r0 += 1;
            *(s8 *)var_r7 = (s8)(temp_r2 + *(u8 *)var_r4);
            *(s8 *)((u8 *)var_r7 + 1) = (s8)(temp_r2 + *(u8 *)((u8 *)var_r4 + 1));
            *(s8 *)((u8 *)var_r7 + 2) = (s8)(temp_r3 + *(u8 *)((u8 *)var_r4 + 2));
            temp_r6 = *(u8 *)((u8 *)var_r4 + 3);
            var_r4 += 4;
            *(s8 *)((u8 *)var_r7 + 3) = (s8)(temp_r3 + temp_r6);
            var_r7 += 4;
        } while (var_r0 < (s32)temp_r5);
    }
    *(u8 *)&sp4[temp_r5] = 0xFF;
    if (TouchscreenHitbox_FindRectAtTouchNew((TouchscreenHitbox *)&sp4[0]) != -1) {
        return 1;
    }
    return 0;
}
