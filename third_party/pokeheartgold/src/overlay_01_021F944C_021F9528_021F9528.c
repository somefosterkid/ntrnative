#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "error_handling.h"
#include "overlay_01_021F944C.h"
#include "field_types_def.h"

void ov01_021F93AC(const LocalMapObject *param0, VecFx32 *adjustedObjectPos);

BillboardResources *ov01_021F9704(UnkStruct_ov5_021ED0A4 *param0, u32 param1);

Billboard *ov01_021FA31C(BillboardList *list, const BillboardResources *resources, const VecFx32 *pos);

UnkStruct_ov5_021ED0A4 *ov01_021FA3DC(const LocalMapObject *param0);

Billboard *ov01_021F9528(LocalMapObject *param0, int param1);

Billboard *ov01_021F9528(LocalMapObject *param0, int param1) {
    UnkStruct_ov5_021ED0A4 *gfx;
    BillboardList *list;
    BillboardResources *resources;
    Billboard *billboard;
    VecFx32 pos;

    gfx = ov01_021FA3DC(param0);
    list = ov01_021FA1D0(gfx);
    ov01_021F9F84(gfx, param1);
    resources = ov01_021F9704(gfx, param1);
    if (resources == NULL) {
        GF_AssertFail();
    }
    ov01_021F93AC(param0, &pos);
    billboard = ov01_021FA31C(list, resources, &pos);
    if (billboard == NULL) {
        GF_AssertFail();
    }
    return billboard;
}
