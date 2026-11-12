// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknsearch_obj.h: the lines in use so
// far.
#ifndef BWPORT_APP_ZKNSEARCH_OBJ_H
#define BWPORT_APP_ZKNSEARCH_OBJ_H

#include "types.h"
#include "app/zknsearch_main.h"

// pokebw2's ZknSearchObj_Init
void func_ov210_021bed2c(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_Update
void func_ov210_021bed68(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_SetAnmSeq
void func_ov210_021bed98(ZknSearchWork *wk, u32 id, u32 anm);
// pokebw2's ZknSearchObj_SetAnm
// Sets an actor's animation and plays it
void func_ov210_021bedb8(ZknSearchWork *wk, u32 id, u32 anm);
// pokebw2's ZknSearchObj_GetAnm
u32 func_ov210_021bedd0(ZknSearchWork *wk, u32 id);
// pokebw2's ZknSearchObj_SetVisible
void func_ov210_021bedf0(ZknSearchWork *wk, u32 id, BOOL visible);
// pokebw2's ZknSearchObj_SetBlend
void func_ov210_021bee14(ZknSearchWork *wk, u32 id, BOOL blend);
// pokebw2's ZknSearchObj_SetPos
void func_ov210_021bee38(ZknSearchWork *wk, u32 id, s16 x, s16 y, u16 surface);
// pokebw2's ZknSearchObj_GetPos
void func_ov210_021bee58(ZknSearchWork *wk, u32 id, s16 *x, s16 *y, u16 surface);
// pokebw2's ZknSearchObj_SetBGPriority
void func_ov210_021bee80(ZknSearchWork *wk, u32 id, u32 priority);
// pokebw2's ZknSearchObj_HideList
// Hides the actors of the lists
void func_ov210_021bf38c(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_SetMain
// Show the actors of each page
void func_ov210_021bf418(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_SetOrder
void func_ov210_021bf480(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_SetInitial
void func_ov210_021bf4a4(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_SetShape
void func_ov210_021bf5b8(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_PutShape
// The chosen shape on the top screen
void func_ov210_021bf618(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_PutShapeIcons
// The shapes of the rows on both screens, from the list's scroll
void func_ov210_021bf658(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_UpdateScrollBar
// Puts the scroll bar where the list's scroll is
void func_ov210_021bf6e0(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_SetPageButtons
// Dims the page buttons at the list's ends; keep leaves one that is playing its press alone
void func_ov210_021bf728(ZknSearchWork *wk, BOOL keep);
// pokebw2's ZknSearchObj_PutMark
// A mark at y of the touch screen's rows, or of the top screen's
void func_ov210_021bf7b0(ZknSearchWork *wk, int index, s16 y, BOOL touchScreen);
// pokebw2's ZknSearchObj_HideMarks
void func_ov210_021bf81c(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_PutRowMark
// The mark of a row of the touch screen, or none
void func_ov210_021bf85c(ZknSearchWork *wk, u16 row, BOOL visible);
// pokebw2's ZknSearchObj_PutTypeMarks
void func_ov210_021bf89c(ZknSearchWork *wk, u8 type1, u8 type2);
// pokebw2's ZknSearchObj_PutShapeMark
void func_ov210_021bfa14(ZknSearchWork *wk, s16 y, BOOL touchScreen);
// pokebw2's ZknSearchObj_PutShapeRowMark
void func_ov210_021bfa7c(ZknSearchWork *wk, u16 row, BOOL visible);
// pokebw2's ZknSearchObj_PutItemMark
// The mark of a list's item, if its row is on either screen
void func_ov210_021bfabc(ZknSearchWork *wk, u32 index, u16 item);
// pokebw2's ZknSearchObj_PutShapeItemMark
void func_ov210_021bfb24(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_PutShapeIcon
void func_ov210_021bfb8c(ZknSearchWork *wk, u16 shape, s16 y, BOOL touchScreen);
// pokebw2's ZknSearchObj_ScrollMarks
// Move the marks and the shapes with the list's rows
void func_ov210_021bfbf8(ZknSearchWork *wk, s8 delta);
// pokebw2's ZknSearchObj_ScrollShapeIcons
void func_ov210_021bfce4(ZknSearchWork *wk, s8 delta);
// pokebw2's ZknSearchObj_MoveLoadingBar
// The search's progress bar after count frames
void func_ov210_021bfe84(ZknSearchWork *wk, u32 count);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknSearchObj_Exit
void func_ov210_021bed54(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_IsAnmPlaying
BOOL func_ov210_021bede0(ZknSearchWork *wk, u32 id);
// pokebw2's ZknSearchObj_IsVisible
BOOL func_ov210_021bee04(ZknSearchWork *wk, u32 id);
// pokebw2's ZknSearchObj_SetType
void func_ov210_021bf500(ZknSearchWork *wk);
// pokebw2's ZknSearchObj_SetColor
void func_ov210_021bf55c(ZknSearchWork *wk);

#endif // BWPORT_APP_ZKNSEARCH_OBJ_H
