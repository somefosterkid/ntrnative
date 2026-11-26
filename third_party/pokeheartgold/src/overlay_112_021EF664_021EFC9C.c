#include "global.h"

#include "screen_fade.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EFC9C(void);

s32 ov112_021EFC9C(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0xE;
    }
    return 0xD;
}

s32 ov112_021EFCB0(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x31);
    arg0->unk_C = 0xF;
    PlayFanfare(0x4A2U);
    return 0x23;
}

s32 ov112_021EFCD8(void);

s32 ov112_021EFCD8(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 0x10;
}

s32 ov112_021EFCFC(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (IsPaletteFadeFinished() == 1) {
        v0 = 0;
        do {
            ov112_021EA688(arg0, v0 + 9);
            v0 += 1;
        } while (v0 < 4);
        return 0x11;
    }
    return 0x10;
}
