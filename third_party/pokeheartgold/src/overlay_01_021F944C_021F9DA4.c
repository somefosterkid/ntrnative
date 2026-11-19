#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "error_handling.h"

s32 ov01_021FA524(void *, s32);

int ov01_021F9DA4(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    if (ov01_021F9D88(gfx, id) == 1) {
        return 1;
    }
    if (ov01_021F9E9C(gfx, id) == 1) {
        return 2;
    }
    return ov01_021FA524(gfx, id);
}

void ov01_021F9DD0(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA1FC(gfx);
    v0 = ov01_021FA22C(gfx) * 4;
    if (ov01_021F9AB4((int *)(v1 + v0), id, 0xFFFF, ov01_021FA234(gfx)) == 0) {
        GF_AssertFail();
    }
}

void ov01_021F9E04(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA1FC(gfx);
    v0 = ov01_021FA22C(gfx) * 4;
    ov01_021F9AE4((int *)(v1 + v0), id, 0xFFFF, ov01_021FA234(gfx));
}
