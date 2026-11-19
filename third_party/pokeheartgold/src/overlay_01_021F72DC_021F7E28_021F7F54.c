#include "global.h"

#include "billboard.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F8C64(Billboard *billboard, int animNum);

int ov01_021F9344(const LocalMapObject *mapObj);

int ov01_021FA44C(int dir);

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

void ov01_021F7F54(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir);

void ov01_021F7F54(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir) {
    if (dir != work->unk_00) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
        goto block_6;
    }
    if (work->unk_02 == 9) {
        work->unk_03_0 = 1;
        return;
    }
    if (work->unk_03_0 == 1) {
        ov01_021F8C64(billboard, ov01_021FA44C(dir));
        work->unk_03_0 = 0;
    }
block_6:
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x800);
    }
}

void ov01_021F7FC8(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir);

void ov01_021F7FC8(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir) {
    if (dir != work->unk_00) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
    } else if (work->unk_02 == 9) {
        ov01_021F8C64(billboard, ov01_021FA44C(dir));
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x1000);
    }
}

void ov01_021F801C(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir);

void ov01_021F801C(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir) {
    if (dir != work->unk_00) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
    } else if (work->unk_02 == 9) {
        ov01_021F8C64(billboard, ov01_021FA44C(dir));
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x2000);
    }
}

void ov01_021F8070(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir);

void ov01_021F8070(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir) {
    if (dir != work->unk_00) {
        sub_02023EE0(billboard, ov01_021FA44C(dir));
        sub_02023F40(billboard, 0);
    } else if (work->unk_02 == 9) {
        ov01_021F8C64(billboard, ov01_021FA44C(dir));
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x4000);
    }
}
