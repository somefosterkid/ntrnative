#include "global.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EEF34(void *arg0) {
    u8 sp0[2];
    u8 *new_var;
    u16 var_r4;
    unsigned char new_var2;
    var_r4 = ((UnkStruct_ov14_boxapp *)arg0)->unk21;
    if (((u32)var_r4) >= 0x1EU) {
        var_r4 = (u16)(var_r4 - 0x1E);
    }
    new_var2 = (s32)var_r4;
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, new_var2);
    new_var = (u8 *)arg0;
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)var_r4), &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)new_var)->sys->sprites[9], (s16)(sp0[1]), (s16)(sp0[0]));
    ov14_021F6768(arg0, ((UnkStruct_ov14_boxapp *)new_var)->unk_1C, 0x25);
    *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0x77;
    return 6;
}
