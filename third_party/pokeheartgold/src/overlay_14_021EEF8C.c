#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F1F38(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F40DC(void *);
#else
s32 ov14_021F40DC(void *);
#endif

s32 ov14_021EEF8C(UnkStruct_ov14_boxapp *arg0) {
    u16 *v1;
    UnkStruct_ov14_boxsys *v0;

    v0 = arg0->sys;
    v1 = v0->unk_C;
    if ((*v1 == arg0->unk21) || (v0->unk_88C8 == 0)) {
        ov14_021F1F38(arg0);
        if (arg0->sys->unk_88C8 != 0) {
            PlaySE(0x5EAU);
            ov14_021F34C8(arg0->sys, (u16) arg0->unk21, 0);
        }
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, 0x24);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        return 0x75;
    }
    PlaySE(0x5EAU);
    ov14_021F34C8(arg0->sys, *v1, 0);
    ov14_021F34C8(arg0->sys, (u16) arg0->unk21, 1);
    ov14_021F40DC(arg0);
    ov14_021F391C(arg0->sys, 1);
    ov14_021F29E4(arg0->sys, 0xB, 2);
    return 0x7F;
}
