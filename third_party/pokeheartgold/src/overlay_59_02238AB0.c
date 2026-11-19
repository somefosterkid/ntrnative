#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "msgdata.h"
#include "sprite.h"
#include "text.h"

s32 ov59_02238834(Ov59ApricornApp *, s32, s32);

s32 ov59_02238A04(Ov59ApricornApp *, s32);

void ov59_02238AB0(Ov59ApricornApp *arg0, s32 arg1);

void ov59_02238AB0(Ov59ApricornApp *arg0, s32 arg1) {
    s32 var_r6;
    s32 var_r4;

    if (arg1 != 0) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 6, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->heapId);
        ov59_02238A04(arg0, arg0->quantity);
        ov59_02238834(arg0, 0, 0);
        ov59_02238834(arg0, 1, 0);
        FillWindowPixelBuffer(&arg0->windows[5], 0U);
        AddTextPrinterParameterizedWithColor(&arg0->windows[5], FONT_SYSTEM, arg0->apricornNames[arg0->selected], 0U, 0U, 0xFFU, 0x10200U, NULL);
        ScheduleWindowCopyToVram(&arg0->windows[5]);
        ReadMsgDataIntoString(arg0->msgData, 0x1A, arg0->unk_68);
        FillWindowPixelBuffer(&arg0->windows[10], 0U);
        AddTextPrinterParameterizedWithColor(&arg0->windows[10], FONT_SYSTEM, arg0->unk_68, 2U, 0U, 0xFFU, 0x10200U, NULL);
        ScheduleWindowCopyToVram(&arg0->windows[10]);
    } else {
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x11U);
        FillBgTilemapRect(arg0->bgConfig, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0x11U);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 1U);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 0U);
    }
    var_r6 = 9;
    var_r4 = 0x24 / 4;
    do {
        Sprite_SetDrawFlag(arg0->sprites[var_r4], arg1);
        Sprite_SetAnimActiveFlag(arg0->sprites[var_r4], 0);
        var_r6 += 1;
        var_r4 += 1;
    } while (var_r6 <= 0xE);
    if ((u32) arg0->apricornCounts[arg0->selected] < 0xAU) {
        Sprite_SetAnimCtrlSeq(arg0->sprites[13], 0x19);
        Sprite_SetAnimCtrlSeq(arg0->sprites[14], 0x1B);
        return;
    }
    Sprite_SetAnimCtrlSeq(arg0->sprites[13], 0x18);
    Sprite_SetAnimCtrlSeq(arg0->sprites[14], 0x1A);
}

s32 ov59_02238C40(Ov59ApricornApp *, s32);

void ov59_02238C24(Ov59ApricornApp *arg0);

void ov59_02238C24(Ov59ApricornApp *arg0) {
    if (arg0->unk_18 == 0) {
        ov59_02238C40(arg0, 1);
        return;
    }
    ov59_02238C40(arg0, arg0->unk_19 + 1);
}
