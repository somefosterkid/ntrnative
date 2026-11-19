#include "global.h"

#include "map_object.h"
#include "overlay_01_021FD41C.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "unk_020689C8.h"

extern const OverworldAnimManagerFuncs ov01_02208EDC;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FD41C.s (same label), with the context size from the type, not the DS literal.
int ov01_021FD6C8(OverworldAnimManager *param0, void *param1);
void ov01_021FD714(void);
void ov01_021FD92C(OverworldAnimManager *a0, UnkStruct_ov5_021F15B4 *arg1);
void ov01_021FD980(OverworldAnimManager *param0, void *param1);
const OverworldAnimManagerFuncs ov01_02208EDC = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F15B4, ov01_021FD6C8, ov01_021FD714, ov01_021FD92C, ov01_021FD980);
#endif

OverworldAnimManager *ov01_021FD8E8(LocalMapObject *param0, int param1) {
    int v0;
    UnkStruct_ov5_021F1570 v1;
    FieldEffectManager *v2;
    VecFx32 v3;

    v2 = ov01_021F146C(param0);

    v1.unk_00 = v2;
    v1.unk_04 = ov01_021F1450(v2, 0);
    v1.unk_08 = param0;

    MapObject_CopyPositionVector(param0, &v3);

    v0 = MapObject_GetPriorityPlusValue(param0, 2);
    return ov01_021F1620(v2, &ov01_02208EDC, &v3, param1, &v1, v0);
}
