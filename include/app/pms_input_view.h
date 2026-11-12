// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pms_input_view.h, the lines in use so
// far.
#ifndef BWPORT_APP_PMS_INPUT_VIEW_H
#define BWPORT_APP_PMS_INPUT_VIEW_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/tcb.h"
#include "struct_decls.h"
#include "gfl/touchpanel.h"
#include "system/printsys.h"

// The phrase input's screens, overlay 110's pms_input_view.c: the commands that the input sends them, run as tasks,
// and the graphics its parts share. The names are Black's dsd names; pokebw2's guesses are in the comments

// The resources of a set of OBJ graphics, loaded for each screen
typedef struct {
    u32 palette;
    u32 chars;
    u32 cellAnims;
} PMSIVObjRes;

Font *func_ov110_021bac44(PMSInputView *vwk); // PMSIView_GetFont
void func_ov110_021bac50(PMSInputView *vwk, PMSIVObjRes *res, u32 lcd); // PMSIView_GetObjRes2
// The resources of a screen's OBJ graphics. bgPriority is not used
void func_ov110_021bac64(PMSInputView *vwk, PMSIVObjRes *res, u32 lcd, u32 bgPriority); // PMSIView_GetObjRes
// PMSIView_AddActor
ClActor *func_ov110_021bac78(PMSInputView *vwk, const PMSIVObjRes *res, u32 x, u32 y, u32 priority, int drawArea);
// Loads the lower screen's background, the one with the category buttons or the plain one (PMSIView_SetLowerScreen)
void func_ov110_021bad2c(PMSInputView *vwk, BOOL categories);
TCB *func_ov110_021b9ba4(TCBFunc func, void *wk, u32 priority); // PMSIView_AddVTask

// Black 2's; twinned code confirms the values of PMSIV_CMD_QUIT, PMSIV_CMD_FADEIN, PMSIV_CMD_UPDATE_EDITAREA,
// PMSIV_CMD_KTCHANGE_EDITAREA, PMSIV_CMD_EDITAREA_TO_BUTTON, PMSIV_CMD_BUTTON_TO_EDITAREA,
// PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT, PMSIV_CMD_EDITAREA_TO_CATEGORY, PMSIV_CMD_CATEGORY_TO_EDITAREA,
// PMSIV_CMD_CATEGORY_TO_WORDWIN, PMSIV_CMD_WORDWIN_TO_CATEGORY, PMSIV_CMD_WORDWIN_TO_EDITAREA,
// PMSIV_CMD_MOVE_EDITAREA_CURSOR, PMSIV_CMD_MOVE_BUTTON_CURSOR, PMSIV_CMD_MOVE_CATEGORY_CURSOR,
// PMSIV_CMD_MOVE_WORDWIN_CURSOR, PMSIV_CMD_SCROLL_WORDWIN, PMSIV_CMD_PUSH_BUTTON, PMSIV_CMD_UPDATE_EDITAREA_CURSOR,
// PMSIV_CMD_CHANGE_CATEGORY_MODE_ENABLE, PMSIV_CMD_CHANGE_CATEGORY_MODE, PMSIV_CMD_SET_WORDWIN_ARROWS,
// PMSIV_CMD_SHOW_MENU, PMSIV_CMD_MOVE_CATEGORY, PMSIV_CMD_MOVE_WORDWIN and PMSIV_CMD_COUNT in Black
// The commands
enum {
    PMSIV_CMD_INIT,
    PMSIV_CMD_QUIT,
    PMSIV_CMD_FADEIN,
    PMSIV_CMD_UPDATE_EDITAREA,
    PMSIV_CMD_KTCHANGE_EDITAREA,
    PMSIV_CMD_KTCHANGE_CATEGORY,
    PMSIV_CMD_KTCHANGE_WORDWIN,
    PMSIV_CMD_EDITAREA_TO_BUTTON,
    PMSIV_CMD_BUTTON_TO_EDITAREA,
    PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT,
    PMSIV_CMD_EDITAREA_TO_CATEGORY,
    PMSIV_CMD_CATEGORY_TO_EDITAREA,
    PMSIV_CMD_CATEGORY_TO_WORDWIN,
    PMSIV_CMD_WORDWIN_TO_CATEGORY,
    PMSIV_CMD_WORDWIN_TO_EDITAREA,
    PMSIV_CMD_WORDWIN_TO_BUTTON,
    PMSIV_CMD_MOVE_EDITAREA_CURSOR,
    PMSIV_CMD_MOVE_BUTTON_CURSOR,
    PMSIV_CMD_MOVE_CATEGORY_CURSOR,
    PMSIV_CMD_MOVE_WORDWIN_CURSOR,
    PMSIV_CMD_SCROLL_WORDWIN,
    PMSIV_CMD_PUSH_BUTTON,
    PMSIV_CMD_NOP_22,
    PMSIV_CMD_UPDATE_EDITAREA_CURSOR,
    PMSIV_CMD_CHANGE_CATEGORY_MODE_DISABLE,
    PMSIV_CMD_CHANGE_CATEGORY_MODE_ENABLE,
    PMSIV_CMD_NOP_26,
    PMSIV_CMD_NOP_27,
    PMSIV_CMD_NOP_28,
    PMSIV_CMD_NOP_29,
    PMSIV_CMD_CHANGE_CATEGORY_MODE,
    PMSIV_CMD_SET_WORDWIN_ARROWS,
    PMSIV_CMD_SHOW_MENU,
    PMSIV_CMD_MOVE_CATEGORY,
    PMSIV_CMD_MOVE_WORDWIN,
    PMSIV_CMD_COUNT,
};

