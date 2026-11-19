#include "global.h"

typedef struct UnkStruct_ov102_021EBEA4 {
    u8 unk_0[0x40];
    u8 unk_40;
    u8 unk_41[0x3F];
    s32 unk_80;
} UnkStruct_ov102_021EBEA4;

typedef struct UnkStruct_ov102_021EBE80 {
    u8 unk_0[0x40];
    u8 unk_40;
    u8 unk_41[0x3F];
    s32 unk_80;
} UnkStruct_ov102_021EBE80;

s32 ov102_021EC3AC(void *);

s32 ov102_021EBE80(UnkStruct_ov102_021EBE80 *arg0);

s32 ov102_021EBE80(UnkStruct_ov102_021EBE80 *arg0) {
    if (arg0->unk_80 == 0) {
        if (ov102_021EC3AC((void *)&arg0->unk_40) != 0) {
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 ov102_021EC37C(void *, s32, s32, s32, s32, s32);

void ov102_021EBEA4(UnkStruct_ov102_021EBEA4 *arg0);

void ov102_021EBEA4(UnkStruct_ov102_021EBEA4 *arg0) {
    arg0->unk_80 = 0;
    ov102_021EC37C((void *)&arg0->unk_40, 4, 0x3F, 0x7C, 0, 0xC);
}
