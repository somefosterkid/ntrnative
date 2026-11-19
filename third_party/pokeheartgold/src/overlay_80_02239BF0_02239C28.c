#include "global.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "sys_task.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct BattleFrontier;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_ov63_0222BB38_t;

struct UnkStruct_ov63_0222BEC0_t;

typedef struct BattleFrontier BattleFrontier;

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct {
    const UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222BB38 *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u8 unk_12;
    u8 unk_13;
} UnkStruct_ov63_0222CE44;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02[7];
} UnkStruct_ov104_0223D8F0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C;
    UnkStruct_ov104_0223D8F0 unk_0E;
} UnkStruct_ov104_0223D570;

typedef BOOL (*UnkFuncPtr_ov104_022418A8)(BattleFrontier *, UnkStruct_ov104_0223C634 *);

BOOL ov80_02239C80(BattleFrontier *param0, UnkStruct_ov104_0223C634 *param1);

extern const UnkFuncPtr_ov104_022418A8 ov80_0223DB24[2];

void ov80_02239C28(BattleFrontier *param0, UnkStruct_ov104_0223C634 *param1);

void ov80_02239C28(BattleFrontier *param0, UnkStruct_ov104_0223C634 *param1) {
    int v0;

    if (ov80_0223DB24[param1->unk_26.unk_00] == NULL) {
        return;
    }

    v0 = ov80_0223DB24[param1->unk_26.unk_00](param0, param1);

    if (v0 == 1) {
        MI_CpuClear8(&param1->unk_26, sizeof(UnkStruct_ov104_0223D8F0));
    }
}
