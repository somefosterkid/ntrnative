#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2BE8(UnkStruct_ov14_boxsys *sys, int slot) {
    if (sys->sprites[slot] != NULL) {
        Sprite_DeleteAndFreeResources(sys->sprites[slot]);
        sys->sprites[slot] = NULL;
    }
}
