#include "global.h"

#include "sys_task.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EBDCC(void *arg0) {
    ov14_021F6768(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk_1C, 0x25);
    *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0xE;
    return 6;
}

void ov14_021EBDE0(UnkStruct_ov14_boxapp *arg0) {
    ov14_021EBDE8(arg0);
}
