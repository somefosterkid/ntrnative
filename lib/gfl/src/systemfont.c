// Black's code at main 0x020461f4-0x02046440. Placed by tools/gap_files.py from 6 name, 5 pokebw2.

#include "types.h"
#include "gfl/heap.h"
#include "gfl/systemfont.h"
#include "nitro/fs.h"
#include "nitro/os.h"

typedef struct SystemFontHeader {
    u32 glyphOffset;
    u32 widthOffset;
} SystemFontHeader;

typedef struct SystemFont {
    u8 *data;
    u32 fgMask;
    u32 bgMask;
    s32 isBuiltin;
} SystemFont;

static SystemFont *data_02146a58;

#include "systemfont_data.h"

#pragma thumb off
void GFL_SystemFontInit(const char *path) {
    FSFile sp4;
    SystemFont *font;
    u32 fileSize;

    if (data_02146a58 != NULL) {
        return;
    }
    font = GFL_HeapAllocate(0U, sizeof(SystemFont), 0, "systemfont.c", 0x44U);
    data_02146a58 = font;
    if (path != NULL) {
        FS_InitFile(&sp4);
        if (FS_OpenFile(&sp4, path) == 1) {
            fileSize = FS_GetLength(&sp4);
            data_02146a58->data = GFL_HeapAllocate(0U, fileSize, 0, "systemfont.c", 0x4DU);
            FS_ReadFile(&sp4, data_02146a58->data, (s32) fileSize);
            FS_CloseFile(&sp4);
        } else {
            OS_Terminate();
        }
        data_02146a58->isBuiltin = 0;
        return;
    }
    font->data = (u8 *)data_020a08d0;
    data_02146a58->isBuiltin = 1;
    GFL_SystemFontSetColorIndices(1U, 0xFU);
}
#pragma thumb on

#pragma thumb off
void GFL_SystemFontGetGlyph(int c, u8 *dest) {
    const SystemFontHeader *header = (const SystemFontHeader *)data_02146a58->data;
    const u8 *glyph = data_02146a58->data + header->glyphOffset + c * 0x20 / 4;
    u32 fgMask = data_02146a58->fgMask;
    u32 bgMask = data_02146a58->bgMask;
    u32 *row = (u32 *)dest;
    int i;

    for (i = 0; i < 8; i++) {
        u32 bits = GFL_SystemFontUnpackBits(*glyph++);

        *row++ = (fgMask & bits) | (bgMask & (bits ^ 0xffffffff));
    }
    dest += 0x20;
    *dest = (data_02146a58->data + ((const SystemFontHeader *)data_02146a58->data)->widthOffset)[c];
}
#pragma thumb on

#pragma thumb off
u32 GFL_SystemFontUnpackBits(u32 param_1) {
    s32 bit;
    u32 bits;

    bits = 0;
    bit = 0;
    do {
        if (param_1 & (1 << bit)) {
            bits |= 0xF0000000U >> (bit * 4);
        }
        bit += 1;
    } while (bit < 8);
    return bits;
}

u8 GFL_SystemFontGetHeight(void) {
    return data_02146a58->data[0xd];
}

#pragma thumb on

#pragma thumb off
u16 GFL_SystemFontGetTabSize(void) {
    return 7;
}

void GFL_SystemFontSetColorIndices(u32 fg, u32 bg) {
    data_02146a58->fgMask = (fg << 28) | (fg << 24) | (fg << 20) | (fg << 16) | (fg << 12) | (fg << 8) | (fg << 4) | fg;
    data_02146a58->bgMask = (bg << 28) | (bg << 24) | (bg << 20) | (bg << 16) | (bg << 12) | (bg << 8) | (bg << 4) | bg;
}

#pragma thumb on
