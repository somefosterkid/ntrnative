#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "yes_no_prompt.h"

extern WindowTemplate ov59_0223C798[];

void ov59_022397E4(Ov59ApricornApp *arg0);

void ov59_022397E4(Ov59ApricornApp *arg0) {
    s32 var_r6;
    u32 var_r7;
    s32 var_r4_2;
    u32 var_r6_2;

    for (var_r6 = 0; var_r6 < 0xB; var_r6++) {
        AddWindow(arg0->bgConfig, &arg0->windows[var_r6], &ov59_0223C798[var_r6]);
        FillWindowPixelBuffer(&arg0->windows[var_r6], 0U);
    }
    var_r7 = 0x1BC;
    var_r4_2 = 0;
    var_r6_2 = 6;
    do {
        AddWindowParameterized(arg0->bgConfig, &arg0->windows[var_r4_2 + 0xB], 2U, (u8)((6 * (var_r4_2 % 2)) + 5), (u8)(s32)var_r6_2, 2U, 2U, 0xCU, (u16)(s32)var_r7);
        FillWindowPixelBuffer(&arg0->windows[var_r4_2 + 0xB], 0U);
        var_r4_2 += 1;
        var_r7 += 4;
        var_r6_2 += 2;
    } while ((s32)var_r4_2 < 7);
    arg0->yesNoPrompt = YesNoPrompt_Create(arg0->heapId);
}
