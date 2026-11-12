// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pmsiv_menu.h: the lines in use so far.
#ifndef BWPORT_APP_PMSIV_MENU_H
#define BWPORT_APP_PMSIV_MENU_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's PMSIVMenu_Delete
void func_ov110_021bca30(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_Main
// Runs every frame
void func_ov110_021bca84(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_SetupEditButtons
// The buttons of each part of the input
void func_ov110_021bcb50(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_UpdateSentenceType
void func_ov110_021bcca8(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_UpdateEditButtons
void func_ov110_021bcd44(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_SetCursor
void func_ov110_021bcf7c(PMSIVMenu *wk, u8 pos, BOOL active);
// pokebw2's PMSIVMenu_SetupCategoryButtons
void func_ov110_021bcc58(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_StartButton
void func_ov110_021bcee4(PMSIVMenu *wk, u32 button);
// pokebw2's PMSIVMenu_WaitButton
BOOL func_ov110_021bcf38(PMSIVMenu *wk, u32 button);
// pokebw2's PMSIVMenu_EndButton
void func_ov110_021bcf70(PMSIVMenu *wk, u32 button);
// pokebw2's PMSIVMenu_StartReturn
// Plays the return button's animation, if it is shown and still
BOOL func_ov110_021bd000(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_WaitReturn
BOOL func_ov110_021bd028(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_SetupWordWinButtons
void func_ov110_021bcc88(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_FlashButton
void func_ov110_021bcfbc(PMSIVMenu *wk, u8 pos, BOOL flash);
// pokebw2's PMSIVMenu_UpdateSearchButtons
void func_ov110_021bcd80(PMSIVMenu *wk);
// pokebw2's PMSIVMenu_IsFlashFinished
BOOL func_ov110_021bcff0(PMSIVMenu *wk, u8 pos);
// pokebw2's PMSIVMenu_SetEditButton
// Lights the edit area's OK or quit button
void func_ov110_021bcf18(PMSIVMenu *wk, u32 which);

// The buttons that PMSIVMenu_StartButton and PMSIVMenu_WaitButton press
enum {
    PMSIV_MENU_BUTTON_MODE,
    PMSIV_MENU_BUTTON_BACK,
    PMSIV_MENU_BUTTON_SEARCH,
    PMSIV_MENU_BUTTON_ERASE,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSIVMenu_Create
PMSIVMenu *func_ov110_021bc990(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk);

#endif // BWPORT_APP_PMSIV_MENU_H
