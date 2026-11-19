#include "global.h"

#include "error_handling.h"
#include "overlay_42.h"
#include "sys_task.h"

struct UnkStruct_ov63_0222BEC0_t;

typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;

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

typedef struct {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov42_0222903C *sprite;
    UnkStruct_ov104_0223D570 unk_08;
    UnkStruct_ov104_0223D8F0 unk_26;
    u8 pad_36[2];
    SysTask *movementTask;
} UnkStruct_ov104_0223C634;

void ov80_02239900(UnkStruct_ov104_0223C634 *param0, UnkStruct_ov104_0223D570 *param1);

void ov80_02239900(UnkStruct_ov104_0223C634 *param0, UnkStruct_ov104_0223D570 *param1) {
    *param1 = param0->unk_08;
}

struct BattleFrontier;

typedef struct BattleFrontier BattleFrontier;

UnkStruct_ov104_0223C634 *sub_0209686C(BattleFrontier *frontier, int param1);

void ov80_02239914(BattleFrontier *param0, int param1, UnkStruct_ov63_0222BEC0 *param2, UnkStruct_ov42_0222903C *param3, const UnkStruct_ov104_0223D570 *param4);

void ov80_02239914(BattleFrontier *param0, int param1, UnkStruct_ov63_0222BEC0 *param2, UnkStruct_ov42_0222903C *param3, const UnkStruct_ov104_0223D570 *param4) {
    UnkStruct_ov104_0223C634 *v0 = sub_0209686C(param0, param1);

    v0->unk_00 = param2;
    v0->sprite = param3;
    v0->unk_08 = *param4;
}

UnkStruct_ov104_0223C634 *sub_02096868(BattleFrontier *frontier);

UnkStruct_ov104_0223C634 *ov80_02239938(BattleFrontier *param0, int param1);

UnkStruct_ov104_0223C634 *ov80_02239938(BattleFrontier *param0, int param1) {
    int v0;
    UnkStruct_ov104_0223C634 *v1 = sub_02096868(param0);

    for (v0 = 0; v0 < 32; v0++) {
        if ((v1->unk_00 != NULL) && (v1->unk_08.unk_04 == param1)) {
            return v1;
        }

        v1++;
    }

    GF_ASSERT(FALSE);
    return NULL;
}
