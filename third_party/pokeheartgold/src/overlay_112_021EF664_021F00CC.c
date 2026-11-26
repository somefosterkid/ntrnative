#include "global.h"

#include "screen_fade.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021F00CC(void);

s32 ov112_021F00CC(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0x1A;
    }
    return 0x19;
}

s32 ov112_021F00E0(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (arg0->unk_9D7A == 0) {
        v0 = ov112_021EA08C(arg0, 0, 0x2E);
    } else {
        v0 = ov112_021EA08C(arg0, 0, 0x2D);
    }
    arg0->unk_1E524 = v0;
    arg0->unk_C = 0x1B;
    return 0x23;
}
