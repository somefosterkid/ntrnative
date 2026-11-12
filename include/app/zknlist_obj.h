// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknlist_obj.h: the lines in use so far.
#ifndef BWPORT_APP_ZKNLIST_OBJ_H
#define BWPORT_APP_ZKNLIST_OBJ_H

#include "types.h"
#include "app/zknlist_main.h"

// pokebw2's ZknListObj_PutIcon
// The icon of a species in a row at y of the touch screen's rows, or of the top screen's
void func_ov209_021be29c(ZknListWork *wk, u16 species, s16 y, BOOL touchScreen);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknListObj_GetPos
void func_ov209_021bd958(ZknListWork *wk, u32 id, s16 *x, s16 *y, u16 surface);
// pokebw2's ZknListObj_SetPos
void func_ov209_021bd934(ZknListWork *wk, u32 id, s16 x, s16 y, u16 surface);
// pokebw2's ZknListObj_Update
void func_ov209_021bd838(ZknListWork *wk);
// pokebw2's ZknListObj_Init
void func_ov209_021bd7f8(ZknListWork *wk);
// pokebw2's ZknListObj_Exit
void func_ov209_021bd824(ZknListWork *wk);
// pokebw2's ZknListObj_SetPageButtons
// Dims the page buttons at the list's ends; keep leaves one that is playing its press alone
void func_ov209_021be5d4(ZknListWork *wk, BOOL keep);
// pokebw2's ZknListObj_SetAnm
// Sets an actor's animation and plays it
void func_ov209_021bd890(ZknListWork *wk, u32 id, u32 anm);
// pokebw2's ZknListObj_IsAnmPlaying
BOOL func_ov209_021bd8c4(ZknListWork *wk, u32 id);
// pokebw2's ZknListObj_ScrollIcons
// Moves the icons with the list's rows
void func_ov209_021be34c(ZknListWork *wk, s8 delta);
// pokebw2's ZknListObj_SaveIcons
// Remembers the icons that are shown, and hides those when the list jumps
void func_ov209_021be43c(ZknListWork *wk);
// pokebw2's ZknListObj_HideSavedIcons
void func_ov209_021be4a8(ZknListWork *wk);
// pokebw2's ZknListObj_SelectIcon
// Animates the icon of a row of the touch screen, and stops the others
void func_ov209_021be51c(ZknListWork *wk, u32 row);
// pokebw2's ZknListObj_UpdateScrollBar
// Puts the scroll bar where the list's scroll is
void func_ov209_021be588(ZknListWork *wk);

// pokebw2's ZknListObj_PutMon: puts the icon of a species, or the empty icon for 0
void func_ov209_021bdd7c(ZknListWork *wk, u16 species);
#endif // BWPORT_APP_ZKNLIST_OBJ_H
