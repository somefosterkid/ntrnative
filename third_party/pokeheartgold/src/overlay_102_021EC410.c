#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"
#include "error_handling.h"
#include "pm_string.h"

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4FF0;

SysTask *ov102_021E93D4(SysTaskFunc param0, void *param1, int param2);

void ov102_021EC478(SysTask *param0, void *param1);

void ov102_021EC410(UnkStruct_ov20_021D4FF0 *param0, int param1, int param2, int param3, int param4);

BOOL ov102_021EC450(UnkStruct_ov20_021D4FF0 *param0);

s32 ov102_021EC4A4(void);

typedef u16 charcode_t;
extern const struct {
    charcode_t charcode;
    u8 x;
    u8 y;
    u8 up;
    u8 down;
    u8 left;
    u8 right;
    u8 top;
    u8 bottom;
} ov102_021EC820[27];
void ov102_021EC4A8(u32 index, String *out);
void ov102_021EC4CC(u32 index, u32 *outX, u32 *outY);
int ov102_021EC4F8(u32 index);

void ov102_021EC410(UnkStruct_ov20_021D4FF0 *param0, int param1, int param2, int param3, int param4) {
    G2_SetBlendBrightness(param1, param2);

    param0->unk_00 = param1;
    param0->unk_14 = param4;
    param0->unk_08 = param2 * ((fx32)0x0000000000001000L);
    param0->unk_0C = param3 * ((fx32)0x0000000000001000L);
    param0->unk_10 = (fx32)(param0->unk_0C - param0->unk_08) / param4;
    param0->unk_18 = 0;
    param0->unk_1C = ov102_021E93D4(ov102_021EC478, param0, 0);
}

BOOL ov102_021EC450(UnkStruct_ov20_021D4FF0 *param0) {
    switch (param0->unk_18) {
    case 0:
        if (param0->unk_14 == 0) {
            SysTask_Destroy(param0->unk_1C);
            param0->unk_18++;
            return 1;
        }
        break;
    default:
        return 1;
    }

    return 0;
}

void ov102_021EC478(SysTask *param0, void *param1) {
    UnkStruct_ov20_021D4FF0 *v0 = param1;
    int v1;

    if (v0->unk_14) {
        v0->unk_08 += v0->unk_10;
        v1 = v0->unk_08 >> 12;
        v0->unk_14--;
    } else {
        v1 = v0->unk_0C >> 12;
    }

    G2_SetBlendBrightness(v0->unk_00, v1);
}

s32 ov102_021EC4A4(void) {
    return 0x1B;
}

void ov102_021EC4A8(u32 index, String *out) {
    GF_ASSERT(index < NELEMS(ov102_021EC820));
    CopyU16ArrayToStringN(out, &(ov102_021EC820[index].charcode), 1 + 1);
}

void ov102_021EC4CC(u32 index, u32 *outX, u32 *outY) {
    GF_ASSERT(index < NELEMS(ov102_021EC820));

    *outX = ov102_021EC820[index].x;
    *outY = ov102_021EC820[index].y;
}

int ov102_021EC4F8(u32 index) {
    GF_ASSERT(index < NELEMS(ov102_021EC820));
    return ov102_021EC820[index].up;
}
