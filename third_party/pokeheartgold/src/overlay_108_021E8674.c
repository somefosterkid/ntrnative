#include "global.h"

#include "sys_task.h"
#include "sprite.h"

void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void ov108_021E8674(ManagedSprite *managedSprite);

void ov108_021E8674(ManagedSprite *managedSprite) {
    Sprite_DeleteAndFreeResources(managedSprite);
}
