#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "billboard.h"

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

int ov01_021F9344(const LocalMapObject *param0);

int ov01_021FA44C(int param0);

void ov01_021F8C64(Billboard *param0, int param1);

void ov01_021F80C4(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

typedef struct {
    fx32 a[7];
} ov01_02208A48_Array;

extern const ov01_02208A48_Array ov01_02208A48;

void ov01_021F80C4(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3) {
    if (param3 != param2->unk_00) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
        param2->unk_01 = 0;
    } else if (param2->unk_02 == (0x8 + 1)) {
        ov01_021F8C64(param1, ov01_021FA44C(param3));
    }

    if (param2->unk_02 != 0x6) {
        param2->unk_01 = 0;
    }

    if (ov01_021F9344(mapObj) == 0) {
        ov01_02208A48_Array v1 = ov01_02208A48;

        sub_02023F04(param1, v1.a[param2->unk_01]);
        param2->unk_01++;

        if (v1.a[param2->unk_01] == 0) {
            param2->unk_01 = 0;
        }
    }
}

void ov01_021F8164(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

typedef struct {
    fx32 a[4];
} ov01_022089D8_Array;

extern const ov01_022089D8_Array ov01_022089D8;

void ov01_021F8164(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3) {
    if (param3 != param2->unk_00) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
        param2->unk_01 = 0;
    } else if (param2->unk_02 == (0x8 + 1)) {
        ov01_021F8C64(param1, ov01_021FA44C(param3));
    }

    if (param2->unk_02 != 0x7) {
        param2->unk_01 = 0;
    }

    if (ov01_021F9344(mapObj) == 0) {
        ov01_022089D8_Array v1 = ov01_022089D8;

        sub_02023F04(param1, v1.a[param2->unk_01]);
        param2->unk_01++;

        if (v1.a[param2->unk_01] == 0) {
            param2->unk_01 = 0;
        }
    }
}

void ov01_021F81FC(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

typedef struct {
    fx32 a[8];
} ov01_02208AA0_Array;

extern const ov01_02208AA0_Array ov01_02208AA0;

void ov01_021F81FC(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3) {
    if (param3 != param2->unk_00) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
        param2->unk_01 = 0;
    } else if (param2->unk_02 == (0x8 + 1)) {
        ov01_021F8C64(param1, ov01_021FA44C(param3));
    }

    if (param2->unk_02 != 0x8) {
        param2->unk_01 = 0;
    }

    if (ov01_021F9344(mapObj) == 0) {
        ov01_02208AA0_Array v1 = ov01_02208AA0;

        sub_02023F04(param1, v1.a[param2->unk_01]);
        param2->unk_01++;

        if (v1.a[param2->unk_01] == 0) {
            param2->unk_01 = 0;
        }
    }
}
