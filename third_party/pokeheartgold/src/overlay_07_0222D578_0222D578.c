#include "global.h"

#include "sprite_system.h"

void ov07_0222D578(ManagedSprite **arg0);

void ov07_0222D578(ManagedSprite **arg0) {
    ManagedSprite_SetDrawFlag(*arg0, 1);
    ManagedSprite_SetAnimateFlag(*arg0, 1);
}
