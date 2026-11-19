#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

int ov01_021F9E9C(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA1FC(gfx);
    v0 = ov01_021FA22C(gfx) * 4;
    return ov01_021F9AD0((int *)(v1 + v0), id, ov01_021FA234(gfx));
}
