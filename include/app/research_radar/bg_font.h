// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/bg_font.h: the lines in
// use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_BG_FONT_H
#define BWPORT_APP_RESEARCH_RADAR_BG_FONT_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// A window and where its text goes
typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    // Where the text starts in the window, in pixels
    u8 textX;
    u8 textY;
    u8 palette;
    u8 letterColor;
    u8 shadowColor;
    u8 backColor;
} BGFontWindow;

typedef struct {
    BGFontWindow window;
    // Whether the text is centered in the window, after textX
    BOOL centered;
} BGFontSetup;

// pokebw2's BGFont_Create
BGFont *func_ov217_021c0ef4(const BGFontSetup *setup, Font *font, MsgData *msgData, HeapID heapId);
// pokebw2's BGFont_PrintMsg
void func_ov217_021c0f5c(BGFont *bgFont, u32 strId);
// pokebw2's BGFont_SetVisible
void func_ov217_021c0ff8(BGFont *bgFont, BOOL visible);
// pokebw2's BGFont_SetPalette
void func_ov217_021c1038(BGFont *bgFont, u8 palette);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BGFont_Delete
void func_ov217_021c0f48(BGFont *bgFont);
// pokebw2's BGFont_PrintStr
void func_ov217_021c0f78(BGFont *bgFont, const StrBuf *str);

#endif // BWPORT_APP_RESEARCH_RADAR_BG_FONT_H
