#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022031C0.h"
#include "overworld_anim_manager.h"

extern const OverworldAnimManagerFuncs ov01_022094C8;

extern const OverworldAnimManagerFuncs ov01_022094DC;

extern const OverworldAnimManagerFuncs ov01_022094F0;

extern const OverworldAnimManagerFuncs ov01_02209504;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs tables the DS keeps in asm/overlay_01_022031C0.s (same labels), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_022094C8 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_022031C0_Work, ov01_02203820, ov01_022033E0, ov01_02203890, ov01_022037E8);
const OverworldAnimManagerFuncs ov01_022094DC = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_022031C0_Work, ov01_0220335C, ov01_022033E0, ov01_022033E4, ov01_022034B8);
const OverworldAnimManagerFuncs ov01_022094F0 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_022031C0_Work, ov01_022035DC, ov01_022033E0, ov01_02203654, ov01_022037E8);
const OverworldAnimManagerFuncs ov01_02209504 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_022031C0_Work, ov01_0220335C, ov01_022033E0, ov01_022034F8, ov01_022035A4);
#endif

OverworldAnimManager *ov01_0220329C(LocalMapObject *obj, int a1);

OverworldAnimManager *ov01_0220329C(LocalMapObject *obj, int a1) {
    VecFx32 sp2C;
    FieldEffectAnimArgs args;
    FieldEffectManager *fieldEffMan;
    u32 temp_r0;
    fieldEffMan = ov01_021F146C(obj);
    MapObject_CopyPositionVector(obj, &args.pos);
    args.pos.z += 0x6000;
    args.unk_1C = 0;
    args.unk_1E = 0;
    args.direction = -1;
    args.fieldEffMan = fieldEffMan;
    args.fieldSystem = ov01_021F1468(fieldEffMan);
    args.unk_14 = ov01_021F1450(fieldEffMan, 0x11);
    args.mapObject = obj;
    MapObject_CopyPositionVector(obj, &sp2C);
    temp_r0 = MapObject_GetPriorityPlusValue(obj, 2U);
    if (a1 == 0) {
        return ov01_021F1620(fieldEffMan, &ov01_022094DC, &sp2C, 1, &args, (s32)temp_r0);
    } else if (a1 == 1) {
        return ov01_021F1620(fieldEffMan, &ov01_022094F0, &sp2C, 1, &args, (s32)temp_r0);
    } else if (a1 == 2) {
        return ov01_021F1620(fieldEffMan, &ov01_02209504, &sp2C, 1, &args, (s32)temp_r0);
    } else {
        return ov01_021F1620(fieldEffMan, &ov01_022094C8, &sp2C, 1, &args, (s32)temp_r0);
    }
}
