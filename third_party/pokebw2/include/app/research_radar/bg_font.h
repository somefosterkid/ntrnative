#ifndef POKEBW2_APP_RESEARCH_RADAR_BG_FONT_H
#define POKEBW2_APP_RESEARCH_RADAR_BG_FONT_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "struct_decls.h"
#include "system/gf_font.h"

// A line of text in a window of a BG, which the Research Radar's screens use for their labels (bg_font.c)
// The names of these functions and types are ours

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

BGFont *BGFont_Create(const BGFontSetup *setup, Font *font, MsgData *msgData, HeapID heapId);
void BGFont_Delete(BGFont *bgFont);
void BGFont_PrintMsg(BGFont *bgFont, u32 strId);
void BGFont_PrintStr(BGFont *bgFont, const StrBuf *str);
void BGFont_SetVisible(BGFont *bgFont, BOOL visible);
void BGFont_SetPalette(BGFont *bgFont, u8 palette);

#endif // POKEBW2_APP_RESEARCH_RADAR_BG_FONT_H
