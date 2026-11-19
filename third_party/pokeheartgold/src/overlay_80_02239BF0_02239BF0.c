#include "global.h"

#include "overlay_42.h"
#include "sys_task.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct BattleFrontier;
struct UnkStruct_ov63_0222BEC0_t;
typedef struct BattleFrontier BattleFrontier;
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
UnkStruct_ov104_0223C634 *ov80_02239938(BattleFrontier *param0, int param1);

extern void ov80_02239BF0(BattleFrontier *param0, u16 param1, u8 param2, s16 param3[], int param4);

void ov80_02239BF0(BattleFrontier *param0, u16 param1, u8 param2, s16 param3[], int param4) {
    UnkStruct_ov104_0223C634 *v0;
    int v1;

    v0 = ov80_02239938(param0, param1);
    MI_CpuClear8(&v0->unk_26, sizeof(UnkStruct_ov104_0223D8F0));

    v0->unk_26.unk_00 = param2;

    for (v1 = 0; v1 < param4; v1++) {
        v0->unk_26.unk_02[v1] = param3[v1];
    }
}
