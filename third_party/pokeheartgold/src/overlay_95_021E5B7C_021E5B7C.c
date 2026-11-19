#include "global.h"

#include "palette.h"
#include "render_window.h"
#include "screen_fade.h"

void ov95_021E5B7C(void);

void ov95_021E5B7C(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_70);
}

void ov95_021E5B9C(void);

void ov95_021E5B9C(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_70);
}

void ov95_021E5BBC(BgConfig *arg0, PaletteData *arg1, u8 arg2);

void ov95_021E5BBC(BgConfig *arg0, PaletteData *data, u8 frame) {
    LoadUserFrameGfx2(arg0, GF_BG_LYR_SUB_0_F, 0x14U, 0xFU, (u8)(s32)frame, HEAP_ID_70);
    PaletteData_LoadNarc(data, NARC_a_0_3_8, (s32)sub_0200E640((s32)frame), HEAP_ID_70, PLTTBUF_MAIN_BG, 0x20U, 0xC0U);
    LoadUserFrameGfx1(arg0, GF_BG_LYR_SUB_0_F, 0x32U, 0xDU, 0U, HEAP_ID_70);
    PaletteData_LoadNarc(data, NARC_a_0_3_8, (s32)sub_0200E3D8(), HEAP_ID_70, PLTTBUF_MAIN_BG, 0x20U, 0xD0U);
    PaletteData_LoadNarc(data, NARC_graphic_font, 8, HEAP_ID_70, PLTTBUF_MAIN_BG, 0x20U, 0xE0U);
}
