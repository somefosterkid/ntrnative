#include "global.h"

#include "overlay_111.h"

void ov111_021E6934(SwapMonText *text, Window *window, u32 msgId, u8 x, u8 y) {
    ov111_021E6B30(text->bgConfig, window, text->msgFormat, text->msgData, msgId, text->bgLayer, x, y, 0x10200, text->heapID);
    return;
}
