#include "global.h"

typedef struct UnkStruct_ov102_021EA164_v0 {
    u8 unk_0[0x1E0];
    void *unk_1E0;
    void *unk_1E4;
    u8 unk_1E8[0xC];
    u8 unk_1F4;
} UnkStruct_ov102_021EA164_v0;

void ov102_021E94A4(void *arg0);

s32 ov102_021EAE40(u8 *, s32);

s32 ov102_021EAF50(void *);

s32 ov102_021EB2EC(void *, s32, s32);

s32 ov102_021EB518(void *);

void ov102_021EA164(s32 arg0, void **arg1);

void ov102_021EA164(s32 arg0, void **arg1) {
    UnkStruct_ov102_021EA164_v0 *v0;

    v0 = *arg1;
    ov102_021EAE40(v0->unk_1E0, 0);
    if (v0->unk_1F4 == 1) {
        ov102_021EB2EC(v0->unk_1E4, 1, 1);
        ov102_021EB518(v0->unk_1E4);
    } else {
        ov102_021EB2EC(v0->unk_1E4, 1, 0);
        ov102_021EAF50(v0->unk_1E0);
    }
    ov102_021E94A4(arg1);
}
