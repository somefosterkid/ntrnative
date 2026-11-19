#include "global.h"

#include "error_handling.h"

s32 ov45_0222DE3C(void *, s32, s32, s32);

s32 ov45_0222DEA4(u8 *, s32);

void ov45_0222DE74(void *arg0, s32 arg1);

void ov45_0222DE74(void *arg0, s32 arg1) {
    ov45_0222DE3C(arg0, arg1, 2, 0);
    ov45_0222DEA4(arg0, 2);
}

typedef enum {
    UnkEnum_ov66_02231E54_00 = 0,
    UnkEnum_ov66_02231E54_01 = 1 << 0,
    UnkEnum_ov66_02231E54_02 = 1 << 1,
} UnkEnum_ov66_02231E54;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

void ov45_0222DE8C(UnkStruct_ov66_02231D38 *param0, u32 param1, UnkEnum_ov66_02231E54 param2);

void ov45_0222DE8C(UnkStruct_ov66_02231D38 *param0, u32 param1, UnkEnum_ov66_02231E54 param2) {
    GF_ASSERT(param1 < 20);
    param0->unk_00[param1].unk_02 = param2;
}
