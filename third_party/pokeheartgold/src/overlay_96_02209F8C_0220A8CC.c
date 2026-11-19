#include "global.h"

#include "error_handling.h"
#include "unk_struct_ov96_0220b148.h"
#include "overlay_96_app_0220a87c.h"

s32 ov96_0220B0A4(UnkStruct_ov96_0220B148 *, s32);

s32 ov96_0220A8CC(Ov96App_0220A87C *arg0, s32 arg1);

s32 ov96_0220A8CC(Ov96App_0220A87C *arg0, s32 arg1) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (((u32)(arg0->unk_180 << 0x18) >> 0x1C) == 0) {
        GF_AssertFail();
    }
    if (ov96_0220B0A4(arg0->unk_154[((u32)(arg0->unk_180 << 0x18) >> 0x1C) - 1], arg1) != 0) {
        return 1;
    }
    return 0;
}
