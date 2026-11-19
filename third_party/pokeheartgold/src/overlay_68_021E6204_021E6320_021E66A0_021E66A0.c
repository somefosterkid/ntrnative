#include "global.h"
#include "overlay_68_move_relearner.h"

#include "bg_window.h"

void ov68_021E66A0(MoveRelearner *param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7);

void ov68_021E66A0(MoveRelearner *relearner, u8 x, u8 y, u8 width, u8 height, u8 srcX, u8 srcY) {
    NNSG2dScreenData *screen;

    screen = relearner->screenData;
    CopyToBgTilemapRect(relearner->bgConfig, 7, x, y, width, height, screen->rawData, srcX, srcY, (u8)((u32)screen->screenWidth >> 3), (u8)((u32)screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(relearner->bgConfig, 7);
}
