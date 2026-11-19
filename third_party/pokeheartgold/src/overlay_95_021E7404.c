#include "global.h"

#include "overlay_95_egg_hatch_app.h"
#include "sprite_system.h"

void ov95_021E7404(EggHatchPrompt *prompt);

void ov95_021E7404(EggHatchPrompt *prompt) {
    Sprite_DeleteAndFreeResources(prompt->cursorSprite);
}
