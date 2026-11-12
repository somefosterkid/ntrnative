// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gtsnego_local.h: the lines in use so
// far.
#ifndef BWPORT_APP_GTSNEGO_LOCAL_H
#define BWPORT_APP_GTSNEGO_LOCAL_H

#include "types.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "app/ui/touchbar.h"

// The rows of the list of people on the screen, and the rows kept, with the hidden ones above and below
#define GTSNEGO_LIST_SHOWN 3

#define GTSNEGO_LIST_ROWS 6
typedef struct GtsNegoDisp GtsNegoDisp;
typedef struct GtsNegoMessage GtsNegoMessage;

// pokebw2's GtsNego_GetFriend
// gtsnego_proc.c
// The person of the Pal Pad's list at index, or NULL past its ends
PlayerInfo *func_ov120_021d4f70(GameData *gameData, int index);
// pokebw2's GtsNegoMessage_Create
// gtsnego_message.c
GtsNegoMessage *func_ov120_021d7724(HeapID heapId, u32 bank);
// pokebw2's GtsNegoMessage_Update
void func_ov120_021d7858(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_Free
void func_ov120_021d7898(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_Print
// Print a message in the top screen's window, four lines or two, and whether it is printed
void func_ov120_021d79f4(GtsNegoMessage *msg, u32 msgNo);
// pokebw2's GtsNegoMessage_PrintShort
void func_ov120_021d7a74(GtsNegoMessage *msg, u32 msgNo);
// pokebw2's GtsNegoMessage_IsDone
BOOL func_ov120_021d7af4(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_ClearWindow
void func_ov120_021d7b38(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_CreateYesNo
// The yes/no menu, at one of two heights
AppTaskMenu *func_ov120_021d7b60(GtsNegoMessage *msg, u32 layout);
// pokebw2's GtsNegoMessage_FreeLabels
// The lower screen's labels
void func_ov120_021d7c1c(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_AddLabel
int func_ov120_021d7c48(GtsNegoMessage *msg, u32 msgNo, int x, int y, int width, int bg);
// pokebw2's GtsNegoMessage_AddMenuLabel
int func_ov120_021d7cd4(GtsNegoMessage *msg, u32 msgNo, int x, int y);
// pokebw2's GtsNegoMessage_SetLabel
void func_ov120_021d7ce8(GtsNegoMessage *msg, u32 msgNo, int label);
// pokebw2's GtsNegoMessage_QueueLabelsLoad
void func_ov120_021d7d30(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_ClearLabels
void func_ov120_021d7d3c(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_ShowMainMenu
// The labels and touch buttons of the main menu and of the search's settings
void func_ov120_021d7d6c(GtsNegoMessage *msg, ButtonManCallback callback, void *work);
// pokebw2's GtsNegoMessage_ShowSearchMenu
void func_ov120_021d7dc8(GtsNegoMessage *msg, ButtonManCallback callback, void *work, int level, int myKind,
                                   int wantKind);
// pokebw2's GtsNegoMessage_SetLevel
void func_ov120_021d7e88(GtsNegoMessage *msg, int level);
// pokebw2's GtsNegoMessage_SetMyKind
void func_ov120_021d7e9c(GtsNegoMessage *msg, int kind);
// pokebw2's GtsNegoMessage_SetWantKind
void func_ov120_021d7eb0(GtsNegoMessage *msg, int kind);
// pokebw2's GtsNegoMessage_CreateButton
// A button at the bottom of the lower screen
AppTaskMenuWin *func_ov120_021d7ecc(GtsNegoMessage *msg, u32 msgNo);
// pokebw2's GtsNegoMessage_PrintHeading
void func_ov120_021d7f5c(GtsNegoMessage *msg, u32 msgNo);
// pokebw2's GtsNegoMessage_PrintLocation
void func_ov120_021d7fd0(GtsNegoMessage *msg, PlayerInfo *partner);
// pokebw2's GtsNegoMessage_ClearLocation
void func_ov120_021d80d4(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_PrintFriend
// The names of the list of people to meet
int func_ov120_021d8118(GtsNegoMessage *msg, PlayerInfo *friend, int row);
// pokebw2's GtsNegoMessage_ShowFriendList
void func_ov120_021d81e0(GtsNegoMessage *msg, GameData *gameData);
// pokebw2's GtsNegoMessage_PrintLastFriend
void func_ov120_021d822c(GtsNegoMessage *msg, GameData *gameData, int index);
// pokebw2's GtsNegoMessage_ShiftFriendsUp
void func_ov120_021d824c(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_PrintFriends
void func_ov120_021d82a8(GtsNegoMessage *msg, GameData *gameData, int top);
// pokebw2's GtsNegoMessage_PrintFirstFriend
void func_ov120_021d82d8(GtsNegoMessage *msg, GameData *gameData, int index);
// pokebw2's GtsNegoMessage_ShiftFriendsDown
void func_ov120_021d82f8(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_CreateConfirm
// The menu that confirms the person to meet
AppTaskMenu *func_ov120_021d8358(GtsNegoMessage *msg, u32 layout);
// pokebw2's GtsNegoMessage_FreeMenu
void func_ov120_021d8418(AppTaskMenu *menu);
// pokebw2's GtsNegoMessage_CreatePMSDraw
// The partner's greeting
void func_ov120_021d8430(GtsNegoMessage *msg, GtsNegoDisp *disp);
// pokebw2's GtsNegoMessage_FreePMSDraw
void func_ov120_021d846c(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_PrintPhrase
void func_ov120_021d8484(GtsNegoMessage *msg, PMSData *phrase);
// pokebw2's GtsNegoMessage_PrintTitle
void func_ov120_021d8520(GtsNegoMessage *msg, u32 msgNo);
// pokebw2's GtsNegoMessage_CreateCancelButton
// The search's cancel button
void func_ov120_021d8590(GtsNegoMessage *msg, ButtonManCallback callback, void *work);
// pokebw2's GtsNegoMessage_PressCancelButton
void func_ov120_021d8638(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_FreeCancelButton
void func_ov120_021d8648(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_IsCancelPressDone
BOOL func_ov120_021d8664(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_PrintTradeCount
void func_ov120_021d8684(GtsNegoMessage *msg, u32 msgNo, int count);
// pokebw2's GtsNegoMessage_PrintPartnerName
void func_ov120_021d8784(GtsNegoMessage *msg, PlayerInfo *partner);
// pokebw2's GtsNegoMessage_ShowPartner
// The partner found, on the top screen: name, trades, place
void func_ov120_021d880c(GtsNegoMessage *msg, PlayerInfo *partner, int count);
// pokebw2's GtsNegoMessage_Reset
void func_ov120_021d883c(GtsNegoMessage *msg);

// Black 2's, not checked against Black
// The process's heap, which shares its number
#define HEAPID_GTSNEGO HEAPID_IRC_BATTLE_MENU

// Black 2's; twinned code confirms the values of GTSNEGO_CURSOR_LEVEL, GTSNEGO_CURSOR_MY_KIND,
// GTSNEGO_CURSOR_WANT_KIND, GTSNEGO_CURSOR_SEARCH, GTSNEGO_CURSOR_FRIEND_1, GTSNEGO_CURSOR_FRIEND_2,
// GTSNEGO_CURSOR_FRIEND_3 and GTSNEGO_CURSOR_NONE in Black
// Where the cursor of the lower screen is: the two buttons of the main menu, the three settings of the search and its
// button, the three rows of the list of people to meet, or nowhere
enum {
    GTSNEGO_CURSOR_ANYONE,
    GTSNEGO_CURSOR_RENDEZVOUS,
    GTSNEGO_CURSOR_LEVEL,
    GTSNEGO_CURSOR_MY_KIND,
    GTSNEGO_CURSOR_WANT_KIND,
    GTSNEGO_CURSOR_SEARCH,
    GTSNEGO_CURSOR_FRIEND_1,
    GTSNEGO_CURSOR_FRIEND_2,
    GTSNEGO_CURSOR_FRIEND_3,
    GTSNEGO_CURSOR_NONE,
};

// Black 2's, not checked against Black
// What the lower screen's palettes are dimmed for, leaving these lit: nothing; the yes/no menu over the list of
// people; the search's top screen; the yes/no menu over the main menu
enum {
    GTSNEGO_DIM_ALL,
    GTSNEGO_DIM_LIST_MENU,
    GTSNEGO_DIM_SEARCH,
    GTSNEGO_DIM_MAIN_MENU,
};

// What moving in the list of people did: nothing, moved the cursor, or scrolled the list
enum {
    GTSNEGO_SCROLL_NONE,
    GTSNEGO_SCROLL_CURSOR,
    GTSNEGO_SCROLL_LIST,
};

// The way the list of people scrolls
enum {
    GTSNEGO_LIST_STILL,
    GTSNEGO_LIST_DOWN,
    GTSNEGO_LIST_UP,
};

// The list of people to meet: how many there are, the row of the cursor and the person in the first row
struct GtsNegoScroll {
    int count;
    int cursor;
    int top;
};

// pokebw2's GtsNegoDisp_SetCursor
// Puts the cursor at a GTSNEGO_CURSOR_*, the search button being lit instead of it
void func_ov120_021d69f4(GtsNegoDisp *disp, AppTaskMenuWin *button, int cursor);
// pokebw2's GtsNegoDisp_PressArrow
void func_ov120_021d6b84(GtsNegoDisp *disp, int arrow);
// pokebw2's GtsNegoDisp_ShowPartner
// The top screen while it searches, and with the partner found
void func_ov120_021d74b0(GtsNegoDisp *disp, int avatar);
// pokebw2's GtsNegoDisp_ShowSearching
void func_ov120_021d7550(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_PressCursor
// Plays the cursor's animation of a press, and whether it has ended
BOOL func_ov120_021d6a9c(GtsNegoDisp *disp, int cursor);
// pokebw2's GtsNegoDisp_HideSearchMenu
void func_ov120_021d68c4(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_ShowSearchMenu
// The search's settings on the lower screen, with their arrows
void func_ov120_021d6814(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_Reset
// Back to the screens of the main menu
void func_ov120_021d761c(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_HideFriendList
void func_ov120_021d6d94(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_SetKnobAnimating
void func_ov120_021d6fd8(GtsNegoDisp *disp, BOOL animating);
// pokebw2's GtsNegoDisp_ScrollUp
int func_ov120_021d6e50(GtsNegoDisp *disp, GtsNegoScroll *scroll);
// pokebw2's GtsNegoDisp_StartListScroll
// Starts scrolling the list by a row (GTSNEGO_LIST_*), stops it, and scrolls it for a frame: the way it still
// scrolls, and in done the way it has just ended
void func_ov120_021d6ff8(GtsNegoDisp *disp, int dir);
// pokebw2's GtsNegoDisp_ScrollDown
// Move the cursor in the list, returning a GTSNEGO_SCROLL_*
int func_ov120_021d6e1c(GtsNegoDisp *disp, GtsNegoScroll *scroll);
// pokebw2's GtsNegoDisp_UpdateListScroll
int func_ov120_021d702c(GtsNegoDisp *disp, int *done);
// pokebw2's GtsNegoDisp_DrawListRows
void func_ov120_021d6d10(GtsNegoDisp *disp, GameData *gameData, int top);
// pokebw2's GtsNegoDisp_ShiftFriendIconsDown
void func_ov120_021d7260(GtsNegoDisp *disp, PlayerInfo *friend);
// pokebw2's GtsNegoDisp_ShiftFriendIconsUp
// Move the icons a row up or down, the new person's taking the free row
void func_ov120_021d7220(GtsNegoDisp *disp, PlayerInfo *friend);
// pokebw2's GtsNegoDisp_MoveFriendIcons
void func_ov120_021d70c8(GtsNegoDisp *disp, BOOL up);
// pokebw2's GtsNegoDisp_SetKnob
// The scroll bar's knob: puts it where a position of count is, and drags it to y, returning the position there
void func_ov120_021d733c(GtsNegoDisp *disp, int pos, int count);
// pokebw2's GtsNegoDisp_SetScrollCursor
void func_ov120_021d6e70(GtsNegoDisp *disp, GtsNegoScroll *scroll, u32 row);
// pokebw2's GtsNegoDisp_DragKnob
int func_ov120_021d7374(GtsNegoDisp *disp, int y, int count);
// pokebw2's GtsNegoDisp_SetFriendIcons
void func_ov120_021d72a0(GtsNegoDisp *disp, GameData *gameData, int top);
// pokebw2's GtsNegoDisp_LeaveFriendList
void func_ov120_021d6d48(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_ResetList
void func_ov120_021d70f0(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_IsCursorPressDone
BOOL func_ov120_021d6ac0(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_Main
void func_ov120_021d5f98(GtsNegoDisp *disp);

// pokebw2's GtsNegoDisp_CreateTouchBar
void func_ov120_021d692c(GtsNegoDisp *disp, u32 objPalette);
// pokebw2's GtsNegoDisp_FreeTouchBar
void func_ov120_021d698c(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_FreeArrows
void func_ov120_021d6b5c(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_FreeListScreen
void func_ov120_021d6c64(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_FreeListFooter
void func_ov120_021d6ed8(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_FreeListActors
void func_ov120_021d6fb4(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_StopListScroll
void func_ov120_021d7020(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_CreateFriendIcon
void func_ov120_021d7128(GtsNegoDisp *disp, int row, int avatar);
// pokebw2's GtsNegoDisp_ShowFriendIcon
void func_ov120_021d7190(GtsNegoDisp *disp, int row, int x, int y);
// pokebw2's GtsNegoDisp_SetFriendIcon
void func_ov120_021d71b8(GtsNegoDisp *disp, PlayerInfo *friend, int row);
// pokebw2's GtsNegoDisp_MoveFriendIcon
void func_ov120_021d71e8(GtsNegoDisp *disp, int row, int y);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GtsNegoDisp_GetClActUnit
ClActUnit *func_ov120_021d60fc(GtsNegoDisp *disp);
// pokebw2's GtsNegoMessage_UpdateButtons
void func_ov120_021d7c10(GtsNegoMessage *msg);
// pokebw2's GtsNegoMessage_HideMenu
void func_ov120_021d7ec4(GtsNegoMessage *msg);
// pokebw2's GtsNegoDisp_SetDim
// Dims the lower screen's palettes for a GTSNEGO_DIM_*, or brings them back
void func_ov120_021d63f4(GtsNegoDisp *disp, BOOL dim, u32 type);
// pokebw2's GtsNegoDisp_GetTouchBar
TouchBar *func_ov120_021d6988(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_ShowFriendList
// The list of people to meet, with a scroll bar when there are more than its rows
void func_ov120_021d6bb0(GtsNegoDisp *disp, BOOL scrollable);
// pokebw2's GtsNegoDisp_Create
// gtsnego_disp.c
GtsNegoDisp *func_ov120_021d5e88(HeapID heapId, GameData *gameData);
// pokebw2's GtsNegoDisp_Free
void func_ov120_021d5fc4(GtsNegoDisp *disp);
// pokebw2's GtsNegoDisp_Dummy
void func_ov120_021d6810(GtsNegoDisp *disp, BOOL list);

#endif // BWPORT_APP_GTSNEGO_LOCAL_H
