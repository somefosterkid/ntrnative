#include "global.h"
#include "gf_3d_loader.h"
#include "map_object.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"







s32 ov01_021F9FCC(MapObjectManager *, s32, LocalMapObject *);

s32 ov01_021FA01C(MapObjectManager *, s32, LocalMapObject *);

s32 ov01_021FA094(MapObjectManager *, s32, LocalMapObject *);



s32 ov01_021FA4F0(void *, s32);

s32 ov01_021FA6A4(void *, s32);

void ov01_021FC588(struct ResourceHeap *, u32);

void ov01_021FA108(MapObjectManager *mgr, s32 gfxId, LocalMapObject *mapObject);


void ov01_021FA108(MapObjectManager *mgr, s32 gfxId, LocalMapObject *mapObject) {
    s32 v1;
    s32 v0;
    UnkStruct_ov5_021ED0A4 *v2;

    v2 = sub_0205F1A0(mgr);
    v0 = ov01_021FA28C(gfxId);
    if ((ov01_021F9EEC(v2, v0) == 1) && (ov01_021FA01C(mgr, v0, mapObject) == 0)) {
        ov01_021FC588(v2->unk_F8, v0);
        ov01_021F9BAC(v2, v0);
    }
    v1 = ov01_021FA2A0(gfxId);
    if ((ov01_021F9F14(v2, v1) == 1) && (ov01_021FA094(mgr, v1, mapObject) == 0)) {
        ov01_021FC588(v2->unk_FC, v1);
        ov01_021F9CD0(v2, v1);
    }
    if ((ov01_021F9EC4(v2, gfxId) == 1) && (ov01_021F9FCC(mgr, gfxId, mapObject) == 0)) {
        GF3dGfxRawResMan_FreeObjById(ov01_021FA1F4(v2), gfxId);
        ov01_021F9E04(v2, gfxId);
        ov01_021FA4F0(v2, gfxId);
        ov01_021FA6A4(v2, gfxId);
    }
}


void ov01_021FA1C8(UnkStruct_ov5_021ED0A4 *gfx, struct BillboardList *list) {
    gfx->unk_E0 = list;
}

struct BillboardList *ov01_021FA1D0(UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_E0;
}

void ov01_021FA1D8(UnkStruct_ov5_021ED0A4 *gfx, int value) {
    gfx->unk_04 = value;
}
