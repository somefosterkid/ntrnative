#include "global.h"
#include "simple3dmodel.h"

#include "overlay_01_021F1348.h"
#include "sys_task.h"
#include "gf_rtc.h"
#include "overlay_01_021FD41C.h"

extern const VecFx32 ov01_02208F38[];

extern const fx32 ov01_02208EB4[];

void ov01_021FD4A4(UnkStruct_ov5_021F1388 *param0, int param1);

void ov01_021FD4D0(s32 *arg0, s32 arg1, s32 arg2);

int ov01_021FD498(int param0);
void ov01_021FD4D0(fx32 *param0, fx32 param1, fx32 param2);
void ov01_021FD624(UnkStruct_ov5_021F1388 *param0, int param1);

void ov01_021FD4A4(UnkStruct_ov5_021F1388 *param0, int param1) {
    param0->unk_10 = ov01_02208EB4[param0->unk_04];
    param0->unk_14 = ov01_02208F38[param0->unk_04];
}

void ov01_021FD4D0(s32 *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    s32 v1;
    s32 v2;

    v2 = *arg0;
    if (v2 < arg1) {
        v0 = v2 + arg2;
        *arg0 = v0;
        if (v0 > arg1) {
            *arg0 = arg1;
        }
    } else if (v2 > arg1) {
        v1 = v2 - arg2;
        *arg0 = v1;
        if (v1 < arg1) {
            *arg0 = arg1;
        }
    }
}

void ov01_021FD4F4(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov5_021F1388 *v1 = param1;

    v0 = GF_RTC_GetTimeOfDay();

    switch (v1->unk_00) {
    case 0:
        v1->unk_04 = v0;
        ov01_021FD4A4(v1, v1->unk_04);
        ov01_021FD624(v1, ov01_021FD498(v1->unk_10));
        v1->unk_00++;
        break;
    case 1:
        if (v1->unk_04 == v0) {
            break;
        }

        v1->unk_00++;
    case 2: {
        fx32 v2 = ov01_02208EB4[v0];
        VecFx32 v3 = ov01_02208F38[v0];

        ov01_021FD4D0(&v1->unk_14.x, v3.x, 0x10);
        ov01_021FD4D0(&v1->unk_14.y, v3.y, 0x10);
        ov01_021FD4D0(&v1->unk_14.z, v3.z, 0x10);
        ov01_021FD4D0(&v1->unk_10, v2, 0x200);
        ov01_021FD624(v1, ov01_021FD498(v1->unk_10));

        if ((v3.x == v1->unk_14.x) && (v3.y == v1->unk_14.y) && (v3.z == v1->unk_14.z) && (v2 == v1->unk_10)) {
            v1->unk_04 = v0;
            v1->unk_00 = 1;
        }
    } break;
    }
}
