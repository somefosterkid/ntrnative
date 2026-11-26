#include "global.h"
struct GTSApplicationState;

#include "render_window.h"
#include "sprite.h"
#include "player_data.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02238F80(struct GTSApplicationState *param_1);

void ov70_02238F9C(Sprite *sprite, int x, int y);

void ov70_02238FB4(s32 arg0);
void ov70_02238FE0(void);
s32 ov70_02239134(BgConfig *);
s32 ov70_02239304(void *);
s32 ov70_02239330(void *);
s32 ov70_02239414(void *);
s32 ov70_022394B8(void *);
s32 ov70_02239614(GTSApplicationState *arg0);
s32 ov70_02239B00(void *);
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov70_02240EF4(void *, u32);
#else
s32 ov70_02240EF4(void *, u32);
#endif
s32 ov70_02241358(void *);
SysTask *ov70_02245124(void *);
s32 ov70_02238FF4(void *arg0);

void ov70_02238F80(GTSApplicationState *param_1) {
    if (*((WaitingIcon * *)&param_1->unk_11BC) != NULL) {
        sub_0200F450(*((WaitingIcon * *)&param_1->unk_11BC));
        param_1->unk_11BC = 0;
    }
    return;
}

void ov70_02238F9C(Sprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = FX32_ONE * x;
    pos.y = FX32_ONE * y;

    Sprite_SetMatrix(sprite, &pos);
}

void ov70_02238FB4(s32 arg0) {
    if (arg0 != 0) {
        G2_SetBlendBrightness(0x1E, -7);
        return;
    }
    G2S_SetBlendBrightness(0x1E, -7);
}

void ov70_02238FE0(void) {
    *(s16 *)REG_BLDCNT_ADDR = 0;
    *(s16 *)REG_DB_BLDCNT_ADDR = 0;
}

s32 ov70_02238FF4(void *arg0) {
    ov70_02239614(arg0);
    *(u16 *)REG_POWCNT_ADDR &= 0xFFFF7FFF;
    ov70_02239134(*((void **)&((GTSApplicationState *)arg0)->bgConfig));
    ov70_02239330(arg0);
    ov70_022394B8(arg0);
    ov70_02239414(arg0);
    ov70_02245124(arg0);
    ov70_02239B00(arg0);
    Sound_SetSceneAndPlayBGM(0x17U, 0x484U, 1);
    if (((GTSApplicationState *)arg0)->hasPlayerDescended == 0) {
        ov70_02239304(arg0);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 0;
        ((GTSApplicationState *)arg0)->hasPlayerDescended = 1U;
        ov70_02240EF4(arg0, PlayerProfile_GetTrainerGender(*(PlayerProfile **)((u8 *)(*(void **)arg0) + 0x1C)));
    } else {
        if ((*((s32 *)&((GTSApplicationState *)arg0)->fadeBothScreens) == 1) && (GXS_GetMasterBrightness() != 0)) {
            ov70_02241358(arg0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
        } else {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
        }
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 5;
    }
    *((s32 *)&((GTSApplicationState *)arg0)->fadeBothScreens) = 0;
    return 2;
}
