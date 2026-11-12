// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pmsiv_wordwin.h.
#ifndef BWPORT_APP_PMSIV_WORDWIN_H
#define BWPORT_APP_PMSIV_WORDWIN_H

#include "types.h"
#include "gfl/clact.h"
#include "struct_decls.h"

// The phrase input's word window, the list of a category's words on BG2 with its cursor and scroll bar, of overlay
// 110's pmsiv_wordwin.c. The names are Black's dsd names; pokebw2's guesses are in the comments

// PMSIVWordWin_Create
PMSIVWordWin *func_ov110_021bf844(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk);
void func_ov110_021bf8bc(PMSIVWordWin *wk); // PMSIVWordWin_Delete
void func_ov110_021bf8f4(PMSIVWordWin *wk); // PMSIVWordWin_SetupGraphicDatas
// Draws the first words, or the words from a line on (PMSIVWordWin_SetupWords, PMSIVWordWin_RedrawWords)
void func_ov110_021bf9f0(PMSIVWordWin *wk);
void func_ov110_021bfa40(PMSIVWordWin *wk, u32 top);
// Shows the window over the categories, and hides it (PMSIVWordWin_StartFadeIn, PMSIVWordWin_WaitFadeIn,
// PMSIVWordWin_StartFadeOut, PMSIVWordWin_WaitFadeOut)
void func_ov110_021bfaa0(PMSIVWordWin *wk);
BOOL func_ov110_021bfb58(PMSIVWordWin *wk);
void func_ov110_021bfb7c(PMSIVWordWin *wk);
BOOL func_ov110_021bfba0(PMSIVWordWin *wk);
void func_ov110_021bfc20(PMSIVWordWin *wk, BOOL visible); // PMSIVWordWin_VisibleCursor
void func_ov110_021bfc64(PMSIVWordWin *wk, u32 pos); // PMSIVWordWin_MoveCursor
// Scrolls the words by lines (PMSIVWordWin_StartScroll, PMSIVWordWin_WaitScroll)
void func_ov110_021bfca4(PMSIVWordWin *wk, int vector);
BOOL func_ov110_021bfd60(PMSIVWordWin *wk);
// The scroll bar's place, and whether it is shown (PMSIVWordWin_GetScrollBarPos, PMSIVWordWin_SetScrollBar,
// PMSIVWordWin_SetScrollBarY)
BOOL func_ov110_021bfd94(PMSIVWordWin *wk, ClActorPos *pos);
void func_ov110_021bfdb0(PMSIVWordWin *wk, u32 top, u32 scrollMax);
void func_ov110_021bfe24(PMSIVWordWin *wk, u32 y);
// The line the scroll bar points at (PMSIVWordWin_GetScrollBarLine)
u32 func_ov110_021bfe60(PMSIVWordWin *wk, u32 scrollMax);
// Plays the cursor's animation of a word chosen (PMSIVWordWin_StartCursorDecide, PMSIVWordWin_WaitCursorDecide)
void func_ov110_021c00ac(PMSIVWordWin *wk, u32 pos);
BOOL func_ov110_021c00fc(PMSIVWordWin *wk);

#endif // BWPORT_APP_PMSIV_WORDWIN_H
