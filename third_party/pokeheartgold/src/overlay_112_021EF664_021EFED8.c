#include "global.h"

#include "screen_fade.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EFED8(void);

s32 ov112_021EFED8(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0x13;
    }
    return 0x12;
}

s32 ov112_021EFEEC(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x32);
    arg0->unk_C = 0x14;
    PlayFanfare(0x4A2U);
    return 0x23;
}

s32 ov112_021EFF14(void);

s32 ov112_021EFF14(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 0x15;
}

s32 ov112_021EFF38(void);

s32 ov112_021EFF38(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0x24;
    }
    return 0x15;
}

s32 ov112_021EFF4C(void);

s32 ov112_021EFF4C(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 0x17;
}

s32 ov112_021EFF70(void);

s32 ov112_021EFF70(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0x18;
    }
    return 0x17;
}
