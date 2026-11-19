#include "global.h"

#include "message_format.h"
#include "screen_fade.h"
#include "overlay_112_pokewalker_app.h"

u8 ov112_021EA08C(Ov112PokewalkerApp *, u32, u32);

u32 ov112_021F0114(Ov112PokewalkerApp *param_1);

s32 ov112_021F015C(void);
s32 ov112_021F0180(void);
s32 ov112_021F0194(void);
s32 ov112_021F01B8(void);

u32 ov112_021F0114(Ov112PokewalkerApp *param_1) {
    u32 uVar1;

    if (param_1->unk_1EC78 != 0) {
        BufferPokewalkerCourseName(param_1->msgFormat, 4, param_1->unk_1EC78 & 0xff);
        uVar1 = ov112_021EA08C(param_1, 0, 0x30);
        param_1->unk_1E524 = uVar1;
        param_1->unk_C = 0x1c;
        param_1->unk_1EC78 = 0;
        return 0x23;
    }
    return 0x1c;
}

s32 ov112_021F015C(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 0x1D;
}

s32 ov112_021F0180(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0xC;
    }
    return 0x1D;
}

s32 ov112_021F0194(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 0x1F;
}

s32 ov112_021F01B8(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0x20;
    }
    return 0x1F;
}
