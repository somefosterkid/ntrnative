#include "global.h"

#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "system.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

struct GridInputHandler;

struct SysTask;

struct SysTaskQueue;

struct System;

extern const TouchscreenHitbox ov83_022484F4[3];

extern const int ov83_0224858C[7];

int ov83_02247BC4(GridInputHandler *inputHandler);

int ov83_02247BC4(GridInputHandler *inputHandler) {
    int touch;
    int input = GridInputHandler_HandleInput_NoHold(inputHandler);
    switch (input) {
    case GRID_MENU_BUTTON_MODE:
    case GRID_MENU_CURSOR_MOVE:
    case GRID_MENU_CANCEL:
        return input;
    case GRID_MENU_NOTHING_CHOSEN:
        if (gSystem.newKeys & PAD_KEY_LEFT) {
            int next = GridInputHandler_GetNextInput(inputHandler);
            if (next == 0 || next == 2 || next == 4) {
                return 6;
            }
        }
        if (gSystem.newKeys & PAD_KEY_RIGHT) {
            int next = GridInputHandler_GetNextInput(inputHandler);
            if (next == 1 || next == 3 || next == 5) {
                return 7;
            }
        }
        break;
    default:
        return ov83_0224858C[input];
    }
    touch = TouchscreenHitbox_FindRectAtTouchNew(ov83_022484F4);
    if (touch == 0) {
        return 6;
    }
    if (touch == 1) {
        return 7;
    }
    return -1;
}
