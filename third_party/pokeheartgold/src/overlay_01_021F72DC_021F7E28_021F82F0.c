#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"

void ov01_021F8400(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F847C(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F8374(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F82F0(LocalMapObject *mapObj);

extern const MapObjectBillboardStateFuncE ov01_02208AE4[12];

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021F7F54(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F7FC8(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F801C(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F80C4(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F7C80(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F79A0(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

// The asm cuts this table at ov01_02208B0C (the last two entries are that table's first two).
const MapObjectBillboardStateFuncE ov01_02208AE4[12] = {
    ov01_021F8374,
    ov01_021F7F54,
    ov01_021F7F54,
    ov01_021F7FC8,
    ov01_021F801C,
    ov01_021F80C4,
    ov01_021F80C4,
    ov01_021F80C4,
    ov01_021F8400,
    ov01_021F847C,
    ov01_021F7C80,
    ov01_021F79A0,
};
#endif

void ov01_021F8C88(LocalMapObject *mapObj, Billboard *billboard);

void ov01_021F82F0(LocalMapObject *mapObj) {
    int facing, v1;
    MapObjectBillboardWorkE *v2 = (MapObjectBillboardWorkE *)sub_0205F40C(mapObj);
    Billboard *v3 = v2->billboard;

    if (ov01_021FA2D4(mapObj) == 1) {
        return;
    }

    if (v3 == NULL) {
        return;
    }

    facing = MapObject_GetFacingDirection(mapObj);
    v1 = sub_0205F330(mapObj);

    GF_ASSERT(v1 < 12);

    if ((v2->unk_02 == 0x0) && (v1 != v2->unk_02) && (v2->unk_03_0 == 0)) {
        v2->unk_00 = -1;
    }

    ov01_02208AE4[v1](mapObj, v3, v2, facing);

    v2->unk_00 = facing;
    v2->unk_02 = v1;

    ov01_021FA3E8(mapObj, v3);
    ov01_021F8C88(mapObj, v3);
}

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

typedef struct {
    int v[4];
} ov01_02208A38_t;

extern const ov01_02208A38_t ov01_02208A38;

void ov01_021F8374(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3) {
    ov01_02208A38_t v0 = ov01_02208A38;
    int v1 = v0.v[param3];

    if (param3 != param2->unk_00) {
        sub_02023EE0(param1, v1);
        sub_02023F40(param1, 0);
        param2->unk_03_0 = 0;
    } else if (param2->unk_02 != 0x0) {
        param2->unk_03_0 = 1;
    } else if (param2->unk_03_0 == 1) {
        sub_02023EE0(param1, v1);
        sub_02023F40(param1, 0);
        param2->unk_03_0 = 0;
    } else {
        sub_02023F04(param1, FX32_ONE);
    }
}

int sub_02023EF4(const Billboard *billboard);

int ov01_021F9344(const LocalMapObject *param0);

int ov01_021FA464(int dir);

void ov01_021F8400(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3) {
    int v0 = ov01_021FA464(param3);

    if ((param3 != param2->unk_00) || (param2->unk_02 != 0x0)) {
        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    } else if (param2->unk_02 != 10) {
        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    } else if (sub_02023EF4(param1) != v0) {
        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    }

    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(param1, FX32_ONE / 2);
    }
}

void ov01_021F847C(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3) {
    int v0 = ov01_021FA464(param3);

    if (param3 != param2->unk_00) {
        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    } else if (param2->unk_02 != 11) {
        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    } else if (sub_02023EF4(param1) != v0) {
        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    }

    if (ov01_021F9344(mapObj) == 0) {
        sub_02023F04(param1, FX32_ONE);
    }
}

void ov01_021F84F4(LocalMapObject *mapObj);

extern const MapObjectBillboardStateFuncE ov01_022089E8[4];

void ov01_021F8708(LocalMapObject *mapObj, Billboard *billboard, int facing);

void ov01_021F84F4(LocalMapObject *mapObj) {
    int facing, v1;
    MapObjectBillboardWorkE *v2 = (MapObjectBillboardWorkE *)sub_0205F40C(mapObj);
    Billboard *v3 = v2->billboard;

    if ((v3 == NULL) || (ov01_021FA2D4(mapObj) == 1)) {
        return;
    }

    facing = MapObject_GetFacingDirection(mapObj);
    v1 = sub_0205F330(mapObj);

    GF_ASSERT(v1 < 4);

    ov01_022089E8[v1](mapObj, v3, v2, facing);
    ov01_021F8708(mapObj, v3, facing);

    v2->unk_00 = facing;
    v2->unk_02 = sub_0205F330(mapObj);

    ov01_021FA3E8(mapObj, v3);
    ov01_021F8C88(mapObj, v3);
}
