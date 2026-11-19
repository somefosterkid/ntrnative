#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_0201956C.h"

BOOL sub_02019978(UnkStruct_0201956C *frames, int a1);

void sub_02019934(UnkStruct_0201956C *frames) {
    u32 i;
    if (frames->moving == 0) {
        return;
    }
    frames->moving = 0;
    for (i = 0; i < frames->count; i++) {
        if (sub_02019978(frames, i) == 1) {
            frames->moving = 1;
        }
    }
}

void sub_020197F4(UnkStruct_0201956C *a0, int a1);

BOOL sub_02019978(UnkStruct_0201956C *frames, int a1) {
    BGWinFrameEntry *entry = &frames->entries[a1];
    if (entry->moving == 0) {
        return FALSE;
    }
    sub_0201980C(frames, a1);
    frames->entries[a1].x += frames->entries[a1].moveX;
    frames->entries[a1].y += frames->entries[a1].moveY;
    sub_020197F4(frames, a1);
    frames->entries[a1].moveCount--;
    if (frames->entries[a1].moveCount == 0) {
        frames->entries[a1].moving = 0;
        return FALSE;
    }
    return TRUE;
}
