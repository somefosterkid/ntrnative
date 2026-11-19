#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

void ov01_021F9F3C(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    if (ov01_021F9DA4(gfx, id) == 0) {
        ov01_021F9974(gfx, id);
    }
}

void ov01_021F9F54(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    if (ov01_021F9EEC(gfx, id) == 2) {
        ov01_021F99D0(gfx, id);
    }
}

void ov01_021F9F6C(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    if (ov01_021F9F14(gfx, id) == 2) {
        ov01_021F9A44(gfx, id);
    }
}

void ov01_021F9F84(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    ov01_021F9F54(gfx, ov01_021FA28C(id));
    ov01_021F9F6C(gfx, ov01_021FA2A0(id));
    ov01_021F9F3C(gfx, id);
}

void ov01_021F9FB0(struct MapObjectManager *mgr, UnkStruct_ov5_021ED0A4 *gfx) {
    ov01_021F9BD4(mgr, gfx);
    ov01_021F9CF8(mgr, gfx);
    ov01_021F9E30(mgr, gfx);
}
