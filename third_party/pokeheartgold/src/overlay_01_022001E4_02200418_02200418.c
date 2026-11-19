#include "global.h"
#include "billboard.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_unk_struct_ov5_021f5a34.h"

struct FieldEffectManager;

struct MapObject;

struct SysTask;

struct SysTaskQueue;

typedef struct MapObject MapObject;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

BOOL PlaySE(u16 seqID);

typedef struct {
    int unk_00;
    FieldEffectManager *unk_04;
    UnkStruct_ov5_021F5A34 *unk_08;
    MapObject *unk_0C;
} UnkStruct_ov5_021F5D8C;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    VecFx32 unk_18;
    VecFx32 unk_24;
    UnkStruct_ov5_021F5D8C unk_30;
    Billboard *unk_40;
} UnkStruct_ov5_021F5C4C;

void ov5_021F5A7C(UnkStruct_ov5_021F5A34 *param0, int param1);

void ov5_021F5BF0(UnkStruct_ov5_021F5A34 *param0, int param1);

Billboard *ov5_021F5C18(FieldEffectManager *param0, int param1, VecFx32 *param2, u32 param3);

int ov01_02200418(OverworldAnimManager *param0, void *param1);

void ov01_0220039C(UnkStruct_ov5_021F5A34 *param0, int param1);

Billboard *ov01_022003C4(FieldEffectManager *param0, int param1, VecFx32 *param2);

void ov01_02200238(UnkStruct_ov5_021F5A34 *param0, int param1);

void sub_02023EA4(void *, u8 arg1);

int ov01_02200418(OverworldAnimManager *param0, void *param1) {
    VecFx32 v0;
    UnkStruct_ov5_021F5C4C *v1;
    const UnkStruct_ov5_021F5D8C *v2;

    v1 = param1;
    v2 = sub_02068D98(param0);

    v1->unk_30 = *v2;
    v1->unk_24.y = (((fx32)0x0000000000001000L) * 6);

    ov01_0220039C(v1->unk_30.unk_08, v1->unk_30.unk_00);

    sub_02068DB8(param0, &v0);
    v1->unk_40 = ov01_022003C4(v1->unk_30.unk_04, v1->unk_30.unk_00, &v0);
    ov01_02200238(v1->unk_30.unk_08, v1->unk_30.unk_00);
    sub_02023EA4(v1->unk_40, 0);

    if (sub_02068D90(param0) == 1) {
        PlaySE(1501);
    }

    return 1;
}
