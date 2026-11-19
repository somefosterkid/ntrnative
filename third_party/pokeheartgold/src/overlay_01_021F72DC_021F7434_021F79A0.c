#include "global.h"

#include "billboard.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

int ov01_021F9344(const LocalMapObject *mapObj);

int ov01_021FA44C(int dir);

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

void ov01_021F79A0(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir);

void ov01_021F79A0(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir) {
    if (dir != work->unk_02) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x800);
    }
}

void ov01_021F79DC(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir);

void ov01_021F79DC(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir) {
    if (dir != work->unk_02) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x1000);
    }
}

void ov01_021F7A18(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir);

void ov01_021F7A18(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir) {
    if (dir != work->unk_02) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x2000);
    }
}

void ov01_021F7A54(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir);

void ov01_021F7A54(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkA *work, int dir) {
    if (dir != work->unk_02) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x4000);
    }
}
