#include "global.h"
struct UnkStruct_ov96_0220B9A8;

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220_b9_a8.h"

void ov96_0220B940(struct UnkStruct_ov96_0220B9A8 *param_1, s32 param_2);

void ov96_0220B940(UnkStruct_ov96_0220B9A8 *param_1, s32 param_2) {
    BOOL BVar1;

    if ((param_2 == 0) && (BVar1 = ManagedSprite_IsAnimated(param_1->spr10), BVar1 == 0)) {
        ManagedSprite_SetAnim(param_1->spr10, 1);
    }
    return;
}
