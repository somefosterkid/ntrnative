#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"

void ov01_021F8C88(LocalMapObject *, Billboard *);

extern const MapObjectBillboardStateFuncE ov01_02208B34[10];

void ov01_021F7E6C(LocalMapObject *param_1);

void sub_02023EE0(Billboard *billboard, int animNum);
void sub_02023F40(Billboard *billboard, fx32 animFrameNum);
int ov01_021FA44C(int param0);
void ov01_021F8C30(Billboard *param0);
void ov01_021F8C64(Billboard *param0, int param1);
void ov01_021F7EE4(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F7E6C(LocalMapObject *param_1) {
    u32 new_var;
    MapObjectBillboardWorkE *work;
    BOOL BVar2;
    u32 uVar3;
    u32 uVar4;
    VecFx32 *pVVar5;
    Billboard *billboard;
    work = (MapObjectBillboardWorkE *)sub_0205F40C(param_1);
    billboard = work->billboard;
    BVar2 = ov01_021FA2D4(param_1);
    if ((1 != BVar2) && (billboard != 0L)) {
        new_var = MapObject_GetFacingDirection(param_1);
        uVar3 = new_var;
        uVar4 = sub_0205F330(param_1);
        if ((9 + 1) <= ((s32)uVar4)) {
            GF_AssertFail();
        }
        ov01_02208B34[uVar4](param_1, billboard, work, uVar3);
        pVVar5 = MapObject_GetFacingVector(param_1);
        pVVar5->z = 0x800;
        work->unk_00 = (u8)uVar3;
        uVar3 = sub_0205F330(param_1);
        work->unk_02 = (u8)uVar3;
        ov01_021FA3E8(param_1, billboard);
        ov01_021F8C88(param_1, billboard);
    }
    return;
}

void ov01_021F7EE4(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3) {
    if (param3 != param2->unk_00) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
        param2->unk_03_0 = 0;
    } else if (param2->unk_02 == (0x8 + 1)) {
        param2->unk_03_0 = 1;
    } else if (param2->unk_03_0 == 1) {
        ov01_021F8C64(param1, ov01_021FA44C(param3));
        param2->unk_03_0 = 0;
    } else {
        ov01_021F8C30(param1);
    }
}
