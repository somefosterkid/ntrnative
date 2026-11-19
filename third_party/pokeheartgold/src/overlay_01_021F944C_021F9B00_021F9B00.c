#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "error_handling.h"

void ov01_021F9B00(UnkStruct_ov5_021ED0A4 *gfx) {
    ov01_021F9AAC(ov01_021FA200(gfx), 0xFF, 8);
}

void ov01_021F9B10(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;

    v0 = ov01_021FA20C(gfx);
    if (ov01_021F9AB4(ov01_021FA200(gfx), id, 0xFF, v0) == 0) {
        GF_AssertFail();
    }
}

int ov01_021F9B38(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;

    v0 = ov01_021FA20C(gfx);
    return ov01_021F9AD0(ov01_021FA200(gfx), id, v0);
}

void ov01_021F9B54(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA200(gfx);
    v0 = ov01_021FA20C(gfx) * 4;
    if (ov01_021F9AB4((int *)(v1 + v0), id, 0xFF, ov01_021FA214(gfx)) == 0) {
        GF_AssertFail();
    }
}

int ov01_021F9B84(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA200(gfx);
    v0 = ov01_021FA20C(gfx) * 4;
    return ov01_021F9AD0((int *)(v1 + v0), id, ov01_021FA214(gfx));
}

void ov01_021F9BAC(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA200(gfx);
    v0 = ov01_021FA20C(gfx) * 4;
    ov01_021F9AE4((int *)(v1 + v0), id, 0xFF, ov01_021FA214(gfx));
}
