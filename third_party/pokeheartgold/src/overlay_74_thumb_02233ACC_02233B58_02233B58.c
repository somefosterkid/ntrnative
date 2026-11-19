#include "global.h"

typedef struct {
    u8 unk_00[4084];
    u16 unk_FF4;
    u16 unk_FF6;
    u32 unk_FF8;
    u32 unk_FFC;
} UnkStruct_ov97_02235F4C;

#include "pm_agb_save.h"

u16 ov74_02233A88(u32 *param0, u16 param1);

void ov74_02233AB8(int param0, void *param1);

u16 ov74_02233ACC(int gbaSectorID);

u16 ov74_02233B58(int param0, void *param1, u32 *param2);

u16 ov74_02233B58(int param0, void *param1, u32 *param2) {
    u32 v0 = 0;
    u16 v1 = 0;
    u16 v2;
    UnkStruct_ov97_02235F4C *v3 = (UnkStruct_ov97_02235F4C *)param1;
    int v4;

    *param2 = 0;
    ov74_0223D33C.sectorsWritten = 0;

    for (v4 = 0; v4 < 14; v4++) {
        ov74_02233AB8(v4 % 14 + param0 * 14, (void *)v3);
        DC_FlushRange(v3, 0x1000);

        if (v3->unk_FF8 == 0x8012025) {
            v1 = 1;
            v2 = ov74_02233A88((u32 *)v3->unk_00, ov74_02233ACC(v3->unk_FF4));

            if (v3->unk_FF6 == v2) {
                *param2 = v3->unk_FFC;
                v0 |= (0x1 << v3->unk_FF4);
            }
        }
    }

    if (v1) {
        if (v0 == ((0xffffffff << 14) ^ 0xffffffff)) {
            return 0x1;
        } else {
            return 0xff;
        }
    } else {
        return 0x0;
    }
}
