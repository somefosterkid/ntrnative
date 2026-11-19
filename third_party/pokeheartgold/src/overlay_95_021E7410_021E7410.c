#include "global.h"

#include "error_handling.h"
#include "overlay_95_egg_hatch_app.h"
#include "sprite.h"
#include "sprite_system.h"

extern u32 ov95_021E7818;

void ov95_021E7410(EggHatchPrompt *param_1);

void ov95_021E7410(EggHatchPrompt *param_1) {
    s32 idx;

    if (param_1->cursorSprite == NULL) {
        GF_AssertFail();
    }
    switch (param_1->selection) {
    case 1:
        idx = 0;
        break;
    case 2:
        idx = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ManagedSprite_SetPositionXY(param_1->cursorSprite, *(s16 *)((u8 *)&ov95_021E7818 + idx * 4), *(s16 *)((u8 *)&ov95_021E7818 + 2 + idx * 4));
    return;
}
