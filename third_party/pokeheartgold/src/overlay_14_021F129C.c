#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F459C(u8 *param_1);
s32 ov14_021F58B8(void *);

void ov14_021F129C(UnkStruct_ov14_boxapp *app, s32 arg1) {
    u8 pos[2];

    app->sys->unk_44D = (u8)(arg1 + (app->sys->unk_44D / 4) * 4);
    ov14_021F459C((u8 *)app);
    ov14_021F58B8(app);
    app->sys->unk_43C = GridInputHandler_GetNextInput(app->sys->unk_2C);
    GridInputHandler_SetNextLastUnk0FInputs(app->sys->unk_2C, 6, (u8)app->sys->unk_43C, app->sys->unk_43C & 0xFF);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(app->sys->unk_2C, 6), &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(app->sys->sprites[9], pos[1], pos[0]);
    ov14_021F29E4(app->sys, 9, 8);
    ov14_021F2A18(app->sys, 9, 1);
}
