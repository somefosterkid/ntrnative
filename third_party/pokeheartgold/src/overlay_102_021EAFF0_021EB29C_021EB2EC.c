#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_102_app_021eb1e0.h"

BgConfig *ov102_021EA268(void *);

s32 ov102_021EB524(void *, s32);

void ov102_021EB2EC(void *arg0, s32 arg1, s32 arg2);

void ov102_021EB2EC(void *arg0, s32 arg1, s32 arg2) {
    BgConfig *sp1C;
    BgConfig *temp_r0;
    s32 var_r6;
    s32 temp_r3;
    Window *var_r4;
    void *temp_r0_2;
    void *var_r5;

    var_r5 = arg0;
    temp_r0 = ov102_021EA268(*(void **)var_r5);
    sp1C = temp_r0;
    FillBgTilemapRect(temp_r0, 0U, 0U, 0x17U, 0xDU, 9U, 7U, 0x11U);
    ov102_021EB524(var_r5, arg2);
    if (arg1 == 0) {
        BgCommitTilemapBufferToVram(sp1C, 0U);
        return;
    }
    temp_r0_2 = *(void **)((u8 *)var_r5 + 0x5C);
    CopyToBgTilemapRect(sp1C, 0U, 0x17U, 0xDU, 9U, 7U, temp_r0_2 + 0xC, 0U, 0U, (u8)((u32)(*(u16 *)temp_r0_2) >> 3), (u8)((u32)(*(u16 *)((u8 *)temp_r0_2 + 2)) >> 3));
    var_r6 = 0;
    var_r4 = var_r5 + 0x24;
    do {
        FillWindowPixelBuffer(var_r4, 0xEU);
        temp_r3 = 0x38 - FontID_String_GetWidth(FONT_UNK_4, *(String **)((u8 *)var_r5 + 0x18), 0U);
        AddTextPrinterParameterizedWithColor(var_r4, FONT_UNK_4, *(String **)((u8 *)var_r5 + 0x18), (u32)(temp_r3 / 2), 0U, 0U, 0x3010EU, NULL);
        var_r6 += 1;
        var_r4 = (void *)((u8 *)var_r4 + (0x10));
        var_r5 += 4;
    } while (var_r6 < 2);
    BgCommitTilemapBufferToVram(sp1C, 0U);
}

void ov102_021EB3B4(void *arg0, s32 arg1);

void ov102_021EB3B4(void *arg0, s32 arg1) {
    BgConfig *temp_r0;
    BgConfig *temp_r6;
    s32 temp_r3;
    void *temp_r2;

    temp_r0 = ov102_021EA268(*(void **)arg0);
    temp_r6 = temp_r0;
    if (arg1 == 0) {
        FillBgTilemapRect(temp_r0, 0U, 0U, 0x18U, 0x14U, 8U, 4U, 0x11U);
        ClearWindowTilemapAndCopyToVram(&((Ov102App_021EB1E0 *)arg0)->unk_24[2]);
        ScheduleBgTilemapBufferTransfer(temp_r6, 0U);
        return;
    }
    temp_r2 = ((Ov102App_021EB1E0 *)arg0)->unk_5C;
    CopyToBgTilemapRect(temp_r0, 0U, 0x18U, 0x14U, 8U, 4U, temp_r2 + 0xC, 0U, 7U, (u8)((u32) * (u16 *)temp_r2 >> 3), (u8)((u32) * (u16 *)((u8 *)temp_r2 + 2) >> 3));
    FillWindowPixelBuffer(&((Ov102App_021EB1E0 *)arg0)->unk_24[2], 0xEU);
    temp_r3 = 0x30 - FontID_String_GetWidth(FONT_UNK_4, ((Ov102App_021EB1E0 *)arg0)->unk_18[2], 0U);
    AddTextPrinterParameterizedWithColor(&((Ov102App_021EB1E0 *)arg0)->unk_24[2], FONT_UNK_4, ((Ov102App_021EB1E0 *)arg0)->unk_18[2], (u32)((s32)temp_r3 / 2), 0U, 0U, 0x3010EU, NULL);
    ScheduleBgTilemapBufferTransfer(temp_r6, 0U);
}
