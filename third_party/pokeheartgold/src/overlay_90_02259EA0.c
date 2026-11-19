#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "overlay_90_app_02259ea0.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"


void ov90_02259EA0(UnkStruct_ov114_0225DAC0 *param0);

void ov90_02259EAC(Window *arg0);

void ov90_02259EA0(UnkStruct_ov114_0225DAC0 *param0) {
    Sprite_SetDrawFlag(param0->unk_10, TRUE);
}

void ov90_02259EAC(Window *window) {
    Sprite *sprite;
    Sprite *v0;

    sprite = ((Ov90App_02259EA0 *)window)->unk_10;
    if (sprite != NULL) {
        Sprite_Delete(sprite);
        ((Ov90App_02259EA0 *)window)->unk_10 = NULL;
    }
    v0 = ((Ov90App_02259EA0 *)window)->unk_14;
    if (v0 != NULL) {
        Sprite_Delete(v0);
        ((Ov90App_02259EA0 *)window)->unk_14 = NULL;
    }
    if (WindowIsInUse(window) == 1) {
        RemoveWindow(window);
    }
}
