#include "global.h"

#include "sprite.h"
#include "overlay_34.h"

void ov34_0225DE04(Ov34Work *work) {
    work->count = (u16)work->chatLog->count;
    if (((u32) work->count > 3U) && ((u32) work->prevCount <= 3U)) {
        work->scrollbarVisible = 1U;
        work->scroll = (u16)(work->count - 3);
    }
    Sprite_SetDrawFlag(work->sprites[2], (s32) work->scrollbarVisible);
    if ((work->scrollbarVisible != 0) && (ov34_0225E5E4(work) == 1)) {
        ov34_0225DDB8(work->sprites[2], ((s32)(0x60 * work->scroll) / (s32)(work->count - 3)) + 0x30);
    }
    work->prevCount = (u16) work->count;
}
