#include "global.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"

void ov91_0225E1E4(UnkStruct_ov115_02261C18 *arg0, s32 arg1);

u16 ov91_0225E1EC(const UnkStruct_ov115_02261C18 *param0, s32 param1, s32 param2);
u16 ov91_0225E1F4(const UnkStruct_ov115_02261C18 *param0, s32 param1, s32 param2);

void ov91_0225E1E4(UnkStruct_ov115_02261C18 *arg0, s32 arg1) {
    arg0->unk_9C.z = arg1;
}

u16 ov91_0225E1EC(const UnkStruct_ov115_02261C18 *param0, s32 param1, s32 param2) {
    return (20 * 0xffff) / 360;
}

u16 ov91_0225E1F4(const UnkStruct_ov115_02261C18 *param0, s32 param1, s32 param2) {
    return (90 * 0xffff) / 360;
}
