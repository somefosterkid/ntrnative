#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2A18(struct UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
void ov14_021F4848(u8 *param_1);

void ov14_021F4848(u8 *param_1) {
    s32 iVar1;
    s32 iVar2;
    u32 remainder;
    s16 pos[2];
    iVar1 = ((UnkStruct_ov14_boxapp *)param_1)->unk25 / 6;
    iVar2 = ((UnkStruct_ov14_boxapp *)param_1)->unk1F / 6;
    if (iVar1 == iVar2) {
        ov14_021F2A18(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), 6, 1);
    } else {
        ov14_021F2A18(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), 6, 0);
    }
    remainder = ((UnkStruct_ov14_boxapp *)param_1)->unk1F % 6;
    ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxapp *)param_1)->sys->sprites[6], &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)param_1)->sys->sprites[6], (s16)((remainder * 0x22) + 0x2b), pos[0]);
}
