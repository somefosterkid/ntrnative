#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E637C(void *);
s32 ov14_021F08F0(void *);

s32 ov14_021EB7B0(UnkStruct_ov14_boxapp *arg0) {
    PlaySE(0x5EAU);
    ov14_021E637C(arg0);
    ov14_021F08F0(arg0);
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, 0x24);
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0xC;
}
