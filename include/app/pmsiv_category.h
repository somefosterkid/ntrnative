// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pmsiv_category.h: the lines in use so
// far.
#ifndef BWPORT_APP_PMSIV_CATEGORY_H
#define BWPORT_APP_PMSIV_CATEGORY_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's PMSIVCategory_SetupGraphicDatas
void func_ov110_021bb328(PMSIVCategory *wk, ArcTool *arc);
// pokebw2's PMSIVCategory_Delete
void func_ov110_021bb2cc(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_StartEnableBG
// Brightens the categories, and darkens them again
void func_ov110_021bb76c(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_PrintSearchInput
void func_ov110_021bb958(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_WaitEnableBG
BOOL func_ov110_021bb784(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_MoveCursor
void func_ov110_021bb6a0(PMSIVCategory *wk, u32 pos);
// pokebw2's PMSIVCategory_VisibleCursor
void func_ov110_021bb658(PMSIVCategory *wk, BOOL visible);
// pokebw2's PMSIVCategory_ChangeModeBG
// Shows the groups or the initials, by the input's category mode
void func_ov110_021bb8b8(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_StartResultList
// Scrolls the search's results in, printed, or out
void func_ov110_021bb9c0(PMSIVCategory *wk, BOOL show);
// pokebw2's PMSIVCategory_WaitResultList
BOOL func_ov110_021bbb0c(PMSIVCategory *wk, BOOL show);
// pokebw2's PMSIVCategory_StartDisableBG
void func_ov110_021bb7a0(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_WaitDisableBG
BOOL func_ov110_021bb7b8(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_WaitCursorDecide
BOOL func_ov110_021bbc10(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_StartFadeOut
// Fades the categories out and in, for the word window
void func_ov110_021bb838(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_WaitFadeOut
BOOL func_ov110_021bb854(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_StartFadeIn
void func_ov110_021bb870(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_SetDisableBG
void func_ov110_021bb804(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_StartBrightDown
void func_ov110_021bb814(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_WaitBrightDown
BOOL func_ov110_021bb82c(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_StartCursorDecide
// Plays the cursor's animation of a category chosen
void func_ov110_021bbb84(PMSIVCategory *wk, u32 pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSIVCategory_Create
PMSIVCategory *func_ov110_021bb290(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk);
// pokebw2's PMSIVCategory_ChangeModeScreen
void func_ov110_021bb8d8(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_WaitModeChange
BOOL func_ov110_021bb954(PMSIVCategory *wk);
// pokebw2's PMSIVCategory_WaitFadeIn
BOOL func_ov110_021bb8ac(PMSIVCategory *wk);

#endif // BWPORT_APP_PMSIV_CATEGORY_H

// The cursor's positions besides the categories: the three buttons, the last of which goes back
#define PMSIV_CATEGORY_POS_BUTTON_0 0xfc

#define PMSIV_CATEGORY_POS_BUTTON_1 0xfd
#define PMSIV_CATEGORY_POS_BACK 0xfe
