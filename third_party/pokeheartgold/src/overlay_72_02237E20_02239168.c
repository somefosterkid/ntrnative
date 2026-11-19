#include "global.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_02238680(void *, s32, s32);

s32 ov72_0223A350(void *, void *, s32, s32, s32);

s32 ov72_02239168(NintendoWfcConnect *arg0);

s32 ov72_02239168(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataWfc, 0xC, 1, 0xF0F);
    ov72_02238680(arg0, 0x30, 0x34);
    return 3;
}
