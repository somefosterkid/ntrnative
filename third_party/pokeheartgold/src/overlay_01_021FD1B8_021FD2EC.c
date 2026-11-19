#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FD1B8.h"
#include "overlay_01_021FD2EC.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"

extern const OverworldAnimManagerFuncs ov01_02208E1C;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FD1B8.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_02208E1C = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_021F1258, ov01_021FD328, ov01_021FD378, ov01_021FD37C, ov01_021FD3E0);
#endif

OverworldAnimManager *ov01_021FD2EC(LocalMapObject *param0, const VecFx32 *param1) {
    int v0;
    OverworldAnimManager *v1;
    UnkStruct_021F121C v2;
    FieldEffectManager *v3 = ov01_021F146C(param0);

    v2.unk_00 = MapObject_GetSpriteID(param0);
    v2.unk_04 = v3;
    v2.unk_08 = param0;

    v0 = MapObject_GetPriorityPlusValue(param0, 2);
    v1 = ov01_021F1620(v3, &ov01_02208E1C, param1, 0, &v2, v0);

    return v1;
}

int ov01_021FD328(OverworldAnimManager *param0, UnkStruct_021F1258 *v0) {
    const UnkStruct_021F121C *v1 = sub_02068D98(param0);

    v0->unk_1C = *v1;
    v0->unk_04 = MapObject_GetID(v0->unk_1C.unk_08);
    v0->unk_18 = ov01_021FD2CC(v0->unk_1C.unk_04, v0->unk_1C.unk_00);

    if (MapObject_CheckFlag25(v0->unk_1C.unk_08) == 1) {
        v0->unk_08 = sub_0205F544(v0->unk_1C.unk_08);
    } else {
        v0->unk_08 = MapObject_GetMapID(v0->unk_1C.unk_08);
    }

    sub_02068DB8(param0, &v0->unk_0C);
    return 1;
}

void ov01_021FD378(void) {
}
