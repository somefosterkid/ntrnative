#include "global.h"
struct HallOfFameShowcase;

#include "unk_02019BA4.h"
#include "screen_fade.h"
#include "overlay_64_hall_of_fame_showcase.h"

void ov64_021E6260(struct HallOfFameShowcase *arg0);

void ov64_021E6270(void);
void ov64_021E6274(void);
s32 ov64_021E6BD8();
void ov64_021E6278(void);
void ov64_021E6280(void);
void ov64_021E6288(void);
void ov64_021E62A8(void);

void ov64_021E6260(HallOfFameShowcase *arg0) {
    GridInputHandler_Free(arg0->gridInput);
}

void ov64_021E6270(void) {
}

void ov64_021E6274(void) {
}

void ov64_021E6278(void) {
    ov64_021E6BD8();
}

void ov64_021E6280(void) {
    ov64_021E6BD8();
}

void ov64_021E6288(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_59);
}

void ov64_021E62A8(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_59);
}
