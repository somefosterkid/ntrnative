#include "global.h"

#include "field/map_prop_animation.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01.h"

s32 GetDoorSE(FieldSystem *, s32, s32);

void ov01_021E9BB8(FieldSystem *fieldSystem, u8 a1) {
    MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, (s32)a1, 0, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, (s32)a1), 1));
}

void ov01_021E9BDC(FieldSystem *fieldSystem, u8 a1) {
    MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, (s32)a1, 1, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, (s32)a1), 0));
}
