#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_59_juice_feed_app.h"

extern WindowTemplate ov59_0223CA40;

void ov59_0223B740(Ov59JuiceFeedApp *arg0);

void ov59_0223B740(Ov59JuiceFeedApp *arg0) {
    Window *sp18;
    void *sp14;
    WindowTemplate *var_r4;
    Window *var_r5;
    s32 temp_r7;
    s32 var_r6;
    u32 var_r6_2;
    u32 var_r4_2;

    var_r6 = 0;
    var_r4 = &ov59_0223CA40;
    sp14 = arg0;
    var_r5 = arg0->windows;
    do {
        AddWindow(*(BgConfig **)((u8 *)sp14 + 0x54), var_r5, var_r4);
        FillWindowPixelBuffer(var_r5, 0U);
        var_r4 = (void *)((u8 *)var_r4 + (8));
        var_r5++;
        var_r6 += 1;
    } while (var_r6 <= 4);
    var_r6_2 = 0xC1;
    var_r6 = 0;
    var_r4_2 = 4;
    sp18 = (Window *)((u8 *)sp14 + 0x1B4);
    do {
        temp_r7 = var_r6 + 5;
        AddWindowParameterized(*(BgConfig **)((u8 *)sp14 + 0x54), sp18 + temp_r7, 2U, 1U, (u8)(s32)var_r4_2, 0x10, 2U, 0xCU, (u16)(s32)var_r6_2);
        FillWindowPixelBuffer(sp18 + temp_r7, 0U);
        var_r6 += 1;
        var_r6_2 += 0x20;
        var_r4_2 += 4;
    } while (var_r6 < 5);
    *(YesNoPrompt **)((u8 *)sp14 + 0x254) = YesNoPrompt_Create(*(enum HeapID *)sp14);
}
