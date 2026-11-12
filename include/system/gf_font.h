// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/gf_font.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_GF_FONT_H
#define BWPORT_SYSTEM_GF_FONT_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Fonts and the colors their glyphs are drawn in (gf_font.c)

// loadType 0 reads each glyph from the file when it is drawn, and 1 loads them all; with fixedWidth every character
// advances by the default width
Font *GFL_FontCreate(u32 arcId, u32 fileId, u32 loadType, BOOL fixedWidth, HeapID heapId);
void GFL_FontFree(Font *font);

// Both call GFL_TextRndUpdateColorIndexLUT(1, 2, 0) (pokebw2's func_020232d0 and func_020232d8)
void func_0201dd60(void);
void func_0201dd68(void);
void GFL_TextRndUpdateColorIndexLUT(u8 letter, u8 shadow, u8 background);

// Draws a character's glyph into dest, a 2x2-tile bitmap's pixels
void GFL_FontGetGlyph(Font *font, u16 c, void *dest, GlyphInfo *info);
u32 GFL_FontGetCharWidth(Font *font, u16 c);
u8 GFL_FontGetCharHeight(Font *font);
// The letter, shadow and background color indices the glyphs are drawn with, and whether they differ from these
void GFL_TextRndGetGlobalColors(u8 *letter, u8 *shadow, u8 *background);
BOOL GFL_TextRndCheckColorIndexChange(u8 letter, u8 shadow, u8 background);

// A glyph's placement in its cell: the column it starts at, its width, how far it advances the pen, and its height
struct GlyphInfo {
    u8 x;
    u8 width;
    u8 advance;
    u8 height;
};

#endif // BWPORT_SYSTEM_GF_FONT_H
