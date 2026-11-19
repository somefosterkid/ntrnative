#include "global.h"

#include "bg_window.h"
#include "sys_task.h"
#include "sys_task_api.h"

typedef struct {
    BgConfig *unk_00;
    u32 unk_04;
    int unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    u16 unk_18;
    u16 unk_1A;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4E8C;

SysTask *ov102_021E93D4(SysTaskFunc param0, void *param1, int param2);

void ov102_021EC338(SysTask *param0, void *param1);

BOOL ov102_021EC2EC(UnkStruct_ov20_021D4E8C *param0);

BOOL ov102_021EC2EC(UnkStruct_ov20_021D4E8C *param0) {
    switch (param0->unk_1A) {
    case 0:
        param0->unk_1C = ov102_021E93D4(ov102_021EC338, param0, 0);
        param0->unk_1A++;
        break;
    case 1:
        if (param0->unk_18 == 0) {
            SysTask_Destroy(param0->unk_1C);
            param0->unk_1A++;
            return 1;
        }
        break;
    case 2:
        return 1;
    }

    return 0;
}
