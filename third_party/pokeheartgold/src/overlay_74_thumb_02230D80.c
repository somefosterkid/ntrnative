#include "global.h"

typedef struct {
    u16 unk_00[12];
} UnkStruct_ov97_02233268_sub2_sub1;
typedef struct {
    union {
        u8 val1[4];
        u32 val2;
    } unk_00;
} UnkStruct_ov97_022335A8;
typedef struct {
    union {
        u8 val1[2];
        u16 val2;
    } unk_00;
} UnkStruct_ov97_022335C8;
typedef struct {
    UnkStruct_ov97_022335A8 unk_00;
    UnkStruct_ov97_02233268_sub2_sub1 unk_04;
    UnkStruct_ov97_02233268_sub2_sub1 unk_1C;
    UnkStruct_ov97_022335C8 unk_34;
    u8 unk_36;
    u8 unk_37;
} UnkStruct_ov97_02232620_sub1;
typedef struct {
    UnkStruct_ov97_02232620_sub1 unk_00[8];
    u16 unk_1C0;
    u8 unk_1C2;
    u8 unk_1C3_0 : 4;
    u8 unk_1C3_4 : 4;
} UnkStruct_ov97_02232620;
u8 ov74_02231094(void);
UnkStruct_ov97_02232620 *ov74_02231154(void);

u32 ov74_02230D80(void);

u32 ov74_02230D80(void) {
    int v0 = ov74_02231094();
    u32 v1 = 0;

    if (v0 == 1) {
        UnkStruct_ov97_02232620 *v2 = ov74_02231154();

        if (v2->unk_1C3_4 == 1) {
            u32 v3;

            for (v3 = 0; v3 < 8; v3++) {
                if (v2->unk_00[v3].unk_00.unk_00.val2 != 0) {
                    v1++;
                }
            }
        }
    }

    return v1;
}
