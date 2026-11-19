#ifndef POKEHEARTGOLD_OVERLAY_74_MAIN_MENU_WINDOW_H
#define POKEHEARTGOLD_OVERLAY_74_MAIN_MENU_WINDOW_H

#include "font.h"

// MainMenuWindow: merged by tools/hg/promote.py from 11 m2c local copies (1 variants, base
// src/overlay_74_thumb_02228D20_02228D64.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct StringTemplate;
struct Window;

typedef struct MainMenuWindow {
    BOOL scrollable;
    BOOL drawFrame;
    BOOL clearBeforePrinting;
    BOOL textRightAligned;
    struct Window *window;
    struct StringTemplate *strTemplate;
    int width;
    int height;
    int textXOffset;
    int textYOffset;
    int baseTile;
    u32 bgLayer;
    int textPltt;
    int textBank;
    int frameBaseTile;
    int framePltt;
    enum Font font;
    u32 textColor;
    u8 backgroundColorIdx;
    u8 pad_49[0x3];
    int textEntryID;
    int renderDelay;
} MainMenuWindow;

// sizeof(MainMenuWindow) == 0x54

#endif // POKEHEARTGOLD_OVERLAY_74_MAIN_MENU_WINDOW_H
