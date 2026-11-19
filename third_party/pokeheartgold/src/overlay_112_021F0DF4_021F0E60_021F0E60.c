#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_112_sprite_app.h"

extern const u8 ov112_021FF2DC[];

s32 ov112_021F0E60(Ov112SpriteApp *arg0);

s32 ov112_021F0E60(Ov112SpriteApp *arg0) {
    TouchscreenHitbox sp0 = *(const TouchscreenHitbox *)(ov112_021FF2DC + 0xB);
    if ((TouchscreenHitbox_TouchNewIsIn(&sp0) != 0) || (2 & gSystem.newKeys)) {
        PlaySE(0x5DCU);
        ManagedSprite_ResetSpriteAnimCtrlState(*((ManagedSprite * *)&arg0->sprites[2]));
        return 1;
    }
    return 0;
}
