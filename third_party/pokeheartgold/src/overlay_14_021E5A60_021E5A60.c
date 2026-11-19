#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_14_box_funcs.h"

extern u8 ov14_021F7CE4[];

void ov14_021E5A60(void) {
    GfGfx_SetBanks((GraphicsBanks *)ov14_021F7CE4);
    return;
}
