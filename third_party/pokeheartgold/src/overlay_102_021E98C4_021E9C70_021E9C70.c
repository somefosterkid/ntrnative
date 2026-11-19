#include "global.h"

typedef struct UnkStruct_ov102_021E9C70_arg1 {
    void *unk_0;
    u8 unk_4[0x10];
    u32 unk_14;
} UnkStruct_ov102_021E9C70_arg1;

typedef struct UnkStruct_ov102_021E9C70_v1 {
    u8 unk_0[0x18];
    void *unk_18;
    u8 unk_1C[0x1C8];
    void *unk_1E4;
    void *unk_1E8;
    void *unk_1EC;
    void *unk_1F0;
} UnkStruct_ov102_021E9C70_v1;

u16 ov102_021E8FD0(void *);

s32 ov102_021E94A4(void *);

s32 ov102_021EB624(void *);

s32 ov102_021EB654(void *);

s32 ov102_021EB98C(u8 *, s32);

s32 ov102_021EB9C0(void *, s32);

s32 ov102_021EBB88(u8 *);

s32 ov102_021EBBC4(void *);

s32 ov102_021EBBD0(void *);

s32 ov102_021EBEA4(void *);

s32 ov102_021EBEC8(u8 *);

s32 ov102_021EBEF4(u8 *, s32);

s32 ov102_021EC290(void *, s32);

void ov102_021E9C70(s32 arg0, UnkStruct_ov102_021E9C70_arg1 *arg1);

void ov102_021E9C70(s32 arg0, UnkStruct_ov102_021E9C70_arg1 *arg1) {
    u32 v0;
    UnkStruct_ov102_021E9C70_v1 *v1;

    v0 = arg1->unk_14;
    v1 = arg1->unk_0;
    switch (v0) {
    case 0:
        ov102_021EB624(v1->unk_1E4);
        arg1->unk_14 = (u32)(arg1->unk_14 + 1);
        return;
    case 1:
        ov102_021EBEF4(v1->unk_1EC, 0);
        ov102_021EC290(v1->unk_1F0, 0);
        ov102_021EBEA4(v1->unk_1EC);
        arg1->unk_14 = (u32)(arg1->unk_14 + 1);
        return;
    case 2:
        if (ov102_021EBEC8(v1->unk_1EC) != 0) {
            ov102_021EBBD0(v1->unk_1E8);
            arg1->unk_14 = (u32)(arg1->unk_14 + 1);
            return;
        }
    default:
        return;
    case 3:
        ov102_021EBB88(v1->unk_1E8);
        arg1->unk_14 = (u32)(arg1->unk_14 + 1);
        return;
    case 4:
        if ((ov102_021EBBC4(v1->unk_1E8) != 0) && (ov102_021EB654(v1->unk_1E4) != 0)) {
            ov102_021EB9C0(v1->unk_1E8, ov102_021E8FD0(v1->unk_18));
            ov102_021EB98C(v1->unk_1E8, 1);
            ov102_021E94A4(arg1);
        }
        break;
    }
}
