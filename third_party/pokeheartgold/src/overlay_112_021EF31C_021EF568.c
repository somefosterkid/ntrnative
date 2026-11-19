#include "global.h"

#include "overlay_112_pokewalker_app.h"
#include "screen_fade.h"
#include "sound.h"

s32 ov112_021EF568(Ov112PokewalkerApp *arg0);

s32 ov112_021EF568(Ov112PokewalkerApp *arg0) {
    arg0->unk_1D750 = arg0->saveData;
    arg0->unk_1D754 = arg0->unk_9DFC;
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, 6, 1, HEAP_ID_154);
    GF_SndStartFadeOutBGM(0U, 0x10U);
    return 2;
}
