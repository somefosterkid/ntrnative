#include "global.h"

#include "system.h"
#include "touchscreen.h"
#include "overlay_112_sprite_app.h"

extern const u8 ov112_021FF2DC[];

BOOL ov112_021F0EFC(Ov112SpriteApp *arg0);

BOOL ov112_021F0EFC(Ov112SpriteApp *arg0) {
    if (arg0->curIdx < arg0->count - 1) {
        TouchscreenHitbox sp0 = *(const TouchscreenHitbox *)(ov112_021FF2DC + 7);
        if (TouchscreenHitbox_TouchNewIsIn(&sp0) != 0 || (0x10 & gSystem.newKeys)) {
            return TRUE;
        }
    }
    return FALSE;
}
