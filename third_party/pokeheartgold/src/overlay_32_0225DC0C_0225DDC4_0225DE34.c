#include "global.h"

#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"

extern TouchscreenHitbox _0225E15C;

u32 ov32_0225DE34(NamePickerData *data) {
    u32 input;
    int next;

    input = TouchscreenHitbox_FindRectAtTouchNew(&_0225E15C);
    if (input == 0) {
        PlaySE(0x5E0);
        return ov32_0225DF9C(data, -1);
    }
    if (input == 1) {
        PlaySE(0x5E0);
        return ov32_0225DF9C(data, 1);
    }
    input = GridInputHandler_HandleInput_AllowHold(data->gridInput);
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        PlaySE(0x5DD);
        data->selection = input + data->page * 8;
        return ov32_0225E0FC(data, (input & 1) * 16 + 2, (input >> 1) * 5, 3);
    case 8:
    case GRID_MENU_CANCEL:
        PlaySE(0x5DD);
        data->selection = -2;
        return ov32_0225E0A8(data, 1, 3, 2, 3);
    case GRID_MENU_NOTHING_CHOSEN:
        next = GridInputHandler_GetNextInput(data->gridInput);
        if ((gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) && (next == 1 || next == 3 || next == 5 || next == 7)) {
            PlaySE(0x5DC);
            return ov32_0225DF9C(data, 1);
        }
        if ((gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) && (next == 0 || next == 2 || next == 4 || next == 6)) {
            PlaySE(0x5DC);
            return ov32_0225DF9C(data, -1);
        }
        break;
    case GRID_MENU_CURSOR_MOVE:
        PlaySE(0x5DC);
        break;
    case GRID_MENU_BUTTON_MODE:
        break;
    }
    return 0;
}
