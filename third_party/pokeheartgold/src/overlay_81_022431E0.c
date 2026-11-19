#include "global.h"

#include "bg_window.h"
#include "sys_task.h"
#include "unk_02019BA4.h"

void ov81_022431E0(BgConfig *arg0, s32 arg1);

void GridInputHandler_Free(GridInputHandler *inputHandler);
void ov81_02243220(GridInputHandler *inputHandler);

void ov81_022431E0(BgConfig *bgConfig, s32 arg1) {
    u8 y;
    u8 height;

    if (arg1 == 0) {
        y = 0xB;
        height = 7;
    } else {
        y = 0xE;
        height = 4;
    }
    FillBgTilemapRect(bgConfig, 5U, 0U, 0U, (u8)(s32)y, 0x20U, (u8)(s32)height, 9U);
    ScheduleBgTilemapBufferTransfer(bgConfig, 5U);
}

void ov81_02243220(GridInputHandler *inputHandler) {
    GridInputHandler_Free(inputHandler);
}
