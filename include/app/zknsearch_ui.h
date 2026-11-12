// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknsearch_ui.h: the lines in use so far.
#ifndef BWPORT_APP_ZKNSEARCH_UI_H
#define BWPORT_APP_ZKNSEARCH_UI_H

#include "types.h"
#include "app/zknsearch_main.h"

// The positions of the main page's cursor, which ZknSearchUI_Main returns
enum {
    ZKNSEARCH_UI_ORDER,
    ZKNSEARCH_UI_INITIAL,
    ZKNSEARCH_UI_TYPE,
    ZKNSEARCH_UI_COLOR,
    ZKNSEARCH_UI_SHAPE,
    ZKNSEARCH_UI_START,
    ZKNSEARCH_UI_RESET,
    ZKNSEARCH_UI_SHORTCUT,
    ZKNSEARCH_UI_EXIT,
    ZKNSEARCH_UI_RETURN,
};

#define ZKNSEARCH_UI_LIST_CANCEL 8
#define ZKNSEARCH_UI_LIST_RESET 9

// The palettes of a position of the main page's cursor
#define ZKNSEARCH_UI_PALETTE_OFF 1

#define ZKNSEARCH_UI_PALETTE_ON 3

// pokebw2's ZknSearchUI_InitCursor
void func_ov210_021bfeac(ZknSearchWork *wk, u32 pos);
// pokebw2's ZknSearchUI_ExitCursor
void func_ov210_021bff18(ZknSearchWork *wk);
// pokebw2's ZknSearchUI_SetCursorPalette
void func_ov210_021bff30(ZknSearchWork *wk, u32 pos, u32 palette);
// pokebw2's ZknSearchUI_Main
// The main page: a position chosen, or a CURSOR_MOVE_* code
u32 func_ov210_021bff68(ZknSearchWork *wk);
// pokebw2's ZknSearchUI_WaitResult
// Whether the search's result has been read: a touch, A or B, or 90 frames
BOOL func_ov210_021c00fc(ZknSearchWork *wk);
// pokebw2's ZknSearchUI_ListMain
int func_ov210_021c014c(ZknSearchWork *wk);

// What ZknSearchUI_ListMain and ZknSearchUI_ShapeListMain return besides -1: the return button, a row from 1, the
// cancel of a key and the reset of the criterion
#define ZKNSEARCH_UI_LIST_RETURN 0

#define ZKNSEARCH_UI_LIST_ROW 1
#define ZKNSEARCH_UI_PALETTE_PRESSED 2

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknSearchUI_ShapeListMain
int func_ov210_021c01c0(ZknSearchWork *wk);

#endif // BWPORT_APP_ZKNSEARCH_UI_H
