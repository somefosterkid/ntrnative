#include "global.h"

/* HDR-OVERLAY {"StartBrightnessTransition": {"file": "include/brightness.h", "new": "void StartBrightnessTransition(u8 stepCount, s16 targetBrightness, s16 startBrightness, GXBlendPlaneMask surfaceMask, u32 screenMask);", "old": "void StartBrightnessTransition(u16 stepCount, s16 targetBrightness, s16 startBrightness, GXBlendPlaneMask surfaceMask, u32 screenMask);"}} */
#define StartBrightnessTransition StartBrightnessTransition__hdr

/* HDR-OVERLAY defines done */
#include "brightness.h"

/* HDR-OVERLAY end */
#undef StartBrightnessTransition

#if defined(SDK_PORT) || defined(NONMATCHING)
void StartBrightnessTransition(u16 stepCount, s16 targetBrightness, s16 startBrightness, GXBlendPlaneMask surfaceMask, u32 screenMask);
#else
void StartBrightnessTransition(u8 stepCount, s16 targetBrightness, s16 startBrightness, GXBlendPlaneMask surfaceMask, u32 screenMask);
#endif

void ov07_0221D2F4(void *arg0, s32 arg1);

void ov07_0221D2F4(void *arg0, s32 arg1) {
    if (arg1 != 0) {
        StartBrightnessTransition((u16)(u8) * (s32 *)((u8 *)arg0 + 4), 0x10, 0, (GXBlendPlaneMask)(0x3C), 1U);
        return;
    }
    StartBrightnessTransition((u16)(u8) * (s32 *)((u8 *)arg0 + 4), 0, 0x10, (GXBlendPlaneMask)(0x3C), 1U);
}
