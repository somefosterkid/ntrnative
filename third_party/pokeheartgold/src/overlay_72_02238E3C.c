#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_72_wfc_connect.h"

void ov72_02238E3C(void *arg0, void *arg1);

void ov72_0223A3E0(Window *, String *, u32, u32, u32, u32);
void ov72_02238EE4(NintendoWfcConnect *param_1);
void ov72_02238FFC(NintendoWfcConnect *param_1);

void ov72_02239040(NintendoWfcConnect *arg0);
void ov72_02239098(NintendoWfcConnect *arg0);
s32 ov72_02238680(void *, s32, s32);
s32 ov72_0223A350(void *, MsgData *, s32, s32, s32);
s32 ov72_022390D8(NintendoWfcConnect *arg0);

void ov72_02238E3C(void *arg0, void *arg1) {
    u32 v0;
    NintendoWfcPaletteCycle *cycle = arg1;

    if ((cycle->active != 0) && (cycle->tick = (u8)(cycle->tick ^ 1), ((1 & cycle->tick) == 0))) {
        if ((u32)cycle->screens <= 1U) {
            GX_LoadBGPltt(&cycle->cyclePltt[cycle->index * 16], 0U, 0x20U);
        }
        v0 = cycle->screens;
        if ((v0 == 0) || (v0 == 2)) {
            GXS_LoadBGPltt(&cycle->cyclePltt[cycle->index * 16], 0U, 0x20U);
        }
        if (cycle->direction == 0) {
            cycle->index = (s16)(cycle->index + 1);
            if ((s32)cycle->index >= 0x15) {
                cycle->index = 0x13;
                cycle->direction = (s8)(1 ^ cycle->direction);
            }
        } else {
            cycle->index = (s16)(cycle->index - 1);
            if ((s32)cycle->index < 0) {
                cycle->index = 1;
                cycle->direction = (s8)(cycle->direction ^ 1);
            }
        }
    }
}

void ov72_02238EE4(NintendoWfcConnect *param_1) {
    AddWindowParameterized(param_1->bgConfig, &param_1->errorWindow, 0, 4, 4, 0x17, 0x10, 0xd, 0xa4);
    FillWindowPixelBuffer(&param_1->errorWindow, 0);
    AddWindowParameterized(param_1->bgConfig, &param_1->roomWindow, 0, 0x1a, 0xe, 4, 2, 0xd, 0x94);
    FillWindowPixelBuffer(&param_1->roomWindow, 0xf);
    AddWindowParameterized(param_1->bgConfig, &param_1->rankWindow, 0, 0x1a, 0xe, 4, 2, 0xd, 0x9c);
    FillWindowPixelBuffer(&param_1->rankWindow, 0xf);
    AddWindowParameterized(param_1->bgConfig, &param_1->titleWindow, 0, 4, 1, 0x18, 2, 0xd, 0x214);
    FillWindowPixelBuffer(&param_1->titleWindow, 0);
    ov72_0223A3E0(&param_1->titleWindow, param_1->titleString, 0, 1, 1, 0xf0e00);
    AddWindowParameterized(param_1->bgConfig, &param_1->msgWindow, 0, 2, 0x13, 0x1b, 4, 0xd, 0x28);
    FillWindowPixelBuffer(&param_1->msgWindow, 0xf);
    return;
}

void ov72_02238FFC(NintendoWfcConnect *param_1) {
    RemoveWindow(&param_1->msgWindow);
    RemoveWindow(&param_1->titleWindow);
    RemoveWindow(&param_1->rankWindow);
    RemoveWindow(&param_1->roomWindow);
    RemoveWindow(&param_1->errorWindow);
    return;
}

void ov72_02239040(NintendoWfcConnect *arg0) {
    arg0->msgString = String_New(0xB4U, HEAP_ID_67);
    arg0->roomString = String_New(4U, HEAP_ID_67);
    arg0->rankString = String_New(3U, HEAP_ID_67);
    arg0->titleString = NewString_ReadMsgData(arg0->msgDataTower, 0xC);
    arg0->errorString = String_New(0x100U, HEAP_ID_67);
}

void ov72_02239098(NintendoWfcConnect *arg0) {
    String_Delete(arg0->errorString);
    String_Delete(arg0->titleString);
    String_Delete(arg0->rankString);
    String_Delete(arg0->roomString);
    String_Delete(arg0->msgString);
}

s32 ov72_022390D8(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataError, 0x11, 1, 0xF0F);
    ov72_02238680(arg0, 0x30, 1);
    return 3;
}
