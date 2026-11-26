#include "global.h"

#include "overlay_01_fishing_task.h"

#include "field_types_def.h"
#include "follow_mon.h"

extern s32 ov01_02208D88;

u8 ov01_021FCCB0(FishingTask *arg0);

u8 ov01_021FCCB0(FishingTask *arg0) {
    s32 temp_r0;
    s32 var_r3;
    u8 temp_r1;

    if (FollowMon_IsActive(arg0->fieldSystem) != 0) {
        temp_r0 = (s32)FieldSystem_UnkSub108_GetMonMood(arg0->fieldSystem->unk108);
        if (temp_r0 <= -0xA) {
            var_r3 = 0;
        } else if ((temp_r0 >= -9) && (temp_r0 <= 9)) {
            var_r3 = 1;
        } else if ((temp_r0 >= 0xA) && (temp_r0 < 0x32)) {
            var_r3 = 2;
        } else if ((temp_r0 >= 0x32) && (temp_r0 < 0x64)) {
            var_r3 = 3;
        } else {
            var_r3 = 4;
        }
        temp_r1 = (u8)arg0->rodType;
        if ((u32)temp_r1 > 2U) {
            GF_AssertFail();
            return 0U;
        }
        return (u8) * (s32 *)((temp_r1 * 4) + (((u8 *)&ov01_02208D88) + (var_r3 * 0xC)));
    }
    return 0U;
}
