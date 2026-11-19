#include "global.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"

s32 ov45_0222CC00(void *);

s32 ov45_0222CCA4(void *);

s32 ov45_0222CCB8(UnkStruct_ov66_02230A6C *param0);

void ov45_0222CB74(void *arg0, s8 arg1, s32 arg2);

void ov45_0222CB74(void *arg0, s8 arg1, s32 arg2) {
    ov45_0222CCB8(arg0);
    if (((s32)(*(u8 *)((u8 *)arg0 + 0x42) + 1) % 13) == *(u8 *)((u8 *)arg0 + 0x41)) {
        ov45_0222CC00(arg0);
    }
    *(s8 *)((u8 *)(arg0 + *(u8 *)((u8 *)arg0 + 0x42)) + 0x34) = arg1;
    *(s32 *)(arg0 + (*(u8 *)((u8 *)arg0 + 0x42) * 4)) = arg2;
    *(u8 *)((u8 *)arg0 + 0x42) = (u8)((s32)(*(u8 *)((u8 *)arg0 + 0x42) + 1) % 13);
    ov45_0222CCA4(arg0);
}
