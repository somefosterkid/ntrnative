#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_45_wifi_plaza_system.h"

BOOL ov45_0222D724(const UnkStruct_ov66_02231560 *param0, u32 param1);

u32 ov48_0225B13C(const UnkStruct_ov66_02231560 *param0);

u32 ov48_0225B13C(const UnkStruct_ov66_02231560 *param0) {
    int v0;
    u8 v1;

    for (v0 = 0; v0 < 50; v0++) {
        v1 = ov45_0222D724(param0, (u8)v0);

        if (v1 == 1) {
            return v0;
        }
    }

    return 50;
}