// pokebw2's PMSIView_SetCommand
void func_ov110_021b9bec(PMSInputView *vwk, int cmd);
// pokebw2's PMSIView_WaitCommandAll
BOOL func_ov110_021b9c74(PMSInputView *vwk);
// pokebw2's PMSIView_GetSentenceEditPosMax
u32 func_ov110_021babe8(PMSInputView *vwk);
// pokebw2's PMSIView_Delete
void func_ov110_021b9b20(PMSInputView *vwk);
// pokebw2's PMSIView_ChangeKTEditArea
void func_ov110_021bae24(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk);
// pokebw2's PMSIView_ChangeKTCategory
void func_ov110_021bae94(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk);
// pokebw2's PMSIView_ChangeKTWordWin
void func_ov110_021baed0(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk);
// pokebw2's PMSIView_SetMenuButton
void func_ov110_021bac30(PMSInputView *vwk, u32 which);
// pokebw2's PMSIView_WaitCommand
BOOL func_ov110_021b9c90(PMSInputView *vwk, int cmd);
// pokebw2's PMSIView_GetWordWinScrollDir
int func_ov110_021bacdc(PMSInputView *vwk, u32 unused, u32 pos);
// pokebw2's PMSIView_SetWordWinScrollBarY
void func_ov110_021bad14(PMSInputView *vwk, u32 y);
// pokebw2's PMSIView_GetMenuButton
int func_ov110_021bac0c(PMSInputView *vwk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSIView_Create
PMSInputView *func_ov110_021b9a50(const PMSInputWork *mwk, const PMSInputData *dwk);
// pokebw2's PMSIView_GetSentenceWordArea
void func_ov110_021bac00(PMSInputView *vwk, TouchRect *rect, u8 index);
// pokebw2's PMSIView_GetWordWinScrollBarPos
u32 func_ov110_021bad20(PMSInputView *vwk, u32 count);
// pokebw2's PMSIView_GetSentenceWord
u16 func_ov110_021babf4(PMSInputView *vwk, u32 index);
// pokebw2's PMSIView_GetMenu
PMSIVMenu *func_ov110_021bac3c(PMSInputView *vwk);
// pokebw2's PMSIView_GetActUnit
ClActUnit *func_ov110_021bac40(PMSInputView *vwk);
// pokebw2's PMSIView_GetPrintQueue
PrintQueue *func_ov110_021bac4c(PMSInputView *vwk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSIView_GetMenu
PMSIVMenu *func_ov110_021bac3c(PMSInputView *vwk);
#endif // BWPORT_APP_PMS_INPUT_VIEW_H
