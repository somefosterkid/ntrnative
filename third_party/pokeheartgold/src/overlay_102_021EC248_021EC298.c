#include "global.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct SysTask;

struct SysTaskQueue;

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

void ov102_021EC298(UnkStruct_ov20_021D4E8C *param0, BgConfig *param1, u32 param2, int param3, int param4, int param5);

void ov102_021EC298(UnkStruct_ov20_021D4E8C *param0, BgConfig *param1, u32 param2, int param3, int param4, int param5) {
    param0->unk_00 = param1;
    param0->unk_04 = param2;

    if (param3 == 0) {
        param0->unk_0C = Bg_GetXpos((struct BgConfig *)((param1)), (enum GFBgLayer)(param2));
        param0->unk_08 = 0;
    } else {
        param0->unk_0C = Bg_GetYpos((struct BgConfig *)((param1)), (enum GFBgLayer)(param2));
        param0->unk_08 = 3;
    }

    param0->unk_10 = (param0->unk_0C + param4) & 0x1ff;
    param0->unk_0C *= ((fx32)0x0000000000001000L);
    param0->unk_14 = (param4 * ((fx32)0x0000000000001000L)) / param5;
    param0->unk_18 = param5;
    param0->unk_1A = 0;
}
