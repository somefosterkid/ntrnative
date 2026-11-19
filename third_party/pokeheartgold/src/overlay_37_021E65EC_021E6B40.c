#include "global.h"

typedef struct UnkStruct_ov37_021E6B40 {
    u8 unk_0[0x4380];
    u8 unk_4380;
    u8 unk_4381[0x5039];
    s16 unk_93BA;
} UnkStruct_ov37_021E6B40;

s32 ov37_021E68AC();

s32 ov37_021E6B40(UnkStruct_ov37_021E6B40 *arg0, s32 arg1);

s32 ov37_021E6B40(UnkStruct_ov37_021E6B40 *arg0, s32 arg1) {
    arg0->unk_4380 = (u8)(arg0->unk_4380 & ~0x38);
    arg0->unk_93BA = 0;
    ov37_021E68AC();
    return arg1;
}
