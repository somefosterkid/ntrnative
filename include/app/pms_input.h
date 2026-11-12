// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pms_input.h, the lines in use so far.
#ifndef BWPORT_APP_PMS_INPUT_H
#define BWPORT_APP_PMS_INPUT_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The phrase input, overlay 110's pms_input.c: the work that the view and the screens' parts read. The names are
// Black's dsd names; pokebw2's guesses are in the comments

// Where the input is, keys or touch
int *func_ov110_021bf6d4(const PMSInputWork *wk); // PMSInput_GetKeyModePtr
// Whether the categories are listed by initial instead of by group
u32 func_ov110_021bf6dc(const PMSInputWork *wk); // PMSInput_GetCategoryMode
u32 func_ov110_021bf724(const PMSInputWork *wk); // PMSInput_GetCategoryCursorPos
// The words of the chosen group or of the search, and the two places of the word window's buttons
u32 func_ov110_021bf734(const PMSInputWork *wk); // PMSInput_GetCategoryWordMax
void func_ov110_021bf758(const PMSInputWork *wk, u32 index, StrBuf *buf); // PMSInput_GetCategoryWord
// The word window's first line and its last first line
void func_ov110_021bf7ec(const PMSInputWork *wk, u16 *top, u16 *scrollMax); // PMSInput_GetWordWinScroll

// pokebw2's PMSInput_Main
BOOL func_ov110_021bd534(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's PMSInput_GetEditWord
// The word at a place of the sentence, or the word being written
u16 func_ov110_021bf6f0(const PMSInputWork *wk, u32 index);
// pokebw2's PMSInput_GetEditAreaCursorPos
u32 func_ov110_021bf714(const PMSInputWork *wk);
// pokebw2's PMSInput_GetButtonCursorPos
u32 func_ov110_021bf71c(const PMSInputWork *wk);
// pokebw2's PMSInput_GetCategoryPosSaved
// Where the categories' cursor was before the erase button was pressed
u32 func_ov110_021bf72c(const PMSInputWork *wk);
// pokebw2's PMSInput_GetWordWinCursorPos
u32 func_ov110_021bf784(const PMSInputWork *wk);
// pokebw2's PMSInput_GetWordWinScrollVector
int func_ov110_021bf7a0(const PMSInputWork *wk);
// pokebw2's PMSInput_GetTCBManager
TCBManager *func_ov110_021bf7d0(const PMSInputWork *wk);
// pokebw2's PMSInput_GetSearchResultStr
void func_ov110_021bf814(const PMSInputWork *wk, u32 index, StrBuf *buf);
// pokebw2's PMSInput_ResetCategoryPos
// Puts the categories' cursor on the first category
void func_ov110_021bf82c(PMSInputWork *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSInput_Init
BOOL func_ov110_021bd484(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's PMSInput_Exit
BOOL func_ov110_021bd5a0(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's PMSInput_GetInputMode
u32 func_ov110_021bf6d8(const PMSInputWork *wk);
// pokebw2's PMSInput_GetSentenceType
u16 func_ov110_021bf6e4(const PMSInputWork *wk);
// pokebw2's PMSInput_GetEditSourceString
// The sentence's text, with its words as commands
StrBuf *func_ov110_021bf708(const PMSInputWork *wk, HeapID heapId);
// pokebw2's PMSInput_GetWordWinUpArrowVisible
BOOL func_ov110_021bf7ac(const PMSInputWork *wk);
// pokebw2's PMSInput_GetWordWinDownArrowVisible
BOOL func_ov110_021bf7b8(const PMSInputWork *wk);
// pokebw2's PMSInput_HasStartSentence
BOOL func_ov110_021bf7c4(const PMSInputWork *wk);
// pokebw2's PMSInput_GetSearchInputStr
// The search's letters and results, through pmsi_search.c
void func_ov110_021bf7d4(const PMSInputWork *wk, StrBuf *buf);
// pokebw2's PMSInput_ResetSearch
void func_ov110_021bf7e0(const PMSInputWork *wk);
// pokebw2's PMSInput_GetSearchResultCount
u32 func_ov110_021bf7fc(const PMSInputWork *wk);
// pokebw2's PMSInput_GetSearchInputLen
u32 func_ov110_021bf808(const PMSInputWork *wk);
// pokebw2's PMSInput_IsEditComplete
// Whether every word of the sentence is filled in
BOOL func_ov110_021bf824(const PMSInputWork *wk);


// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSInput_GetSearchInputStr
// The search's letters and results, through pmsi_search.c
void func_ov110_021bf7d4(const PMSInputWork *wk, StrBuf *buf);
#endif // BWPORT_APP_PMS_INPUT_H
