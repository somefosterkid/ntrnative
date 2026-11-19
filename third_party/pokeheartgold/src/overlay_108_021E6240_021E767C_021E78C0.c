#include "global.h"

#include "sprite.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E78C0(SafariAreaCustomizer *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov108_021E78C0(SafariAreaCustomizer *param_1, s32 param_2, s32 param_3, s32 param_4) {
    if (param_4 == 0) {
        Sprite_SetDrawFlag(param_1->unk_354[param_2], 0);
        return;
    }
    Sprite_SetDrawFlag(param_1->unk_354[param_2], 1);
    Sprite_SetAnimCtrlSeq(param_1->unk_354[param_2], param_3);
    return;
}
