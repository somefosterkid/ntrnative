#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FEA0C.h"
#include "overworld_anim_manager.h"
#include "unk_0205FD20.h"

extern const OverworldAnimManagerFuncs ov01_022090FC;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FEA0C_021FEB3C_021FEB8C_022090FC.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_022090FC = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F2974, ov01_021FEB3C, ov01_021FEB78, ov01_021FEB8C, ov01_021FEBC0);
#endif

OverworldAnimManager *ov01_021FEAB0(LocalMapObject *param0, int param1, int param2, int param3, int param4);

OverworldAnimManager *ov01_021FEAB0(LocalMapObject *param0, int param1, int param2, int param3, int param4) {
    int v0;
    OverworldAnimManager *v1;
    UnkStruct_ov5_021F28F4 v2;
    VecFx32 v3 = { 0, 0, 0 };
    FieldEffectManager *v4 = ov01_021F146C(param0);

    v2.unk_00 = param3;
    v2.unk_04 = v4;
    v2.fieldSystem = MapObject_GetFieldSystem(param0);
    v2.unk_0C = ov01_021F1450(v4, 5);
    v2.unk_10 = param0;

    if (param4 == 0) {
        v3.x = (((param1) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1);
        v3.z = (((param2) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1);
        sub_0206121C(v2.fieldSystem, &v3);
    } else {
        MapObject_CopyPositionVector(param0, &v3);
    }

    v0 = MapObject_GetPriorityPlusValue(param0, 2);
    v1 = ov01_021F1620(v4, &ov01_022090FC, &v3, param4, &v2, v0);

    return v1;
}
