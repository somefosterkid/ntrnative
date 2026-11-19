#include "global.h"

#include "bg_window.h"
#include "overlay_64_hall_of_fame_showcase.h"
#include "sprite_system.h"

void ov64_021E6754(HallOfFameShowcase *showcase);

void ov64_021E6754(HallOfFameShowcase *showcase) {
    ManagedSprite_SetDrawFlag(showcase->sprites[0], 0);
    ManagedSprite_SetDrawFlag(showcase->sprites[1], 0);
    BgFillTilemapBufferAndSchedule(showcase->bgConfig, 4U, 0U);
}
