#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F6B28(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

void ov14_021F6B28(UnkStruct_ov14_boxapp *arg0, s32 target) {
    u8 py[2];
    DpadMenuBox *dpadBoxes;

    dpadBoxes = (struct DpadMenuBox *)(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, target));
    ov14_021F2A18(*((void * *)&arg0->sys), 9, 1);
    DpadMenuBox_GetPosition(dpadBoxes, &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16) py[1], (s16) py[0]);
}
