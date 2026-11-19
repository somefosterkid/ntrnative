#include "global.h"

#include "overlay_01_021F1348.h"
#include "map_object.h"
#include "overlay_01_021FEEEC.h"
#include "overworld_anim_manager.h"

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F1924(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_021F1970(FieldEffectManager *fieldEffMan, u32 param1);
extern const OverworldAnimManagerFuncs ov01_02209138;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FEEEC.s (same label), with the context size from the type, not the DS literal.
BOOL ov01_021FF174(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FF228(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FF234(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FF35C(OverworldAnimManager *manager, FieldEffectAnimWork *work);
const OverworldAnimManagerFuncs ov01_02209138 = OVERWORLD_ANIM_MANAGER_FUNCS(FieldEffectAnimWork, ov01_021FF174, ov01_021FF228, ov01_021FF234, ov01_021FF35C);
#endif

void ov01_021FEFF8(FieldEffectRendererContext *param0) {
    ov01_021F18FC(param0->fieldEffMan, 0);
    ov01_021F18FC(param0->fieldEffMan, 5);

    ov01_021F1924(param0->fieldEffMan, 0);
    ov01_021F1924(param0->fieldEffMan, 3);

    ov01_021F1924(param0->fieldEffMan, 4);
    ov01_021F1924(param0->fieldEffMan, 5);

    ov01_021F1970(param0->fieldEffMan, 0);
    ov01_021F1970(param0->fieldEffMan, 4);
    ov01_021F1970(param0->fieldEffMan, 5);
    ov01_021F1970(param0->fieldEffMan, 6);

    ov01_021F18C8(param0->fieldEffMan, 0);
    ov01_021F18C8(param0->fieldEffMan, 5);
    ov01_021F18C8(param0->fieldEffMan, 6);
    ov01_021F18C8(param0->fieldEffMan, 7);
}

void ov01_021FF070(LocalMapObject *arg0, s32 a3) {
    VecFx32 v0;
    FieldEffectAnimArgs args;
    FieldEffectManager *fieldEffMan;

    fieldEffMan = ov01_021F146C(arg0);
    args.pos.x = MapObject_GetXCoord(arg0);
    args.pos.y = MapObject_GetYCoord(arg0);
    args.pos.z = MapObject_GetZCoord(arg0);
    args.unk_1C = 0;
    args.unk_1E = 0;
    args.direction = -1;
    args.fieldEffMan = fieldEffMan;
    args.fieldSystem = ov01_021F1468(fieldEffMan);
    args.unk_14 = ov01_021F1450(fieldEffMan, 8);
    args.mapObject = arg0;
    MapObject_CopyPositionVector(arg0, &v0);
    ov01_021F1620(fieldEffMan, &ov01_02209138, &v0, a3, &args, (s32)MapObject_GetPriorityPlusValue(arg0, 2U));
}

void ov01_021FF0E4(LocalMapObject *object, s32 a1, s32 x, s32 y, s32 a4) {
    VecFx32 v0;
    FieldEffectAnimArgs args;
    s32 a3;
    FieldEffectManager *fieldEffMan;

    a3 = a1;
    fieldEffMan = ov01_021F146C(object);
    args.pos.x = x;
    args.pos.y = MapObject_GetYCoord(object);
    args.pos.z = y;
    args.unk_1C = (s16)(MapObject_GetXCoord(object) - x);
    args.unk_1E = (s16)(MapObject_GetZCoord(object) - y);
    if (a4 != 0) {
        args.direction = (s8)MapObject_GetFacingDirection(object);
    } else {
        args.direction = -1;
    }
    args.fieldEffMan = fieldEffMan;
    args.fieldSystem = ov01_021F1468(fieldEffMan);
    args.unk_14 = ov01_021F1450(fieldEffMan, 8);
    args.mapObject = object;
    MapObject_CopyPositionVector(object, &v0);
    ov01_021F1620(fieldEffMan, &ov01_02209138, &v0, a3, &args, (s32)MapObject_GetPriorityPlusValue(object, 2U));
}
