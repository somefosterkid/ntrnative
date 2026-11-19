#include "global.h"

#include "map_object.h"
#include "overlay_01_021FD41C.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"

extern const OverworldAnimManagerFuncs ov01_02208EA0;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FD41C.s (same label), with the context size from the type, not the DS literal.
int ov01_021FD6C8(OverworldAnimManager *param0, void *param1);
void ov01_021FD714(void);
void ov01_021FD718(OverworldAnimManager *a0, UnkStruct_ov5_021F15B4 *arg1);
void ov01_021FD7D4(OverworldAnimManager *param0, void *param1);
const OverworldAnimManagerFuncs ov01_02208EA0 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F15B4, ov01_021FD6C8, ov01_021FD714, ov01_021FD718, ov01_021FD7D4);
#endif

void ov01_021FD640(LocalMapObject *param0) {
    int v0, v1;
    UnkStruct_ov5_021F1570 v2;
    FieldEffectManager *v3;
    VecFx32 v4;

    v3 = ov01_021F146C(param0);

    v2.unk_00 = v3;
    v2.unk_04 = ov01_021F1450(v3, 0);
    v2.unk_08 = param0;

    MapObject_CopyPositionVector(param0, &v4);

    v0 = 0;
    v1 = MapObject_GetPriorityPlusValue(param0, 2);

    ov01_021F1620(v3, &ov01_02208EA0, &v4, v0, &v2, v1);
}
