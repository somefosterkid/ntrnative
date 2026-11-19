#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"


s32 ov14_021F3380(UnkStruct_ov14_boxscan *arg0);

s32 ov14_021F3380(UnkStruct_ov14_boxscan *arg0) {
    f32 v0;

    arg0->frame = (u8)(arg0->frame + 1);
    arg0->scale = (f32)(arg0->scale - 0.025f);
    if (arg0->frame == 0x28) {
        return 0;
    }
    v0 = arg0->scale;
    ManagedSprite_SetAffineScale(arg0->sprite, v0, v0);
    return 1;
}
