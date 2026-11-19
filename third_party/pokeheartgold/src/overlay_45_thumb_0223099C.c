#include "global.h"

#include "error_handling.h"

typedef struct {
    u16 unk_00;
    u16 unk_02_0 : 15;
    u16 unk_02_15 : 1;
} UnkStruct_ov66_02258B38;

extern u32 ov45_02254C48;

UnkStruct_ov66_02258B38 *ov45_0223099C(u32 param0);

s32 ov45_022309C4(s32 arg0, s32 arg1);
s32 ov45_022309D0(u32 arg0, s32 arg1, s32 arg2);

UnkStruct_ov66_02258B38 *ov45_0223099C(u32 param0) {
    s32 iVar1;
    u16 *puVar2;

    puVar2 = (u16 *)&ov45_02254C48;
    iVar1 = 0;
    do {
        if (param0 == *puVar2) {
            return (UnkStruct_ov66_02258B38 *)(&ov45_02254C48 + iVar1);
        }
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 2;
    } while (iVar1 < 0x14);
    GF_AssertFail();
    return NULL;
}

s32 ov45_022309C4(s32 arg0, s32 arg1) {
    s32 v0;

    v0 = arg1;
    if (arg0 == 0) {
        v0 += 4;
    }
    return v0;
}

s32 ov45_022309D0(u32 arg0, s32 arg1, s32 arg2) {
    return ((u32)((u16)(arg1 + 1) * arg2) / arg0) << 0xC;
}
