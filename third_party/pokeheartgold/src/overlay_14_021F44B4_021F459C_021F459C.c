#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F459C(u8 *param_1);

void ov14_021F459C(u8 *param_1) {
    s16 sStack_e;
    s16 sStack_10;
    s16 asStack_c[2];
    unsigned short new_var;
    new_var = (*((u8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_44D)) % 4;
    ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxapp *)param_1)->sys->sprites[new_var + 0x15], &sStack_10, &sStack_e);
    ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxapp *)param_1)->sys->sprites[7], asStack_c, &sStack_e);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)param_1)->sys->sprites[7], sStack_10, sStack_e);
    ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxapp *)param_1)->sys->sprites[8], asStack_c, &sStack_e);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)param_1)->sys->sprites[8], sStack_10, sStack_e);
    ov14_021F4410(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys));
    return;
}
