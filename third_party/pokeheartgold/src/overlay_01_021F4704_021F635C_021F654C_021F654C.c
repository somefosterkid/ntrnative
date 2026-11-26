#include "global.h"
#include "error_handling.h"
#include "unk_02037C94.h"
#include "overlay_01_map_load_manager.h"
#include "overlay_01_021F4704.h"


#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov01_021F635C(u32, const u8, void *);
#else
s8 ov01_021F635C(u32, s32, void *);
#endif

s32 ov01_021F654C(struct MapLoadManager *arg0, s32 arg1, s32 arg2, s8 *arg3);

s32 ov01_021F654C(MapLoadManager *arg0, s32 arg1, s32 arg2, s8 *arg3) {
    s8 *sp0;
    s32 temp_r1;
    s8 temp_r0;
    u32 temp_r7;
    temp_r1 = *((s32 *)&arg0->width);
    sp0 = arg3;
    temp_r7 = (((s32)(arg1 + (((u32)(arg1 >> 4)) >> 0x1B))) >> 5) + ((arg2 / 0x20) * temp_r1);
    if ((temp_r7 >= ((u32)(temp_r1 * (*((s32 *)&arg0->height))))) && (sub_02039AD8(1) != 0)) {
        return 0;
    }
    if (temp_r7 >= ((u32)((*((s32 *)&arg0->width)) * (*((s32 *)&arg0->height))))) {
        GF_AssertFail();
    }
    temp_r1 = *((s32 *)&arg0->stride);
    temp_r0 = ov01_021F635C(temp_r7, ov01_021F6328(arg1 + (arg2 * temp_r1), temp_r1), arg0);
    if (sp0 != 0L) {
        *sp0 = temp_r0;
    }
    if (((u32)temp_r0) > 3U) {
        return 0;
    }
    return 1;
}
