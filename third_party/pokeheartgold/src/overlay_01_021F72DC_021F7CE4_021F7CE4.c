#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"

void ov01_021F8C88(LocalMapObject *, Billboard *);

void sub_02023EE0(Billboard *billboard, int animNum);

s32 sub_02023EF4(Billboard *);

s32 sub_02023F04(Billboard *, s32);

s32 sub_02023F40(Billboard *, s32);

void ov01_021F7CE4(LocalMapObject *arg0);

void ov01_021F7CE4(LocalMapObject *arg0) {
    s32 sp4;
    s32 sp0;
    Billboard *billboard;
    u32 temp_r7;
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(arg0);
    billboard = work->billboard;
    if ((ov01_021FA2D4(arg0) != 1) && (billboard != NULL)) {
        temp_r7 = MapObject_GetFacingDirection(arg0);
        if (sub_0205F330(arg0) == 0) {
            if (temp_r7 > 1U) {
                sp4 = 1;
            } else {
                sp4 = 0;
            }
            if ((sp4 != sub_02023EF4(billboard)) || (work->unk_03 == 0)) {
                sub_02023EE0(billboard, sp4);
                sub_02023F40(billboard, 0);
                sub_02023F04(billboard, 0);
            }
            sub_02023F04(billboard, 0);
        } else {
            if (temp_r7 > 1U) {
                sp0 = 1;
            } else {
                sp0 = 0;
            }
            if (sp0 != sub_02023EF4(billboard)) {
                sub_02023EE0(billboard, sp0);
                sub_02023F40(billboard, 0);
            }
            sub_02023F04(billboard, 0x1000);
        }
        work->unk_02 = (s8)temp_r7;
        work->unk_03 = (s8)sub_0205F330(arg0);
        ov01_021FA3E8(arg0, billboard);
        ov01_021F8C88(arg0, billboard);
    }
}
