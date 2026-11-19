#include "global.h"

#include "overlay_01_021F1348.h"
#include "map_object.h"
#include "overlay_01_021FF854.h"
#include "overworld_anim_manager.h"

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F1924(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_021F1970(FieldEffectManager *fieldEffMan, u32 param1);
extern const OverworldAnimManagerFuncs ov01_02209218;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FF854.s (same label), with the context size from the type, not the DS literal.
BOOL ov01_021FF9F4(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FFAB0(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FFABC(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FFBD4(OverworldAnimManager *manager, FieldEffectAnimWork *work);
const OverworldAnimManagerFuncs ov01_02209218 = OVERWORLD_ANIM_MANAGER_FUNCS(FieldEffectAnimWork, ov01_021FF9F4, ov01_021FFAB0, ov01_021FFABC, ov01_021FFBD4);
#endif

void ov01_021FF8C8(FieldEffectRendererContext *param0) {
    ov01_021F18FC(param0->fieldEffMan, 7);
    ov01_021F1924(param0->fieldEffMan, 7);
    ov01_021F1970(param0->fieldEffMan, 8);
    ov01_021F18C8(param0->fieldEffMan, 9);
}

void ov01_021FF8F0(LocalMapObject *arg0, s32 a3) {
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
    args.unk_14 = ov01_021F1450(fieldEffMan, 0xC);
    args.mapObject = arg0;
    MapObject_CopyPositionVector(arg0, &v0);
    ov01_021F1620(fieldEffMan, &ov01_02209218, &v0, a3, &args, (s32)MapObject_GetPriorityPlusValue(arg0, 2U));
}

void ov01_021FF964(LocalMapObject *object, s32 a1, s32 x, s32 y, s32 a4) {
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
    args.unk_14 = ov01_021F1450(fieldEffMan, 0xC);
    args.mapObject = object;
    MapObject_CopyPositionVector(object, &v0);
    ov01_021F1620(fieldEffMan, &ov01_02209218, &v0, a3, &args, (s32)MapObject_GetPriorityPlusValue(object, 2U));
}
