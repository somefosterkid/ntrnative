#include "global.h"
#include "sprite_system.h"
#include "struct_defs/resdat_narc_structs.h"

struct SpriteManager;

struct ManagedSpriteTemplate;

extern const ManagedSpriteTemplate ov80_0223DA54[4];

void ov80_02239B7C(SpriteManager *param0, u16 param1);

void ov80_02239B7C(SpriteManager *param0, u16 param1) {
    const ManagedSpriteTemplate *v0;

    GF_ASSERT(param1 < NELEMS(ov80_0223DA54));
    v0 = &ov80_0223DA54[param1];

    SpriteManager_UnloadCharObjById(param0, v0->resIdList[0]);
    SpriteManager_UnloadPlttObjById(param0, v0->resIdList[1]);
    SpriteManager_UnloadCellObjById(param0, v0->resIdList[2]);
    SpriteManager_UnloadAnimObjById(param0, v0->resIdList[3]);
}
