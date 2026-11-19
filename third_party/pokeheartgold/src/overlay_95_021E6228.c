#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite_system.h"
#include "pokepic.h"
#include "list_menu_items.h"
#include "egg_hatch_particle_system.h"
#include "overlay_95_egg_hatch_app.h"

void ov95_021E6228(EggHatchGraphics *eggHatch);

void ov95_021E6228(EggHatchGraphics *eggHatch) {
    SpriteSystem_FreeResourcesAndManager(eggHatch->spriteSys, eggHatch->spriteMan);
    SpriteSystem_Free(eggHatch->spriteSys);
}
