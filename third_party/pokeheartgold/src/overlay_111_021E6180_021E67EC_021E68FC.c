#include "global.h"

#include "overlay_111.h"

void ov111_021E68FC(SwapMonText *text, s32 msgId) {
    text->printerId = ov111_021E6A74(text->bgConfig, &text->window, &text->string, text->msgFormat, text->msgData, msgId, text->bgLayer, text->heapID, text->options);
}
