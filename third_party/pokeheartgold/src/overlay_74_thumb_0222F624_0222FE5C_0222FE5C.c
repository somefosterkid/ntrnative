#include "global.h"

typedef struct {
    union {
        u8 val1[4];
        u32 val2;
    } unk_00;
} UnkStruct_ov97_022335A8;

UnkStruct_ov97_022335A8 ov74_022311F4(u8 *param0);

UnkStruct_ov97_022335A8 ov74_0222FE5C(u8 *param0);

UnkStruct_ov97_022335A8 ov74_0222FE5C(u8 *param0) {
    return ov74_022311F4(param0);
}

typedef struct {
    union {
        u8 val1[2];
        u16 val2;
    } unk_00;
} UnkStruct_ov97_022335C8;

UnkStruct_ov97_022335C8 ov74_02231214(u8 *param0);

UnkStruct_ov97_022335C8 ov74_0222FE68(u8 *param0);

UnkStruct_ov97_022335C8 ov74_0222FE68(u8 *param0) {
    return ov74_02231214(param0);
}
