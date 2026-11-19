#include "global.h"
struct UnkStruct_ov64_0222E060;

#include "bg_window.h"
#include "menu_input_state.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43.h"
#include "overlay_43_palpad.h"

void ov43_0222A24C(BgConfig *param_1, Window *param_2, u8 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8, u16 param_9, u8 param_10);

void ov43_0222A290(PalPadShared *shared, PalPadArgs *args, enum HeapID heapID);
void ov43_0222A2F0(PalPadShared *shared, PalPadArgs *args);
void ov43_0222A318(struct UnkStruct_ov64_0222E060 *arg0, s8 arg1, s8 arg2);

void ov43_0222A24C(BgConfig *param_1, Window *param_2, u8 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8, u16 param_9, u8 param_10) {
    InitWindow(param_2);
    AddWindowParameterized(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9);
    FillWindowPixelBuffer(param_2, param_10);
    return;
}

void ov43_0222A290(PalPadShared *shared, PalPadArgs *args, enum HeapID heapID) {
    Options *options;
    s8 v0;

    shared->saveData = args->saveData;
    shared->name = String_New(0x80U, heapID);
    shared->code = String_New(0x80U, heapID);
    shared->inputState = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(shared->saveData);
    shared->textSpeed = Options_GetTextFrameDelay(options);
    shared->frame = (s8)Options_GetFrame(options);
    if (Options_GetButtonMode(options) != 1) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    shared->lrPaging = v0;
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetCanTouchSpeedUpPrint(1);
}

void ov43_0222A2F0(PalPadShared *shared, PalPadArgs *args) {
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    MenuInputStateMgr_SetState(args->menuInputStatePtr, (enum MenuInputState)shared->inputState);
    String_Delete(shared->name);
    String_Delete(shared->code);
}

void ov43_0222A318(UnkStruct_ov64_0222E060 *arg0, s8 arg1, s8 arg2) {
    *((s8 *)&arg0->unk_05) = arg1;
    *((s8 *)&arg0->unk_06) = arg2;
}
