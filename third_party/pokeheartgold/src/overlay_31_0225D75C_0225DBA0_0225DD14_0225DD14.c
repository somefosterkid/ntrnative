#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_03.h"
#include "overlay_31.h"
#include "pm_string.h"

void ov31_0225DD14(MartSubScreen *arg0) {
    String *sp10;
    s32 spC;
    MartData *sp8;
    int sp4;
    Window *var_r4_3;
    MartData *temp_r0_2;
    MartData *temp_r2;
    String *temp_r0_3;
    Window *var_r4;
    s32 temp_r0;
    s32 temp_r1;
    s32 var_r4_2;
    s32 var_r6;
    s32 var_r6_3;
    s32 temp_r7;
    Window *var_r6_2;
    var_r6 = 0;
    var_r4 = arg0->windows;
    do {
        FillWindowPixelBuffer(var_r4, 0U);
        var_r4++;
        var_r6 += 1;
    } while (var_r6 < 6);
    temp_r2 = arg0->martData;
    temp_r0 = temp_r2->count - temp_r2->pageOffset;
    sp4 = temp_r0;
    if (temp_r0 > 6) {
        sp4 = 6;
    } else if (temp_r0 < 0) {
        sp4 = 0;
    }
    var_r4_2 = 0;
    if (sp4 > 0) {
        var_r6_2 = arg0->windows;
        do {
            temp_r0_2 = arg0->martData;
            temp_r1 = var_r4_2 + temp_r0_2->pageOffset;
            spC = temp_r1;
            temp_r7 = temp_r0_2->items[spC];
            temp_r0_3 = NewString_ReadMsgData(arg0->itemMsgData, (s32)temp_r7);
            sp10 = temp_r0_3;
            ov31_0225DE00(arg0, var_r6_2, sp10, var_r4_2);
            String_Delete(temp_r0_3);
            if (ov31_0225E12C(arg0->martData, temp_r1, temp_r7) != 0) {
                MartData *temp_r0_4;
                temp_r0_4 = arg0->martData;
                sp8 = temp_r0_4;
                ov31_0225DE24(arg0->messageFormat, arg0->msgData, var_r6_2, ShopMenu_GetItemPrice(temp_r0_4, temp_r7), (s32)sp8->martType);
            }
            var_r4_2 += 1;
            var_r6_2++;
        } while (var_r4_2 < sp4);
    }
    var_r6_3 = 0;
    var_r4_3 = arg0->windows;
    do {
        ScheduleWindowCopyToVram(var_r4_3);
        var_r4_3++;
        var_r6_3 += 1;
    } while (var_r6_3 < 6);
    ov31_0225E0E4(arg0, sp4);
}
