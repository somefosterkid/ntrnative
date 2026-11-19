#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_102_app_021eb1e0.h"

extern WindowTemplate ov102_021EC808;

void ov102_021EB46C(Window *arg0, BgConfig *arg1);

void ov102_021EB46C(Window *arg0, BgConfig *arg1) {
    BgConfig *sp4;
    Window *var_r4;
    Window *sp0;
    s32 var_r7;
    WindowTemplate *var_r6;
    Window *var_r5;
    Window *temp_r0;
    var_r4 = arg0;
    var_r6 = &ov102_021EC808;
    sp0 = arg0;
    sp4 = arg1;
    var_r7 = 0;
    var_r4 = (void *)(((u8 *)var_r4) + 0x24);
    var_r5 = arg0;
    do {
        AddWindow(sp4, var_r4, var_r6);
        ((Ov102App_021EB1E0 *)var_r5)->unk_18[0] = NewString_ReadMsgData(((Ov102App_021EB1E0 *)sp0)->unk_14, var_r7);
        var_r6 = (void *)(((u8 *)var_r6) + 8);
        var_r4 = (void *)(((u8 *)var_r4) + 0x10);
        var_r5 = (void *)(((u8 *)var_r5) + 4);
        var_r7 = var_r7 + 1;
    } while (3U > ((u32)var_r7));
    PutWindowTilemap(((void *)sp0) + 0x24);
    FillWindowPixelBuffer(((void *)sp0) + 0x24, 0xEU);
    PutWindowTilemap(((void *)sp0) + 0x34);
    temp_r0 = ((void *)sp0) + 0x34;
    sp0 = temp_r0;
    FillWindowPixelBuffer(temp_r0, 0xEU);
}
