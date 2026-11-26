#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4940(UnkStruct_ov14_boxsys *param_1, s32 param_2, s16 *param_3, s16 *param_4) {
    ManagedSprite_GetPositionXY(param_1->sprites[param_2 + 15], param_3, param_4);
    return;
}
