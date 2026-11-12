// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/monolith/monolith_tool.h, the lines in
// use so far.
#ifndef BWPORT_APP_MONOLITH_MONOLITH_TOOL_H
#define BWPORT_APP_MONOLITH_MONOLITH_TOOL_H

#include "types.h"
#include "app/monolith/monolith_main.h"
#include "gfl/clact.h"
#include "struct_decls.h"

// The Entralink monolith's helpers that its screens share (monolith_tool.c, named by the ROM's string): text shown as
// actors on a frame, the panels' palette effects and the return button. pokebw2's guessed names are in the comments

// The widths of a text actor's frame
enum {
    MONOLITH_TEXT_WIDE,
    MONOLITH_TEXT_MEDIUM,
    MONOLITH_TEXT_NARROW,
};

// A panel's palette effect: none, a pulse that marks the cursor's panel, or a flash that marks a picked one
enum {
    PANEL_MODE_NONE,
    PANEL_MODE_PULSE,
    PANEL_MODE_FLASH,
};

// A line of text from the monolith's messages in a frame actor
typedef struct {
    ClActor *frame;
    BmpOamActor *text;
    GFLBitmap *bitmap;
    // Set until the text is printed and uploaded
    u8 printing;
} MonolithTextActor;

// A line of text from the monolith's messages as an actor, without a frame (Black's own)
typedef struct {
    GFLBitmap *bitmap;
    BmpOamActor *actor;
    // Set until the text is printed and uploaded
    u8 printing;
} MonolithOamText;

// The return button, which blinks when touched
typedef struct {
    ClActor *actor;
    u8 blinking;
    u8 timer;
    u8 lit;
} MonolithReturnButton;

// MonolithTool_CreateText
void func_ov071_021b6a0c(MonolithWork *wk, MonolithTextActor *text, int sub, int width, int x, int y, u32 msgId,
                         WordSet *wordSet);
// MonolithTool_DeleteText
void func_ov071_021b6b38(MonolithTextActor *text);
// MonolithTool_UpdateText: uploads the text once it is printed; returns whether it did now
BOOL func_ov071_021b6b70(MonolithWork *wk, MonolithTextActor *text);
// MonolithTool_SetTextCursor: marks the text actor at cursor of count, with a pulse of the panel
void func_ov071_021b6b98(MonolithScreenParam *param, MonolithTextActor *texts, int count, int cursor, int panel);
// MonolithTool_SetTextPicked: marks the text actor at cursor of count as picked, with a flash of the panel
void func_ov071_021b6bec(MonolithScreenParam *param, MonolithTextActor *texts, int count, int cursor, int panel);
// MonolithTool_InitPanels
void func_ov071_021b6c54(MonolithScreenParam *param);
// MonolithTool_GetPanelMode, a PANEL_MODE_*
u8 func_ov071_021b6c74(MonolithScreenParam *param, int panel);
// MonolithTool_UpdatePanel
void func_ov071_021b6c7c(MonolithScreenParam *param, int req);
// Prints a message of the monolith's centered on (x, y) of the top or bottom screen, in a text actor of width by
// height tiles
void func_ov071_021b6e1c(MonolithWork *wk, MonolithOamText *text, int sub, int x, int y, int width, int height,
                         u32 msgId, WordSet *wordSet);
void func_ov071_021b6eec(MonolithOamText *text);
// Uploads the text once it is printed; returns whether it did now
BOOL func_ov071_021b6f04(MonolithWork *wk, MonolithOamText *text);
// Black's function that gets the player's numbers, which nothing reads
void *func_ov071_021b6f2c(MonolithScreenParam *screen);
// Black's function that returns the pass power's numbers, its fields by offset (not named yet)
void *func_ov071_021b6f40(MonolithScreenParam *screen);
// Overlay 10's function that counts a number from the monolith's table, the pass power's numbers and a param's bytes
// MonolithTool_CreateReturnButton
void func_ov071_021b7070(MonolithWork *wk, MonolithReturnButton *button);
// MonolithTool_DeleteReturnButton
void func_ov071_021b70b8(MonolithReturnButton *button);
// MonolithTool_UpdateReturnButton
void func_ov071_021b70c4(MonolithReturnButton *button);
// MonolithTool_PressReturnButton
void func_ov071_021b7108(MonolithReturnButton *button);
// MonolithTool_IsReturnButtonBlinking
BOOL func_ov071_021b7114(MonolithReturnButton *button);

// pokebw2's MonolithTool_CreateMenuRes
AppTaskMenuRes *func_ov071_021b7174(MonolithWork *wk, u32 bg, HeapID heapId);
// pokebw2's MonolithTool_SetTextVisible
void func_ov071_021b6b58(MonolithTextActor *text, BOOL visible);
// pokebw2's MonolithTool_UpdateYesNoMenu
// Returns whether an item was picked, and sets yes to whether it was YES
BOOL func_ov071_021b7204(MonolithWork *wk, int unused, AppTaskMenu *menu, BOOL *yes);
// pokebw2's MonolithTool_SetPanelPulse
void func_ov071_021b6c30(MonolithScreenParam *param, BOOL pulse, int panel);
// pokebw2's MonolithTool_CreateActor
// An actor of the bottom screen, with the monolith's cells
ClActor *func_ov071_021b7118(MonolithWork *wk, s16 x, s16 y, u16 sequence);

// pokebw2's MonolithTool_FlashPanel
void func_ov071_021b6c48(MonolithScreenParam *param, int panel);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MonolithTool_DeleteActor
void func_ov071_021b7168(ClActor *actor);
// pokebw2's MonolithTool_UpdateActor
void func_ov071_021b7170(void *ptr);
// pokebw2's MonolithTool_FreeMenuRes
void func_ov071_021b718c(AppTaskMenuRes *res);
// pokebw2's MonolithTool_CreateYesNoMenu
AppTaskMenu *func_ov071_021b7194(MonolithWork *wk, AppTaskMenuRes *res, HeapID heapId);
// pokebw2's MonolithTool_FreeYesNoMenu
void func_ov071_021b71fc(AppTaskMenu *menu);

#endif // BWPORT_APP_MONOLITH_MONOLITH_TOOL_H
