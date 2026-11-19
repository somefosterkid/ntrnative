#include "global.h"

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;

u32 ov102_021E8798(const UnkStruct_ov20_021D18BC *param0);

int ov102_021E8C64(UnkStruct_ov20_021D18BC *param0);

int ov102_021E8C64(UnkStruct_ov20_021D18BC *param0) {
    if (param0->unk_00 < param0->unk_02) {
        if ((param0->unk_00 + 5) <= param0->unk_02) {
            param0->unk_08 = 5;
            param0->unk_00 += 5;
        } else {
            param0->unk_08 = param0->unk_02 - param0->unk_00;
            param0->unk_00 = param0->unk_02;
        }

        if (ov102_021E8798(param0) < param0->unk_04) {
            return 2;
        } else {
            param0->unk_06 = 0;
            return 3;
        }
    }

    return 4;
}
