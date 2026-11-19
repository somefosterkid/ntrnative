#include "global.h"

#include "gf_gfx_planes.h"
#include "system.h"
#include "sys_task.h"
#include "screen_fade.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

s32 ov106_021E5D38(void);

s32 ov106_021E5D48(void);

void ov106_021E5900(LegendaryCinematic *cinematic) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0U);
    GfGfx_EngineBSetPlanes(0U);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    ov106_021E5D38();
    ov106_021E5D48();
    ov106_021E5D70(cinematic);
}

void ov106_021E594C(LegendaryCinematic *cinematic) {
    ov106_021E5DFC(cinematic);
}

void ov106_021E5954(LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}

void ov106_021E597C(LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}

void ov106_021E59A4(LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}

void ov106_021E59D0(LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}
