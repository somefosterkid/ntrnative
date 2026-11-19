#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_0201956C.h"

struct UnkSrc_02019A60;

typedef struct UnkSrc_02019A60 {
    u8 filler0[5];
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u16 unkA : 15;
    u16 unkA_f : 1;
} UnkSrc_02019A60;

void sub_02019A60(UnkStruct_0201956C *frames, int a1, UnkSrc_02019A60 *src);

void sub_02019A60(UnkStruct_0201956C *frames, int a1, UnkSrc_02019A60 *src) {
    BGWinFrameEntry *entry = &frames->entries[a1];
    u16 *buffer = entry->screen;
    int tilesW = entry->width;
    u16 base = src->unkA;
    int palette = (src->unk9 & 0xf) << 12;
    u16 row, col;
    for (row = 0; row < src->unk8; row++) {
        if (src->unk6 + row >= frames->entries[a1].height) {
            break;
        }
        for (col = 0; col < src->unk7; col++) {
            if (src->unk5 + col >= frames->entries[a1].width) {
                break;
            }
            buffer[col + (src->unk5 + (src->unk6 + row) * tilesW)] = palette + (base + col);
        }
        base += src->unk7;
    }
}
