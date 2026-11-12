// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/brightness.c, which is the same code in
// Black 2/White 2. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "types.h"
#include "gfl/graphics.h"
#include "gfl/std.h"
#include "nitro/ioreg.h"
#include "system/brightness.h"

// Brightness blends of the screens that move to a target over a number of frames. The file's name is a guess: the
// ROM has no string for it

typedef struct {
    u32 planes;
    u32 screen;
    // The frames the transition takes
    u16 steps;
    s16 target;
    u8 unkC[4];
    // Toward the target, 1 or -1
    s8 direction;
    // The distance to the target, and how much of it each frame moves: quotient, plus 1 whenever the remainders add up
    // to a frame's worth
    s16 distance;
    s16 quotient;
    s16 remainder;
    u16 remainderSum;
    s16 brightness;
    BOOL active;
} BrightnessData;

static void func_02023e0c(BrightnessData *data);
static void func_02023e9c(BrightnessData *data, u16 steps, s16 target, s16 start, u32 planes, u32 screen);

// sMainBrightness and sSubBrightness
static BrightnessData data_0214691c;
static BrightnessData data_021468fc;

// Moves a screen's brightness a frame toward its target (BrightnessData_Step)
static void func_02023e0c(BrightnessData *data) {
    s8 direction = data->direction;
    int step = direction * data->quotient;
    s16 brightness = data->brightness;
    s16 target = data->target;
    BOOL done = FALSE;

    if (brightness + step != target && brightness != target) {
        data->brightness += step;
        data->remainderSum += data->remainder;
        if (data->remainderSum >= data->steps) {
            data->brightness += direction;
            if (data->brightness != target) {
                data->remainderSum -= data->steps;
            } else {
                done = TRUE;
            }
        }
    } else {
        data->brightness = target;
        done = TRUE;
    }
    if (data->screen & BRIGHTNESS_MAIN_SCREEN) {
        G2_SetBlendBrightness(data->planes, data->brightness);
    } else if (data->screen & BRIGHTNESS_SUB_SCREEN) {
        G2S_SetBlendBrightness(data->planes, data->brightness);
    }
    if (done == TRUE) {
        data->active = FALSE;
    }
}

// BrightnessData_Init
static void func_02023e9c(BrightnessData *data, u16 steps, s16 target, s16 start, u32 planes, u32 screen) {
    data->active = TRUE;
    data->planes = (u8)planes;
    data->screen = (u8)screen;
    data->distance = start - target;
    data->steps = steps;
    data->brightness = start;
    data->target = target;
    if (data->distance > 0) {
        data->direction = -1;
    } else {
        data->direction = 1;
        data->distance *= -1;
    }
    data->quotient = data->distance / steps;
    data->remainder = data->distance % steps;
    data->remainderSum = 0;
}

// BrightnessController_StartTransition
void func_02023ef4(u8 steps, s16 targetBrightness, s16 startBrightness, u32 planes, u32 screens) {
    if (steps == 0) {
        return;
    }
    if (screens & BRIGHTNESS_MAIN_SCREEN) {
        G2_SetBlendBrightness(planes, startBrightness);
        func_02023e9c(&data_0214691c, steps, targetBrightness, startBrightness, planes, BRIGHTNESS_MAIN_SCREEN);
    }
    if (screens & BRIGHTNESS_SUB_SCREEN) {
        G2S_SetBlendBrightness(planes, startBrightness);
        func_02023e9c(&data_021468fc, steps, targetBrightness, startBrightness, planes, BRIGHTNESS_SUB_SCREEN);
    }
}

// BrightnessController_SetScreenBrightness
void func_02023f60(s16 brightness, u32 planes, u32 screens) {
    if (screens & BRIGHTNESS_MAIN_SCREEN) {
        G2_SetBlendBrightness(planes, brightness);
    }
    if (screens & BRIGHTNESS_SUB_SCREEN) {
        G2S_SetBlendBrightness(planes, brightness);
    }
    func_02023f98(screens);
}

// BrightnessController_Reset
void func_02023f98(u32 screens) {
    if (screens & BRIGHTNESS_MAIN_SCREEN) {
        MI_CpuFill8(&data_0214691c, 0, sizeof(BrightnessData));
        data_0214691c.active = FALSE;
    }
    if (screens & BRIGHTNESS_SUB_SCREEN) {
        MI_CpuFill8(&data_021468fc, 0, sizeof(BrightnessData));
        data_021468fc.active = FALSE;
    }
}

// BrightnessController_Update
void func_02023fd8(void) {
    if (data_0214691c.active) {
        func_02023e0c(&data_0214691c);
    }
    if (data_021468fc.active) {
        func_02023e0c(&data_021468fc);
    }
}

// BrightnessController_IsTransitionComplete
BOOL func_02024004(u32 screens) {
    if (screens == BRIGHTNESS_BOTH_SCREENS) {
        if (data_0214691c.active == FALSE && data_021468fc.active == FALSE) {
            return TRUE;
        }
    } else if (screens == BRIGHTNESS_MAIN_SCREEN) {
        if (data_0214691c.active == FALSE) {
            return TRUE;
        }
    } else if (screens == BRIGHTNESS_SUB_SCREEN) {
        if (data_021468fc.active == FALSE) {
            return TRUE;
        }
    }
    return FALSE;
}
