#include "global.h"

#include "billboard.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F8C3C(Billboard *billboard, int animNum, int frameDiv);

int ov01_021F9344(const LocalMapObject *mapObj);

int ov01_021FA458(int dir);

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

void ov01_021F829C(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir);

void ov01_021F829C(LocalMapObject *mapObj, Billboard *billboard, MapObjectBillboardWorkE *work, int dir) {
    if (dir != work->unk_00) {
        sub_02023EE0(billboard, ov01_021FA458(dir));
        sub_02023F40(billboard, 0);
    } else if (work->unk_02 != 9) {
        ov01_021F8C3C(billboard, ov01_021FA458(dir), 4);
    }
    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(billboard, 0x1000);
    }
}
