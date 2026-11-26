#include "global.h"
#include "overlay_14.h"
struct UnkStruct_ov14_boxapp;

#include "sprite.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6100(void *, u32, u32);
s32 ov14_021F4A20(void *, u32);

void ov14_021F33E8(UnkStruct_ov14_boxscan *arg0);
s32 ov14_021E7264(void *);
BOOL ov14_021E7278(UnkStruct_ov14_boxapp *self);
s32 ov14_021E765C(void *);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021F0234(void *, s32 *, s32);
s32 ov14_021F08BC(void *);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F4958(void *, u32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F685C(void *, s32, s32, s32);
s32 ov14_021E7588(void *, s32);
s32 ov14_021F08F0(u8 *param_1);
s32 ov14_021F57B8(void *);
s32 ov14_021E8824(UnkStruct_ov14_boxsys *);
void ov14_021F33FC(UnkStruct_ov14_boxscan *arg0);
s32 ov14_021F3F6C(void *);
s32 ov14_021EC2A4(struct UnkStruct_ov14_boxapp *arg0);

s32 ov14_021F3380(UnkStruct_ov14_boxscan *arg0);

extern s32 ov14_021E9194;
extern s32 ov14_021E9450;
s32 ov14_021F33B0(UnkStruct_ov14_boxscan *arg0);

s32 ov14_021EC0EC(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxscan *v0;

    ov14_021E7278(arg0);
    if (ov14_021F3380(arg0->sys->unk_88DC) == 0) {
        v0 = arg0->sys->unk_88DC;
        if (v0->moveFlags != 0) {
            return 0x1E;
        }
        ov14_021F33E8(v0);
        ov14_021E7264(arg0);
        return 0x1B;
    }
    return 0x1A;
}

s32 ov14_021EC128(void *arg0) {
    ov14_021F67B0(arg0, 1, 0x25);
    ((UnkStruct_ov14_boxapp *)arg0)->unk30 = 0x1C;
    return 6;
}

s32 ov14_021EC13C(void *arg0) {
    ov14_021F67B0(arg0, 2, 0x25);
    ((UnkStruct_ov14_boxapp *)arg0)->unk30 = 0x1D;
    return 6;
}

void ov14_021EC150(void *arg0) {
    s32 *v0;

    ov14_021E6100(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, ((UnkStruct_ov14_boxapp *)arg0)->unk21);
    ov14_021F6654(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0x25);
    ov14_021E765C(arg0);
    if ((u32) ((UnkStruct_ov14_boxapp *)arg0)->unk21 < 0x1EU) {
        ov14_021F4958(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F);
        ov14_021F4A20(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F);
        if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 1) {
            ov14_021F685C(arg0, 0, 0, 0x27);
            *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0x51;
        } else {
            *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0xC;
            ov14_021E8248(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
            ov14_021E82A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
        }
        ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
        GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32) ((UnkStruct_ov14_boxapp *)arg0)->unk21);
        GridInputHandler_SetButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 1);
        ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1);
        ov14_021F3488(arg0, 1, 0);
        ((UnkStruct_ov14_boxapp *)arg0)->unk21 = 0xFFU;
        v0 = &ov14_021E9450;
    } else {
        ov14_021F08BC(arg0);
        *((s8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk22) = 1;
        *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0x21;
        ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
        ov14_021F3488(arg0, 2, 0);
        v0 = &ov14_021E9194;
    }
    ov14_021F0234(arg0, v0, *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30));
}

s32 ov14_021EC23C(void *arg0) {
    ov14_021F08F0(arg0);
    if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk23[1]) != 0) {
        ov14_021F57B8(arg0);
    }
    ov14_021E7588(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21);
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)(u8)(((UnkStruct_ov14_boxapp *)arg0)->unk21 - 0x1E));
    GridInputHandler_SetButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 1);
    ((UnkStruct_ov14_boxapp *)arg0)->unk21 = 0xFFU;
    if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 0) {
        ov14_021F685C(arg0, 0, 0, 0x27);
        return 0x5B;
    }
    return 0x24;
}

s32 ov14_021EC2A4(UnkStruct_ov14_boxapp *arg0) {
    if (ov14_021F33B0(arg0->sys->unk_88DC) == 0) {
        ov14_021F33FC(arg0->sys->unk_88DC);
        ov14_021E7264(arg0);
        ov14_021F3F6C(arg0);
        ov14_021F67B0(arg0, 4, 0x25);
        ov14_021E8824(arg0->sys);
        *((s32 *)&arg0->unk30) = 0x1F;
        return 6;
    }
    return 0x1E;
}
