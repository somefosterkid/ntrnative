#include "global.h"

#include "overlay_93_thumb_2.h"
#include "sprite_system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

extern ManagedSpriteTemplate ov93_02262FA0;

ManagedSprite *ov93_02262344(UnkStruct_ov117_02261280_t *arg0) {
    ManagedSprite *managedSprite;

    managedSprite = SpriteSystem_NewSprite(arg0->unk_24, arg0->unk_28, &ov93_02262FA0);
    ManagedSprite_SetDrawFlag(managedSprite, 0);
    Sprite_TickFrame(managedSprite->sprite);
    return managedSprite;
}
