#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF464.h"
#include "overworld_anim_manager.h"
#include "unk_020689C8.h"

extern const OverworldAnimManagerFuncs ov01_022091C0;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FF464.s (same label; the anims after it stay asm as ov01_022091D4), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_022091C0 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_021FF464Work, ov01_021FF54C, ov01_021FF5B8, ov01_021FF5C4, ov01_021FF658);
#endif

void ov01_021FF4FC(LocalMapObject *param0, int param1) {
    UnkOv01_021FF464Args v0;
    FieldEffectManager *v1;
    OverworldAnimManager *v2;

    v1 = ov01_021F146C(param0);

    v0.fieldSystem = MapObject_GetFieldSystem(param0);
    v0.fieldEffMan = v1;
    v0.renderer = ov01_021F1450(v1, 10);
    v0.mapObject = param0;

    {
        VecFx32 v3 = { 0, 0, 0 };
        int v4 = MapObject_GetPriorityPlusValue(param0, 2);

        v2 = ov01_021F1620(v1, &ov01_022091C0, &v3, param1, &v0, v4);
    }
}
