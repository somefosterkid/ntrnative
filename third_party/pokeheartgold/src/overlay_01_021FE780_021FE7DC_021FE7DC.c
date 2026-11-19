#include "global.h"

#include "constants/field/field_effect_renderer.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE780.h"
#include "overworld_anim_manager.h"
#include "unk_0205FD20.h"

extern const OverworldAnimManagerFuncs ov01_022090DC;
extern const VecFx32 ov01_022090D0;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FE780_021FE8C8_022090D0.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_022090DC = OVERWORLD_ANIM_MANAGER_FUNCS(SurfMountRenderer, ov01_021FE868, ov01_021FE8B0, ov01_021FE8C8, ov01_021FE970);
#endif

OverworldAnimManager *ov01_021FE7DC(LocalMapObject *playerMapObj, int tileX, int tileZ, int dir, BOOL syncPos) {
    SurfMountUserData userData;
    VecFx32 mapObjPos = { 0, 0, 0 };

    userData.dir = dir;
    userData.fieldEffMan = ov01_021F146C(playerMapObj);
    userData.resources = ov01_021F1450(userData.fieldEffMan, FIELD_EFFECT_RENDERER_4);
    userData.playerMapObj = playerMapObj;

    if (!syncPos) {
        FieldSystem *fieldSystem = MapObject_GetFieldSystem(playerMapObj);

        mapObjPos.x = (((tileX) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1);
        mapObjPos.z = (((tileZ) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1);
        sub_0206121C(fieldSystem, &mapObjPos);
    } else {
        VecFx32 spritePosOffset = ov01_022090D0;

        MapObject_CopyPositionVector(playerMapObj, &mapObjPos);
        sub_0205F9A0(playerMapObj, &spritePosOffset);
    }

    int taskPriority = MapObject_GetPriorityPlusValue(playerMapObj, 2);
    return ov01_021F1620(userData.fieldEffMan, &ov01_022090DC, &mapObjPos, syncPos, &userData, taskPriority);
}
