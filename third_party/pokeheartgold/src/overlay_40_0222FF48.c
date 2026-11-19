#include "global.h"
struct UnkStruct_0208C06C;

#include "sprite_system.h"
#include "overlay_40.h"
#include "sprite.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222FF48(struct UnkStruct_0208C06C *arg0, s32 arg1, ManagedSprite *arg2);

void ov40_0222FF48(UnkStruct_0208C06C *arg0, s32 arg1, ManagedSprite *managedSprite) {
    if (managedSprite != NULL) {
        SpriteManager_UnloadCharObjById(arg0->unk_14.unk_08, arg1 + 0x186A0);
        Sprite_DeleteAndFreeResources(managedSprite);
    }
}

void ov40_0222FF64(u32 param_1, u32 param_2, ManagedSprite *param_3) {
    if (param_3 != NULL) {
        Sprite_DeleteAndFreeResources(param_3);
    }
    return;
}
