#include "global.h"

#include "bg_window.h"
#include "overlay_70_gtsapplication_state.h"

extern s32 ov70_022454EC;

void ov70_0223ACF4(GTSApplicationState *arg0);

void ov70_0223ACF4(GTSApplicationState *arg0) {
    s32 sp18;
    s32 sp14;
    s32 *var_r4;
    Window *var_r5;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r2;
    u32 var_r7;

    AddWindowParameterized(arg0->bgConfig, &arg0->bottomInstructionWindow, 4U, 2U, 0x13U, 0x1BU, 4U, 1U, 0x28U);
    FillWindowPixelBuffer(&arg0->bottomInstructionWindow, 0U);
    AddWindowParameterized(arg0->bgConfig, arg0->menuButtonWindows, 4U, 1U, 0x15U, 0xDU, 2U, 1U, 0x94U);
    FillWindowPixelBuffer(arg0->menuButtonWindows, 0U);
    AddWindowParameterized(arg0->bgConfig, &arg0->menuButtonWindows[1], 4U, 0x11U, 0x15U, 0xDU, 2U, 1U, 0xAEU);
    FillWindowPixelBuffer(&arg0->menuButtonWindows[1], 0U);
    sp14 = 1;
    sp18 = 0;
    var_r4 = &ov70_022454EC;
    var_r7 = 0xC8;
    var_r5 = &arg0->infoWindows[13];
    do {
        temp_r2 = *(s32 *)((u8 *)var_r4 + 0x10);
        temp_r0_2 = *(s32 *)((u8 *)var_r4 + 4);
        if (temp_r2 == 0) {
            AddWindowParameterized(arg0->bgConfig, var_r5, temp_r2, *(s32 *)var_r4, (s32)temp_r0_2, (s32) * (s32 *)((u8 *)var_r4 + 8), (s32) * (s32 *)((u8 *)var_r4 + 0xC), 0xDU, (s32)sp14);
            sp14 += *(s32 *)((u8 *)var_r4 + 8) * *(s32 *)((u8 *)var_r4 + 0xC);
        } else {
            AddWindowParameterized(arg0->bgConfig, var_r5, temp_r2, *(s32 *)var_r4, (s32)temp_r0_2, (s32) * (s32 *)((u8 *)var_r4 + 8), (s32) * (s32 *)((u8 *)var_r4 + 0xC), 1U, (s32)var_r7);
            var_r7 += *(s32 *)((u8 *)var_r4 + 8) * *(s32 *)((u8 *)var_r4 + 0xC);
        }
        FillWindowPixelBuffer(var_r5, 0U);
        var_r4 += 0x5;
        var_r5 = (void *)((u8 *)var_r5 + (0x10));
        temp_r0 = sp18 + 1;
        sp18 = temp_r0;
    } while (temp_r0 < 0x10);
}
