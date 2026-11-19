#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"

s32 ov01_021FA44C(s32 arg0);

s32 ov01_021F8C88(LocalMapObject *, Billboard *);

s32 sub_02023EE0(Billboard *, s32);

s32 sub_02023F04(Billboard *, s32);

s32 sub_02023F40(Billboard *, s32);

void ov01_021F8A4C(LocalMapObject *arg0);

void ov01_021F8A4C(LocalMapObject *arg0) {
    u32 temp_r0;
    MapObjectBillboardWorkA *work;
    Billboard *billboard;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(arg0);
    billboard = work->billboard;
    if ((ov01_021FA2D4(arg0) != 1) && (billboard != NULL)) {
        temp_r0 = MapObject_GetFacingDirection(arg0);
        if (temp_r0 != work->unk_00) {
            sub_02023EE0(billboard, ov01_021FA44C((s32)temp_r0));
            sub_02023F40(billboard, 0);
        }
        sub_02023F04(billboard, 0x1000);
        ov01_021FA3E8(arg0, billboard);
        ov01_021F8C88(arg0, billboard);
        work->unk_00 = (u8)temp_r0;
        work->unk_02 = (s8)sub_0205F330(arg0);
    }
}
