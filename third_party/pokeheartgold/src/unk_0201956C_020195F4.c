#include "global.h"

#include "heap.h"
#include "unk_0201956C.h"

void sub_020195F4(UnkStruct_0201956C *frames, int index, int bg, int width, int height);

void sub_020195F4(UnkStruct_0201956C *frames, int index, int bg, int width, int height) {
    BGWinFrameEntry *frame = &frames->entries[index];

    frame->screen = Heap_Alloc(frames->heapId, width * height * sizeof(u16));
    frame->width = width;
    frame->height = height;
    frame->bg = bg;
    frame->x = 0;
    frame->y = 0;
    frame->moving = 0;
    frame->hidden = 1;
    frame->left = 0;
    frame->right = 32;
    frame->top = 0;
    frame->bottom = 24;
}
