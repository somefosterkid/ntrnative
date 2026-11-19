#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"

void ov01_021F8C88(LocalMapObject *, Billboard *);

u32 ov01_021FA44C(u32);

void sub_02023EE0(Billboard *, u32);

void sub_02023F04(Billboard *, u32);

void sub_02023F40(Billboard *, u32);

void ov01_021F88F0(LocalMapObject *param_1);

void ov01_021F88F0(LocalMapObject *param_1) {
    MapObjectBillboardWorkA *work;
    BOOL BVar2;
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;
    Billboard *billboard;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(param_1);
    billboard = work->billboard;
    BVar2 = ov01_021FA2D4(param_1);
    if ((BVar2 != 1) && (billboard != NULL)) {
        uVar3 = MapObject_GetFacingDirection(param_1);
        work->unk_00 = (u8)uVar3;
        uVar4 = sub_0205F330(param_1);
        work->unk_02 = (u8)uVar4;
        uVar5 = ov01_021FA44C(uVar3);
        sub_02023EE0(billboard, uVar5);
        sub_02023F40(billboard, 0);
        sub_02023F04(billboard, 0);
        ov01_021FA3E8(param_1, billboard);
        ov01_021F8C88(param_1, billboard);
    }
    return;
}
