#include "global.h"

typedef struct UnkStruct_ov102_021EA0D0_arg1 {
    void *unk_0;
    s32 unk_4;
} UnkStruct_ov102_021EA0D0_arg1;

typedef struct UnkStruct_ov102_021EA078_arg1 {
    void *unk_0;
    s32 unk_4;
} UnkStruct_ov102_021EA078_arg1;

typedef struct UnkStruct_ov102_021EA0D0_v0 {
    u8 unk_0[0x1E0];
    void *unk_1E0;
    void *unk_1E4;
    u8 unk_1E8[0xC];
    u8 unk_1F4;
} UnkStruct_ov102_021EA0D0_v0;

typedef struct UnkStruct_ov102_021EA078_v0 {
    u8 unk_0[0x1E0];
    void *unk_1E0;
    void *unk_1E4;
    u8 unk_1E8[0xC];
    u8 unk_1F4;
} UnkStruct_ov102_021EA078_v0;

s32 ov102_021E9090(s32);

s32 ov102_021E94A4(void *);

s32 ov102_021EAE40(u8 *, s32);

s32 ov102_021EAF44(void *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov102_021EB0C0(u32 *, u8);
#else
s32 ov102_021EB0C0(u32 *, s32);
#endif

s32 ov102_021EB2EC(void *, s32, s32);

s32 ov102_021EB50C(void *);

void ov102_021EA078(s32 arg0, UnkStruct_ov102_021EA078_arg1 *arg1);

void ov102_021EA078(s32 arg0, UnkStruct_ov102_021EA078_arg1 *arg1) {
    UnkStruct_ov102_021EA078_v0 *v0;

    v0 = arg1->unk_0;
    ov102_021EAE40(v0->unk_1E0, 2);
    ov102_021EB2EC(*(void **)((u8 *)(arg1->unk_0) + 0x1E4), 0, 0);
    if (v0->unk_1F4 == 1) {
        ov102_021EB50C(v0->unk_1E4);
    } else {
        ov102_021EAF44(v0->unk_1E0);
    }
    ov102_021EB0C0(v0->unk_1E0, ov102_021E9090(arg1->unk_4));
    ov102_021E94A4(arg1);
}

void ov102_021EA0D0(s32 arg0, UnkStruct_ov102_021EA0D0_arg1 *arg1);

void ov102_021EA0D0(s32 arg0, UnkStruct_ov102_021EA0D0_arg1 *arg1) {
    UnkStruct_ov102_021EA0D0_v0 *v0;

    v0 = arg1->unk_0;
    ov102_021EAE40(v0->unk_1E0, 3);
    ov102_021EB2EC(*(void **)((u8 *)(arg1->unk_0) + 0x1E4), 0, 0);
    if (v0->unk_1F4 == 1) {
        ov102_021EB50C(v0->unk_1E4);
    } else {
        ov102_021EAF44(v0->unk_1E0);
    }
    ov102_021EB0C0(v0->unk_1E0, ov102_021E9090(arg1->unk_4));
    ov102_021E94A4(arg1);
}
