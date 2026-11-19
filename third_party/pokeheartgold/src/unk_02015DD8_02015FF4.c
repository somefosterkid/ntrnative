#include "global.h"
#include "field/overlay_01_02204004.h"

void sub_02015FF4(MapPropAnimation *arg0, s32 arg1);

typedef struct {
    u32 unk_00[4];
    u32 unk_10;
    u32 unk_14;
} UnkStruct_ov115_022632C8;
u32 sub_02015FF8(const UnkStruct_ov115_022632C8 *param0);

void sub_02015FF4(MapPropAnimation *arg0, s32 arg1) {
    *((s32 *)&arg0->active) = arg1;
}

u32 sub_02015FF8(const UnkStruct_ov115_022632C8 *param0) {
    return param0->unk_14;
}
