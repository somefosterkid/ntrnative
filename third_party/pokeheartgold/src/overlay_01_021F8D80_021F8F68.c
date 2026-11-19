#include "global.h"

#include "map_object.h"

void ov01_021F8F68(LocalMapObject *arg0, s8 arg1);

void ov01_021F8F68(LocalMapObject *object, s8 arg1) {
    *(s8 *)((u8 *)(sub_0205F40C(object)) + 0x14) = arg1;
}

void ov01_021F8F74(LocalMapObject *arg0, s32 arg1);

void ov01_021F8F74(LocalMapObject *object, s32 arg1) {
    u8 *v0;

    v0 = sub_0205F40C(object);
    *(s8 *)((u8 *)v0 + 0x14) = (s8)(*(s8 *)((u8 *)v0 + 0x14) + arg1);
}

s32 ov01_021F8F88(LocalMapObject *arg0);

s32 ov01_021F8F88(LocalMapObject *object) {
    if (((u32)(*(u8 *)((u8 *)(sub_0205F40C(object)) + 0x17) << 0x1D) >> 0x1F) == 0) {
        return 1;
    }
    return 0;
}

void ov01_021F8FA0(LocalMapObject *arg0, void *arg1);

void ov01_021F8FA0(LocalMapObject *object, void *arg1) {
    u8 *v0;

    v0 = sub_0205F40C(object);
    *(s32 *)arg1 = (s32)(*(s8 *)((u8 *)v0 + 0x12) << 0xC);
    *(s32 *)((u8 *)arg1 + 8) = (s32)(*(s8 *)((u8 *)v0 + 0x13) << 0xC);
    *(s32 *)((u8 *)arg1 + 4) = 0;
}
