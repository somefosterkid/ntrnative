#include "global.h"

#include "unk_0201956C.h"

void sub_020199F4(UnkStruct_0201956C *frames, int index, u8 x, u8 y, u8 width, u8 height, u8 palette);

void sub_020199F4(UnkStruct_0201956C *frames, int index, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    u16 *screen = frames->entries[index].screen;
    u32 frameWidth = frames->entries[index].width;
    u16 i, j;

    for (i = y; i < y + height; i++) {
        for (j = x; j < x + width; j++) {
            screen[i * frameWidth + j] = (screen[i * frameWidth + j] & 0xfff) | (palette << 12);
        }
    }
}
