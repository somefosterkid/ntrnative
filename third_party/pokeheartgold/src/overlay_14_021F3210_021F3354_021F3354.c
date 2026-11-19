#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F3354(UnkStruct_ov14_boxscan *param_1);

void ov14_021F3354(UnkStruct_ov14_boxscan *param_1) {
    param_1->scale = 1.0f;
    param_1->frame = 0;
    ManagedSprite_SetAffineOverwriteMode(param_1->sprite, 1);
    ManagedSprite_SetAffineScale(param_1->sprite, param_1->scale, param_1->scale);
    ManagedSprite_SetAffineTranslation(param_1->sprite, 0, 8);
}
