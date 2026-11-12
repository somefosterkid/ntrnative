// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_util.h: the lines in use
// so far.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_UTIL_H
#define BWPORT_APP_WIFIBATTLEMATCH_UTIL_H

#include "types.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

typedef struct WifiBattleMatchTask WifiBattleMatchTask;
typedef struct WifiBattleMatchMsgBox WifiBattleMatchMsgBox;
typedef struct WifiBattleMatchMenu WifiBattleMatchMenu;

// Black 2's, not checked against Black
// A step of a task: seq is the step it is in, and work is the work that the task was created with
typedef void (*WifiBattleMatchTaskFunc)(WifiBattleMatchTask *task, u32 *seq, void *work);

// What WifiBattleMatchMenu is made from: a list of messages of msgData, with the cursor and its window
typedef struct {
    MsgData *msgData;
    Font *font;
    PrintQueue *printQueue;
    u32 msgIds[4];
    u32 count;
    u16 bg;
    u16 palette;
    u16 framePalette;
    u16 frameChar;
    // Whether B cancels the menu
    u32 canCancel;
    // What WifiBattleMatchMenu's update returns when B cancels it
    u16 cancelValue;
    u16 cursorRow;
} WifiBattleMatchMenuParam;

// pokebw2's WifiBattleMatchMsgBox_Free
void func_ov177_021e8628(WifiBattleMatchMsgBox *box);
// pokebw2's WifiBattleMatchMsgBox_Print
// Prints a message of msgData, with a WIFIBATTLEMATCH_MSG_* way of printing it
void func_ov177_021e8738(WifiBattleMatchMsgBox *box, MsgData *msgData, u32 msgId, u32 mode);
// pokebw2's WifiBattleMatchMenu_Update
// The item picked, or -1 while none is
s32 func_ov177_021e8a00(WifiBattleMatchMenu *menu);

// pokebw2's WifiBattleMatchMsgBox_Clear
// Clears the message
void func_ov177_021e8870(WifiBattleMatchMsgBox *box);
// Whether the message has been printed to its end
BOOL func_ov177_021e8844(WifiBattleMatchMsgBox *msgBox);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WifiBattleMatchTask_Run
// Runs the current step
void func_ov177_021e8a80(WifiBattleMatchTask *task);
// pokebw2's WifiBattleMatchTask_IsEnded
// Is TRUE once the task has ended
BOOL func_ov177_021e8a90(WifiBattleMatchTask *task);
// pokebw2's WifiBattleMatchMsgBox_Create
// The message box, at the bottom of BG bg. The frame is drawn with frameChar and framePalette
WifiBattleMatchMsgBox *func_ov177_021e8568(u32 bg, u32 palette, u16 framePalette, u16 frameChar,
                                           PrintQueue *printQueue, Font *font, HeapID heapId);
// pokebw2's WifiBattleMatchTask_SetFunc
// Makes func the step that runs next
void func_ov177_021e8aa0(WifiBattleMatchTask *task, WifiBattleMatchTaskFunc func);
// pokebw2's WifiBattleMatchMenu_Create
WifiBattleMatchMenu *func_ov177_021e88b0(const WifiBattleMatchMenuParam *param, u8 x, u8 y, u8 width, u8 height,
                                         HeapID heapId);
// pokebw2's WifiBattleMatchMenu_Free
void func_ov177_021e89d0(WifiBattleMatchMenu *menu);

// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_util.h: the lines in use
// so far.


// Black 2's, not checked against Black
// A step of a task: seq is the step it is in, and work is the work that the task was created with

// Black 2's, not checked against Black
// How the message box prints its message
#define WIFIBATTLEMATCH_MSG_QUEUE 0

#define WIFIBATTLEMATCH_MSG_STREAM_KEY 1
#define WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON 2

// Black 2's, not checked against Black
#define WIFIBATTLEMATCH_MSG_STREAM 3

// pokebw2's WifiBattleMatchMsgBox_Free
// pokebw2's WifiBattleMatchMsgBox_Print
// Prints a message of msgData, with a WIFIBATTLEMATCH_MSG_* way of printing it
// pokebw2's WifiBattleMatchMsgBox_Clear
// Clears the message
// pokebw2's WifiBattleMatchMenu_CreateBottomRight
// The list menu. WifiBattleMatchMenu_CreateBottomRight puts it at the bottom right
WifiBattleMatchMenu *func_ov177_021e888c(const WifiBattleMatchMenuParam *param, HeapID heapId);
// pokebw2's WifiBattleMatchMenu_Update
// The item picked, or -1 while none is
// pokebw2's WifiBattleMatchTask_ReturnToSeq
void func_ov177_021e8ab8(WifiBattleMatchTask *task);
// Count a match in the records. mode is the kind of match
void func_ov177_021e8ac0(GameData *gameData, u32 mode);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WifiBattleMatchMsgBox_Create
// The message box, at the bottom of BG bg. The frame is drawn with frameChar and framePalette
WifiBattleMatchMsgBox *func_ov177_021e8568(u32 bg, u32 palette, u16 framePalette, u16 frameChar,
                                           PrintQueue *printQueue, Font *font, HeapID heapId);
// pokebw2's WifiBattleMatchMsgBox_Update
void func_ov177_021e867c(WifiBattleMatchMsgBox *box);
// pokebw2's WifiBattleMatchMenu_Create
WifiBattleMatchMenu *func_ov177_021e88b0(const WifiBattleMatchMenuParam *param, u8 x, u8 y, u8 width, u8 height,
                                         HeapID heapId);
// pokebw2's WifiBattleMatchMenu_Free
// pokebw2's WifiBattleMatchTask_Create
// Tasks
WifiBattleMatchTask *func_ov177_021e8a3c(void *work, WifiBattleMatchTaskFunc func, HeapID heapId);
// pokebw2's WifiBattleMatchTask_Free
void func_ov177_021e8a78(WifiBattleMatchTask *task);
// pokebw2's WifiBattleMatchTask_Run
// Runs the current step
// pokebw2's WifiBattleMatchTask_IsEnded
// Is TRUE once the task has ended
// pokebw2's WifiBattleMatchTask_SetFunc
// Makes func the step that runs next
// pokebw2's WifiBattleMatchTask_End
// Ends the task
void func_ov177_021e8aa8(WifiBattleMatchTask *task);
// pokebw2's WifiBattleMatchTask_SetReturnSeq
// Remembers the step to go back to, which WifiBattleMatchTask_ReturnToSeq goes to
void func_ov177_021e8ab4(WifiBattleMatchTask *task, u32 seq);

#endif // BWPORT_APP_WIFIBATTLEMATCH_UTIL_H
