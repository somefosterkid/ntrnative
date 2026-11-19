#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "field_system.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F7A50(void *arg0, s32, s32, s32);
s32 ov14_021F0244(void *, s32);
s32 ov14_021F1C4C(void *, u32);

s32 ov14_021EF93C(FieldSystem *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_r4;

    var_r4 = ov14_021F7A50(arg0, arg1, arg2, arg3);
    if (GridInputHandler_IsButtonInputMode(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C)) == 0) {
        var_r4 = -2;
    }
    switch ((u32)var_r4) {
    case -3:
        PlaySE(0x5DCU);
        return ov14_021F0244(arg0, 0x89);
    case -4:
        PlaySE(0x5DCU);
        break;
    case -2:
    case 0x24:
        PlaySE(0x5EAU);
        return ov14_021F1C4C(arg0, 0xFFU);
    case -1:
        break;
    default:
        PlaySE(0x5EAU);
        return ov14_021F1C4C(arg0, (u32)var_r4);
    }
    return 0x88;
}
