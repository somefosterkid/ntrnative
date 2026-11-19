#include "global.h"

#include "billboard.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

extern const s32 ov01_02208A18[4];

void ov01_021F8804(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir);

void ov01_021F8804(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir) {
    if ((dir != work->unk_00) || (work->unk_02 != 0)) {
        sub_02023EE0(billboard, ov01_02208A18[dir]);
        sub_02023F40(billboard, 0);
    }
    sub_02023F04(billboard, 0x1000);
}

void ov01_021F883C(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir);

void ov01_021F883C(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir) {
    if ((dir != work->unk_00) || (work->unk_02 != 1)) {
        sub_02023EE0(billboard, ov01_02208A18[dir]);
        sub_02023F40(billboard, 0);
    }
    sub_02023F04(billboard, 0x1000);
}
