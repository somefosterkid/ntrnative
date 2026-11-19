#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F33FC(UnkStruct_ov14_boxscan *arg0);
void ov14_021F33E8(UnkStruct_ov14_boxscan *arg0);


s32 ov14_021F33B0(UnkStruct_ov14_boxscan *arg0);

s32 ov14_021F33B0(UnkStruct_ov14_boxscan *arg0) {
    float new_var;
    f32 v0;
    f32 v1;
    f32 fconst0 = 0.025f;
    arg0->frame = (u8)((arg0->frame) - 2);
    v0 = (arg0->scale) + fconst0;
    arg0->scale = v0;
    arg0->scale = (f32)(v0 + (new_var = 0.025f));
    if ((arg0->frame) == 0) {
        return 0;
    }
    v1 = arg0->scale;
    ManagedSprite_SetAffineScale(arg0->sprite, v1, v1);
    return 1;
}

void ov14_021F33E8(UnkStruct_ov14_boxscan *arg0) {
    ManagedSprite_SetDrawFlag(arg0->sprite, 0);
    ov14_021F33FC(arg0);
}

void ov14_021F33FC(UnkStruct_ov14_boxscan *arg0) {
    ManagedSprite_SetAffineTranslation(arg0->sprite, 0, 0);
    ManagedSprite_SetAffineScale(arg0->sprite, 1.0f, 1.0f);
    ManagedSprite_SetAffineOverwriteMode(arg0->sprite, 0U);
}
