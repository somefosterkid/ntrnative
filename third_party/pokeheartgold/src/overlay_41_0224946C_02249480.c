#include "global.h"

#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

// Task data of the 4-frame slide of the selected node: unk_20 frames left, (unk_8, unk_C) the per-frame step towards
// (unk_10, unk_14).
typedef struct Ov41SlideTask {
    UnkStruct_ov22_02257F50 *unk_0;
    UnkStruct_ov22_02259560 *unk_4;
    int unk_8;
    int unk_C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int *unk_24;
    u32 *unk_28;
} Ov41SlideTask; // size: 0x2c

static void ov41_022494F4(SysTask *task, void *data);

void ov41_02249B44(UnkStruct_ov22_02259560 *, int *, int *);

void ov41_02249480(UnkStruct_ov22_02258EE4 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov41_02249480(UnkStruct_ov22_02258EE4 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 v2;
    s32 v1;
    s32 v0;
    Ov41SlideTask *v3;

    v0 = arg3;
    v3 = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_022494F4, sizeof(Ov41SlideTask), 0U, HEAP_ID_13));
    v3->unk_0 = arg0->unk_08;
    v3->unk_4 = arg0->unk_10;
    v3->unk_20 = arg1;
    v3->unk_10 = arg2;
    v3->unk_14 = v0;
    v3->unk_18 = arg4;
    v3->unk_1C = arg5;
    v3->unk_24 = &arg0->unk_08->unk_00.unk_3C;
    v3->unk_28 = &arg0->unk_30;
    ov41_02249B44(arg0->unk_10, &v2, &v1);
    v3->unk_8 = (v3->unk_10 - v2) / arg1;
    v3->unk_C = (v3->unk_14 - v1) / arg1;
    arg0->unk_30 = 0;
}

void ov41_022463FC(void);

void ov41_022486C4(UnkStruct_ov22_02257F50 *, s32, s32, UnkStruct_ov22_02259560 *);

void ov41_02248724(UnkStruct_ov22_02257F50 *);

void ov41_02249AF4(UnkStruct_ov22_02259560 *, s32, s32);

static void ov41_022494F4(SysTask *arg0, void *data) {
    Ov41SlideTask *arg1 = data;
    s32 v1;
    s32 v0;
    s32 v2;

    if (*arg1->unk_24 != 1) {
        ov41_02249B44(arg1->unk_4, &v1, &v0);
        v1 += arg1->unk_8;
        v0 += arg1->unk_C;
        v2 = arg1->unk_20 - 1;
        arg1->unk_20 = v2;
        if ((v2 < 0) || ((arg1->unk_8 == 0) && (arg1->unk_C == 0))) {
            ov41_02249AF4(arg1->unk_4, arg1->unk_10, arg1->unk_14);
            ov41_022486C4(arg1->unk_0, arg1->unk_18, arg1->unk_1C, arg1->unk_4);
            ov41_02248724(arg1->unk_0);
            if (*arg1->unk_28 == 0) {
                ov41_022463FC();
            }
            DestroySysTaskAndEnvironment(arg0);
            return;
        }
        ov41_02249AF4(arg1->unk_4, v1, v0);
    }
}
