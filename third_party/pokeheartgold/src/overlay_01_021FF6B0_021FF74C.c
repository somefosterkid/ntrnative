#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF6B0.h"
#include "unk_0205FD20.h"

extern const OverworldAnimManagerFuncs ov01_022091EC;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FF6B0.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_022091EC = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F3FB8, ov01_021FF7B0, ov01_021FF7F4, ov01_021FF800, ov01_021FF830);
#endif

void ov01_021FF74C(LocalMapObject *param0) {
    UnkStruct_ov5_021F3F10 v0;
    FieldEffectManager *v1;
    OverworldAnimManager *v2;
    VecFx32 v3;
    int v4;

    v1 = ov01_021F146C(param0);

    v0.fieldSystem = MapObject_GetFieldSystem(param0);
    v0.unk_04 = v1;
    v0.unk_08 = ov01_021F1450(v1, 11);
    v0.unk_0C = param0;

    MapObject_CopyPositionVector(param0, &v3);
    sub_020611C8(MapObject_GetXCoord(param0), MapObject_GetZCoord(param0), &v3);

    v4 = MapObject_GetPriorityPlusValue(param0, 2);
    v2 = ov01_021F1620(v1, &ov01_022091EC, &v3, 0, &v0, v4);
}
