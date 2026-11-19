#include "global.h"

#include "error_handling.h"

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

void ov45_0222DE3C(UnkStruct_ov66_02231D38 *param0, u32 param1, u32 param2, UnkEnum_ov66_02231E54 param3);

void ov45_0222DE3C(UnkStruct_ov66_02231D38 *param0, u32 param1, u32 param2, UnkEnum_ov66_02231E54 param3) {
    GF_ASSERT(param1 < 20);
    param0->unk_00[param1].unk_00 = param2;
    param0->unk_00[param1].unk_02 = param3;
}
