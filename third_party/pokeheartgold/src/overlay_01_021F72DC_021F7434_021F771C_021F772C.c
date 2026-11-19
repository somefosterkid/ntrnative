#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"

void ov01_021F8C88(LocalMapObject *, Billboard *);

extern const MapObjectBillboardStateFuncA ov01_02208AC0[9];

void ov01_021F772C(LocalMapObject *param_1);

void ov01_021F772C(LocalMapObject *param_1) {
    u32 uVar3;
    BOOL BVar2;
    MapObjectBillboardWorkA *work;
    u32 uVar4;
    VecFx32 *pVVar5;
    Billboard *billboard;
    work = (MapObjectBillboardWorkA *)sub_0205F40C(param_1);
    billboard = work->billboard;
    BVar2 = ov01_021FA2D4(param_1);
    if ((BVar2 != 1) && (billboard != 0L)) {
        uVar3 = MapObject_GetFacingDirection(param_1);
        uVar4 = sub_0205F330(param_1);
        if ((8 + 1) <= ((s32)uVar4)) {
            GF_AssertFail();
        }
        ov01_02208AC0[uVar4](param_1, billboard, work, uVar3);
        pVVar5 = MapObject_GetFacingVector(param_1);
        pVVar5->z = 0x800;
        work->unk_02 = (u8)uVar3;
        uVar3 = sub_0205F330(param_1);
        work->unk_03 = (u8)uVar3;
        ov01_021FA3E8(param_1, billboard);
        ov01_021F8C88(param_1, billboard);
    }
    return;
}
