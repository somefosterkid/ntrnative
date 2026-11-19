#include "global.h"

#include "field/map_prop_animation.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_02.h"

void ov02_0224BFC0(FieldSystem *fieldSystem, u8 a1) {
    MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, (s32)a1, 0);
}

void ov02_0224BFCC(FieldSystem *fieldSystem, u8 a1) {
    MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, (s32)a1, 1);
}
