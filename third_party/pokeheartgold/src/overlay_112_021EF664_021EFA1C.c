#include "global.h"

#include "screen_fade.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

u8 ov112_021EA08C(struct Ov112PokewalkerApp *, s32, s32);

s32 ov112_021EFA1C(Ov112PokewalkerApp *arg0);

s32 ov112_021EFA1C(Ov112PokewalkerApp *arg0) {
    PlayFanfare(0x4A2U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x37);
    arg0->unk_C = 8;
    return 0x23;
}

s32 ov112_021EFA44(Ov112PokewalkerApp *arg0);

s32 ov112_021EFA44(Ov112PokewalkerApp *arg0) {
    if (arg0->unk_1F376 != 0) {
        arg0->unk_1F376 = 0U;
        arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x35);
        arg0->unk_C = 9;
        return 0x23;
    }
    return 0xA;
}

s32 ov112_021EFA74(Ov112PokewalkerApp *arg0);

s32 ov112_021EFA74(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x36);
    arg0->unk_C = 0xA;
    PlayFanfare(0x4A2U);
    return 0x23;
}

s32 ov112_021EFA9C(void);

s32 ov112_021EFA9C(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 0xB;
}

s32 ov112_021EFAC0(void);

s32 ov112_021EFAC0(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0xC;
    }
    return 0xB;
}

s32 ov112_021EFAD4(void *arg0, void *arg1);

s32 ov112_021EFAD4(void *arg0, void *arg1) {
    s32 v3;
    s32 v4;
    u16 v0;
    u16 v1;
    void *v2;

    v2 = arg0;
    v3 = 0;
    v4 = 0;
    do {
        v1 = *(u16 *)((u8 *)v2 + 0xC);
        if ((v1 != 0) && ((u32)v1 <= 0x1EDU)) {
            v3 += 1;
        }
        v4 += 1;
        v2 += 0x10;
    } while (v4 < 3);
    if (((u32)(*(u8 *)arg1 << 0x1A) >> 0x1F) != 0) {
        v0 = *(u16 *)((u8 *)arg1 + 0x244);
        if ((v0 != 0) && ((u32)v0 <= 0x1EDU)) {
            v3 += 1;
        }
    }
    return v3;
}
