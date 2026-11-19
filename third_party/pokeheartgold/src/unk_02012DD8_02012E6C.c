#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_struct_ov66_022342_f4.h"
#include "bg_window.h"
#include "screen_fade_internal.h"

void sub_02011068(HardwareWindowSettings *param0, int param1, enum GFScreen screen, int param3);

void sub_020131AC(void *arg0);

s32 sub_0200FF88(u8 *, void *, s32 *, enum GFScreen, s32);

s32 sub_02010E64(void *, int, enum GFScreen, s32);

void *sub_02010EE0(void *, s32);

void sub_02010F00(SysTask *task, void *data);

s32 sub_02010F84(HardwareWindowSettings *, u32, u32, u32, enum GFScreen, s32, s32, s32, s32, s32);

extern s32 sub_02010C38;

void sub_02012E6C(void *arg0, void *arg1, s32 arg2, s32 arg3, enum GFScreen arg4, HardwareWindowSettings *arg5, s32 arg6, s32 arg7);

void sub_02012E6C(void *arg0, void *arg1, s32 arg2, s32 arg3, enum GFScreen arg4, HardwareWindowSettings *arg5, s32 arg6, s32 arg7) {
    s32 v1;
    s32 v0;
    void *v2;

    v1 = arg3;
    v0 = arg2;
    sub_02010E64(arg0, *(u8 *)((u8 *)arg1 + 8), arg4, arg7);
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_14) = 0x7FFF80;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_C) = (s32) * (s16 *)arg1;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_10) = (s32) * (s16 *)((u8 *)arg1 + 2);
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_18) = (s32) * (s32 *)((u8 *)arg1 + 4);
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_1C) = (s32) * (s32 *)((u8 *)arg1 + 4);
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_20) = (s32)((s32)(0x7FFF80U >> 7) / v0);
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_24) = v0;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_28) = v1;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_2C) = 0;
    *((HardwareWindowSettings **)&((UnkStruct_ov66_022342F4 *)arg0)->unk_44) = arg5;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_48) = arg6;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_30) = arg7;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_34) = (s32) * (u8 *)((u8 *)arg1 + 0xB);
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_38) = (s32) * (u8 *)((u8 *)arg1 + 8);
    *((enum GFScreen *)&((UnkStruct_ov66_022342F4 *)arg0)->pad_3C[0]) = arg4;
    *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_40) = 1;
    sub_020131AC(arg0);
    SysTask_CreateOnVWaitQueue(sub_02010F00, arg0, 0x3FFU);
    v2 = sub_02010EE0(arg0, 0);
    sub_02010F84(arg5, *(u8 *)((u8 *)arg1 + 9), *(u8 *)((u8 *)arg1 + 0xA), *(u8 *)((u8 *)arg1 + 8), arg4, (s32) * (s16 *)((u8 *)v2 + 0x300), 0, (s32) * (s16 *)((u8 *)v2 + 0x480), 0xC0, *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_34));
    if (*(u8 *)((u8 *)arg1 + 8) == 0) {
        sub_02011068(arg5, 1, arg4, *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_34));
    } else {
        sub_02011068(arg5, 2, arg4, *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_34));
    }
    sub_0200FF88(*((void **)&((UnkStruct_ov66_022342F4 *)arg0)->unk_48), arg0, &sub_02010C38, arg4, arg7);
}
