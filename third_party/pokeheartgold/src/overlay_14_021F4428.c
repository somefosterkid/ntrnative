#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "unk_02013534.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F462C(void *);
void ov14_021F4428(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F4428(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *v0;

    v0 = arg0->sys;
    ManagedSprite_SetPositionXY(v0->sprites[4], 0xC, -0x15);
    ManagedSprite_SetPositionXY(v0->sprites[5], 0xF4, -0x15);
    ManagedSprite_SetPositionXY(v0->sprites[6], 0x2B, -0x15);
    ManagedSprite_SetPositionXY(v0->sprites[7], 0x80, -0x28);
    ManagedSprite_SetPositionXY(v0->sprites[8], 0x80, -0x1C);
    ov14_021F462C(arg0);
    ov14_021F29E4(v0, 7, 7);
    ov14_021F4410(v0);
    TextOBJ_SetSpritesDrawFlag(v0->textObjs[0].textObj, 1);
    TextOBJ_SetSpritesDrawFlag(v0->textObjs[1].textObj, 0);
}
