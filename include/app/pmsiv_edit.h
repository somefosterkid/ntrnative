// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pmsiv_edit.h: the lines in use so far.
#ifndef BWPORT_APP_PMSIV_EDIT_H
#define BWPORT_APP_PMSIV_EDIT_H

#include "types.h"
#include "gfl/touchpanel.h"
#include "struct_decls.h"

// pokebw2's PMSIVEdit_SetupGraphicDatas
void func_ov110_021bbd60(PMSIVEdit *wk, ArcTool *arc);
// pokebw2's PMSIVEdit_Delete
void func_ov110_021bbcf0(PMSIVEdit *wk);
// pokebw2's PMSIVEdit_UpdateEditArea
// Draws the sentence or the words again
void func_ov110_021bc23c(PMSIVEdit *wk);
// pokebw2's PMSIVEdit_MoveCursor
void func_ov110_021bc72c(PMSIVEdit *wk, u32 pos);
// pokebw2's PMSIVEdit_VisibleCursor
void func_ov110_021bc704(PMSIVEdit *wk, BOOL visible);
// pokebw2's PMSIVEdit_ScrollSet
// Scrolls the edit area up or back down on the lower screen
void func_ov110_021bbef4(PMSIVEdit *wk, BOOL up);
// pokebw2's PMSIVEdit_ScrollWait
BOOL func_ov110_021bbf54(PMSIVEdit *wk);
// pokebw2's PMSIVEdit_GetWordIndex
// Which of the sentence's words a word place of the area is
u16 func_ov110_021bc6e0(PMSIVEdit *wk, u32 index);
// pokebw2's PMSIVEdit_GetWordArea
// The touch rectangle of a word of the sentence
void func_ov110_021bc320(PMSIVEdit *wk, TouchRect *rect, u32 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSIVEdit_Create
PMSIVEdit *func_ov110_021bbc5c(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk);
// pokebw2's PMSIVEdit_StopArrow
void func_ov110_021bc724(PMSIVEdit *wk);
// pokebw2's PMSIVEdit_ActiveArrow
void func_ov110_021bc728(PMSIVEdit *wk);
// pokebw2's PMSIVEdit_StopCursor
void func_ov110_021bc6ec(PMSIVEdit *wk);
// pokebw2's PMSIVEdit_ActiveCursor
void func_ov110_021bc6f8(PMSIVEdit *wk);
// pokebw2's PMSIVEdit_GetWordCount
u32 func_ov110_021bc6dc(PMSIVEdit *wk);

#endif // BWPORT_APP_PMSIV_EDIT_H
