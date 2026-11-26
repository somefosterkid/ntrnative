#include "global.h"
#include "sys_task.h"

struct BgConfig;
struct SysTask;
struct SysTaskQueue;
typedef struct BgConfig BgConfig;
#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/bg_window.c (tools/pokehg_abi.py)
void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);
#else
void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, u8 bgOffsetUpdateOp, int val);
#endif
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
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

void ov102_021EC338(SysTask *param0, void *param1);

void ov102_021EC338(SysTask *param0, void *param1) {
    UnkStruct_ov20_021D4E8C *v0 = param1;

    if (v0->unk_18) {
        v0->unk_0C += v0->unk_14;
        BgSetPosTextAndCommit(v0->unk_00, v0->unk_04, v0->unk_08, (v0->unk_0C >> 12));
        v0->unk_18--;
    } else {
        BgSetPosTextAndCommit(v0->unk_00, v0->unk_04, v0->unk_08, v0->unk_10);
    }
}
