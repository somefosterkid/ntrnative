#include "global.h"

#include "field/field_control.h"
#include "field/terrain_collision_manager.h"

#include "overlay_01_021F4704.h"

#include "field_types_def.h"
#include "error_handling.h"

s32 ov01_021F654C(MapLoadManager *, s32, s32, u8 *);

SoundplateStruct *sub_02054874(FieldSystem *fieldSystem, s32 x, s32 z);

extern const TerrainCollisionManager _020FC604;
extern const TerrainCollisionManager _020FC614;
void sub_0205489C(const TerrainCollisionManager **a0, s32 a1);

SoundplateStruct *sub_02054874(FieldSystem *fieldSystem, s32 x, s32 z) {
    u8 v0;
    MapLoadManager *v1;

    v1 = fieldSystem->mapLoadManager;
    if (ov01_021F654C(v1, x, z, &v0) == 0) {
        return NULL;
    }
    return ov01_021F6600(v1, v0);
}

void sub_0205489C(const TerrainCollisionManager **a0, s32 a1) {
    if (a1 == 0) {
        *a0 = &_020FC604;
        return;
    }
    if (a1 == 1) {
        *a0 = &_020FC614;
        return;
    }
    GF_AssertFail();
}
