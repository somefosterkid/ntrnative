#include "global.h"

#include "map_object.h"
#include "overlay_01_021FD41C.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "unk_020689C8.h"

extern const OverworldAnimManagerFuncs ov01_02208EC8;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FD41C.s (same label), with the context size from the type, not the DS literal.
int ov01_021FD784(OverworldAnimManager *param0, void *param1);
void ov01_021FD714(void);
void ov01_021FD718(OverworldAnimManager *a0, UnkStruct_ov5_021F15B4 *arg1);
void ov01_021FD838(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);
const OverworldAnimManagerFuncs ov01_02208EC8 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F15B4, ov01_021FD784, ov01_021FD714, ov01_021FD718, ov01_021FD838);
#endif

void ov01_021FD684(LocalMapObject *param0) {
    int v0, v1;
    UnkStruct_ov5_021F1570 v2;
    FieldEffectManager *v3;
    VecFx32 v4;

    v3 = ov01_021F146C(param0);

    v2.unk_00 = v3;
    v2.unk_04 = ov01_021F1450(v3, 0);
    v2.unk_08 = param0;

    MapObject_CopyPositionVector(param0, &v4);

    v0 = 3;
    v1 = MapObject_GetPriorityPlusValue(param0, 2);

    ov01_021F1620(v3, &ov01_02208EC8, &v4, v0, &v2, v1);
}


int sub_02068D90(const OverworldAnimManager *manager);

int ov01_021FD6C8(OverworldAnimManager *param0, void *param1);

int ov01_021FD6C8(OverworldAnimManager *param0, void *param1) {
    UnkStruct_ov5_021F15B4 *v0;
    const UnkStruct_ov5_021F1570 *v1;
    const void *userData;

    v0 = param1;
    userData = sub_02068D98(param0);
    v1 = userData;

    v0->unk_14 = *v1;
    v0->unk_10 = sub_02068D90(param0);
    v0->unk_00 = MapObject_GetSpriteID(v0->unk_14.unk_08);
    v0->unk_04 = MapObject_GetID(v0->unk_14.unk_08);

    if (MapObject_CheckFlag25(v0->unk_14.unk_08) == 1) {
        v0->unk_08 = sub_0205F544(v0->unk_14.unk_08);
    } else {
        v0->unk_08 = MapObject_GetMapID(v0->unk_14.unk_08);
    }

    return 1;
}
