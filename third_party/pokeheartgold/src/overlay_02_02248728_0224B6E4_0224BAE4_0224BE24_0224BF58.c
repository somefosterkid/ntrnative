#include "global.h"

#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"

s32 sub_02054C90(FieldSystem *, u32 *, const u8, MapProp **, s32 *);

void ov02_0224BF58(FieldSystem *fieldSystem, u8 a1);

void ov02_0224BF58(FieldSystem *fieldSystem, u8 a1) {
    BOOL found;
    MapProp *mapProp;
    int modelID;
    int models[] = { 0x21, 0x8A };

    found = sub_02054C90(fieldSystem, models, 2, &mapProp, &modelID);
    if (found) {
        NNSG3dResTex *texture;

        texture = ov01_021FB9E0(fieldSystem->areaDataManager);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, a1, modelID, MapProp_GetRenderSurface(mapProp), MapProp_GetResModel(mapProp), texture, 2, 1, FALSE);
    } else {
        GF_AssertFail();
    }
}
