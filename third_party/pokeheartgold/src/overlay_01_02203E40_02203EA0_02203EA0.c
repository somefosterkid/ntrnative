#include "global.h"

#include "camera.h"
#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_02203E40.h"
#include "overworld_anim_manager.h"

u32 _s32_div_f(u32, u32);

extern const OverworldAnimManagerFuncs ov01_022095B4;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_02203E40.s (same label), with the context size from the type, not the DS literal.
BOOL ov01_02203F3C(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);
void ov01_02203F68(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);
void ov01_02203F6C(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);
void ov01_02203F98(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);
const OverworldAnimManagerFuncs ov01_022095B4 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_02203E40_Work, ov01_02203F3C, ov01_02203F68, ov01_02203F6C, ov01_02203F98);
#endif

OverworldAnimManager *ov01_02203EA0(LocalMapObject *mapObj) {
    UnkOv01_02203E40_Data data;
    VecFx32 targetPos;
    VecFx32 objPos;
    VecFx32 cameraPos;
    FieldEffectManager *fieldEffMan;
    FieldSystem *fieldSystem;

    fieldEffMan = ov01_021F146C(mapObj);
    fieldSystem = ov01_021F1468(fieldEffMan);
    data.fieldEffMan = fieldEffMan;
    data.obj = ov01_021F1450(fieldEffMan, 0x13);

    if (!data.obj->anim) {
        data.mapObject = mapObj;
        MapObject_CopyPositionVector(mapObj, &objPos);
        cameraPos = Camera_GetLookAtCamPos(fieldSystem->camera);
        VEC_Subtract(&cameraPos, &objPos, &data.direction);
        data.dist = VEC_Distance(&cameraPos, &objPos);
        data.dist = data.dist / 3;
        VEC_Normalize(&data.direction, &data.direction);
        MapObject_CopyPositionVector(mapObj, &targetPos);
        data.obj->anim = ov01_021F1620(fieldEffMan, &ov01_022095B4, &targetPos, 0, &data, MapObject_GetPriorityPlusValue(mapObj, 2));
        return data.obj->anim;
    }
    return data.obj->anim;
}
