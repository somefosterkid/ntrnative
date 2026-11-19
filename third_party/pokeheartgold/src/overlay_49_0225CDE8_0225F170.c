#include "global.h"
#include "overlay_49_unk_struct_ov70_02263344_t.h"
struct UnkStruct_ov70_02263344_t;

s32 ov49_0225F170(struct UnkStruct_ov70_02263344_t *arg0);

s32 ov49_0225F170(UnkStruct_ov70_02263344_t *arg0) {
    if (*((s32 *)&arg0->unk_14.unk_00) == 0) {
        return 1;
    }
    return 0;
}

s32 ov49_0225F180(void *arg0);

s32 ov49_0225F180(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 4) != 0) {
        return 1;
    }
    return 0;
}

void ov49_0225F190(void *arg0, void *arg1, void *arg2, s32 arg3, void *arg4);

void ov49_0225F190(void *arg0, void *arg1, void *arg2, s32 arg3, void *arg4) {
    *(void **)arg0 = arg1;
    *(s32 *)((u8 *)arg0 + 0xC) = arg3;
    *(void **)((u8 *)arg0 + 4) = arg4;
    *(void **)((u8 *)arg0 + 8) = arg2;
}

void ov49_0225F19C(void *arg0);

void ov49_0225F19C(void *arg0) {
    *(s32 *)arg0 = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
    *(s32 *)((u8 *)arg0 + 4) = 0;
    *(s32 *)((u8 *)arg0 + 8) = 0;
}
