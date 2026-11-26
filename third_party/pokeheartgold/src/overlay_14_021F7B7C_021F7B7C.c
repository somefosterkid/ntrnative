#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "system.h"
#include "touchscreen.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F7B7C(UnkStruct_ov14_boxapp *arg0);

extern TouchscreenHitbox ov14_021F86C8;

s32 ov14_021F7B7C(UnkStruct_ov14_boxapp *arg0) {
    if (ov14_021E8648(arg0->sys->unk_2F0) == 0) {
        return 0;
    }
    if (0x800 & gSystem.newKeys) {
        return 1;
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov14_021F86C8) == 1) {
        return 1;
    }
    return 0;
}
