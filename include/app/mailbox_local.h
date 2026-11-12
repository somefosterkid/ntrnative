// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mailbox_local.h: the lines in use so
// far.
#ifndef BWPORT_APP_MAILBOX_LOCAL_H
#define BWPORT_APP_MAILBOX_LOCAL_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "pml/mail.h"
#include "system/app_taskmenu.h"
#include "system/pms_data.h"
#include "system/printsys.h"
#include "constants/text_banks.h"

// The heaps: the screen's state, and the data of the screen
#define HEAPID_MAILBOX_PROC 0x65

// Black 2's; twinned code confirms the values of MAILBOX_STATE_INIT, MAILBOX_STATE_EXIT, MAILBOX_STATE_WAIT_WIPE,
// MAILBOX_STATE_WAIT_MESSAGE, MAILBOX_STATE_WAIT_YES_NO, MAILBOX_STATE_WAIT_PALETTE_FX, MAILBOX_STATE_START_CHILD,
// MAILBOX_STATE_WAIT_CHILD, MAILBOX_STATE_WIPE_IN, MAILBOX_STATE_SELECT_MAIL, MAILBOX_STATE_CREATE_MENU,
// MAILBOX_STATE_WAIT_MENU, MAILBOX_STATE_READ_MAIL, MAILBOX_STATE_CLOSE_MAIL, MAILBOX_STATE_ASK_ERASE,
// MAILBOX_STATE_ERASE_NOTICE, MAILBOX_STATE_ASK_GIVE, MAILBOX_STATE_GIVE_FADE_IN, MAILBOX_STATE_GIVE_RESULT,
// MAILBOX_STATE_PRESS_RETURN, MAILBOX_STATE_PRESS_CLOSE, MAILBOX_STATE_BACK_TO_LIST and MAILBOX_STATE_DONE in Black
// The states
enum {
    MAILBOX_STATE_INIT,
    MAILBOX_STATE_EXIT,
    MAILBOX_STATE_WAIT_WIPE,
    MAILBOX_STATE_WAIT_MESSAGE,
    MAILBOX_STATE_WAIT_YES_NO,
    MAILBOX_STATE_WAIT_PALETTE_FX,
    MAILBOX_STATE_START_CHILD,
    MAILBOX_STATE_WAIT_CHILD,
    MAILBOX_STATE_WIPE_IN,
    MAILBOX_STATE_SELECT_MAIL,
    MAILBOX_STATE_WIPE_OUT,
    MAILBOX_STATE_CHANGE_PAGE,
    MAILBOX_STATE_OPEN_MENU,
    MAILBOX_STATE_CREATE_MENU,
    MAILBOX_STATE_WAIT_MENU,
    MAILBOX_STATE_READ_MAIL,
    MAILBOX_STATE_CLOSE_MAIL,
    MAILBOX_STATE_ASK_ERASE,
    MAILBOX_STATE_ERASE_NOTICE,
    MAILBOX_STATE_ASK_GIVE,
    MAILBOX_STATE_PARTY_RETURN,
    MAILBOX_STATE_GIVE_FADE_IN,
    MAILBOX_STATE_GIVE_RESULT,
    MAILBOX_STATE_MAIL_WRITTEN,
    MAILBOX_STATE_PRESS_RETURN,
    MAILBOX_STATE_PRESS_CLOSE,
    MAILBOX_STATE_BACK_TO_LIST,
    MAILBOX_STATE_DONE,
};

typedef struct MailBoxView MailBoxView;

// Black 2's, not checked against Black
struct MailBoxWork {
    // The state machine's work. nextState is where a wipe, a message or a child process goes on to
    MailData *mails;
    MailboxProcessData *param;
    MailBoxView *view;
    u32 unk0C;
    void *childParam;
    u32 childProc;
    Font *font;
    u16 page;
    u8 cursor;
    u8 mailIndex;
    u8 partyResult;
    u8 partyPos : 7;
    u8 bagFull : 1;
    u16 item;
    BOOL mailGiven;
    u32 nextState;
    TCB *vblankTask;
    GameProcManager *procManager;
};

// pokebw2's MailBox_Update
// mb_seq.c
BOOL func_ov136_021c68a0(MailBoxWork *work, u32 *state);

#define HEAPID_MAILBOX_VIEW 0x66

// Black's: the archive with the mailbox's graphics
#define ARCID_MAILBOX 0x9b

#define MAILBOX_PAGE_SIZE 10

// The windows of the screen's view
#define MAILBOX_WINDOW_COUNT 32

// The cell actors of the view
#define MAILBOX_ACTOR_COUNT 8

#define MAILBOX_ACTOR_PREV_PAGE 0
#define MAILBOX_ACTOR_NEXT_PAGE 1
#define MAILBOX_ACTOR_CURSOR 2

// Black 2's, not checked against Black
// A window, and whether a string is being printed into it
struct MailBoxPrintState {
    BmpWin *window;
    u8 printing;
};

// Black 2's, not checked against Black
// A palette change of an actor or of a part of a BG's screen, in steps
struct MailBoxPaletteFx {
    u8 onScreen : 1;
    u8 target : 7;
    u8 firstPalette : 4;
    u8 secondPalette : 4;
    u8 phase;
    u8 timer;
    s8 x;
    s8 y;
    u8 width;
    u8 height;
};

// Black 2's, not checked against Black
struct MailBoxView {
    BGWinFrame *frames;
    u8 tileA[0x20];
    u8 tileB[0x20];
    BmpWin *windows[MAILBOX_WINDOW_COUNT];
    MailBoxPrintState printStates[MAILBOX_WINDOW_COUNT];
    PrintQueue *printQueue;
    PrintStream *stream;
    TCBExManager *tcbManager;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    u8 unk1DC;
    u8 waitForInput;
    u32 yesNoIndex;
    u32 unk1E4;
    u32 unk1E8;
    ClActUnit *clActUnit;
    ClActor *actors[MAILBOX_ACTOR_COUNT];
    u32 chars[8];
    u32 palettes[8];
    u32 cellAnims[8];
    u32 unk270[8];
    ArcTool *iconArc;
    CursorMove *cursor;
    MailData *mails[MAIL_COUNT];
    u8 pageMails[MAIL_COUNT];
    u16 mailCount;
    u16 maxPage;
    MailBoxPaletteFx paletteFx;
    u32 wipeNextState;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    AppTaskMenuItem menuItems[4];
    AppTaskMenuItem yesNoItems[2];
    PMSDraw *pmsDraw;
    PMSData stampSentence;
    ClActUnit *pmsClActUnit;
    PrintQueue *pmsPrintQueue;
};

// pokebw2's MailBox_CreateOBJs
// mb_obj.c
void func_ov136_021c64a8(MailBoxWork *work);
// pokebw2's MailBox_FreeOBJs
void func_ov136_021c657c(MailBoxWork *work);
// pokebw2's MailBox_UpdateClActUnit
void func_ov136_021c6594(MailBoxView *view);
// pokebw2's MailBox_SetActorAnim
void func_ov136_021c66a0(MailBoxView *view, u32 index, u32 sequence);
// pokebw2's MailBox_SetActorVisible
void func_ov136_021c66b8(MailBoxView *view, u32 index, BOOL visible);
// pokebw2's MailBox_SetActorPos
void func_ov136_021c66cc(MailBoxView *view, u32 index, int x, int y);
// pokebw2's MailBox_SetActorBlend
void func_ov136_021c670c(MailBoxView *view, u32 index, BOOL blend);
// pokebw2's MailBox_ShowReadCursor
void func_ov136_021c6884(MailBoxView *view);

#define MAILBOX_WIN_MESSAGE 25
#define MAILBOX_WIN_TITLE 26
#define MAILBOX_WIN_SENTENCE_1 27
#define MAILBOX_WIN_STAMP 30
#define MAILBOX_WIN_AUTHOR 31

// The messages of the question that follows picking a mail's use
#define MAILBOX_MESSAGE_ERASE 0

#define MAILBOX_MESSAGE_ERASED 1
#define MAILBOX_MESSAGE_ASK_HOLD 2
#define MAILBOX_MESSAGE_HOLD_DONE 3
#define MAILBOX_MESSAGE_BAG_FULL 4

// pokebw2's MailBox_CreateWindows
// mb_bmp.c
void func_ov136_021c5e58(MailBoxWork *work);
// pokebw2's MailBox_FreeWindows
void func_ov136_021c5f44(MailBoxWork *work);
// pokebw2's MailBox_DrawMailFrame
void func_ov136_021c6028(MailBoxWork *work, u32 frame, u32 windowIndex, u32 mailIndex);
// pokebw2's MailBox_PrintTitle
void func_ov136_021c6170(MailBoxWork *work);
// pokebw2's MailBox_PrintWhatWouldYouLike
void func_ov136_021c6284(MailBoxWork *work);
// pokebw2's MailBox_ShowMessageWindow
void func_ov136_021c62bc(MailBoxWork *work);
// pokebw2's MailBox_HideMessageWindow
void func_ov136_021c62f4(MailBoxWork *work);
// pokebw2's MailBox_SetMessage
void func_ov136_021c630c(MailBoxWork *work, u32 message);
// pokebw2's MailBox_PrintMail
void func_ov136_021c6374(MailBoxWork *work);

// The frames of BGWinFrame, one per mail of the page and a last for the mail picked
#define MAILBOX_FRAME_SELECTED 10

#define MAILBOX_ACTOR_RETURN_BUTTON 3
#define MAILBOX_ACTOR_CLOSE_BUTTON 7

// Black 2's; twinned code confirms the values of MAILBOX_CHILD_PARTY_GIVE, MAILBOX_CHILD_PARTY_ASK,
// MAILBOX_CHILD_MAIL_SYS and MAILBOX_CHILD_PARTY_PICK in Black
// What the state machine runs as child processes
enum {
    MAILBOX_CHILD_PARTY_GIVE,
    MAILBOX_CHILD_PARTY_ASK,
    MAILBOX_CHILD_MAIL_SYS,
    MAILBOX_CHILD_PARTY_PICK,
    MAILBOX_CHILD_COUNT,
};

// Black 2's, not checked against Black
typedef u32 (*MailBoxStateFunc)(MailBoxWork *work);

// pokebw2's MailBox_LoadMails
void func_ov136_021c5b40(MailBoxWork *work);
// pokebw2's MailBox_InitBGs
void func_ov136_021c5518(MailBoxWork *work);
// pokebw2's MailBox_LoadSystemPalettes
void func_ov136_021c5828(MailBoxWork *work);
// pokebw2's MailBox_CreatePMSDraw
void func_ov136_021c5dd8(MailBoxWork *work);
// pokebw2's MailBox_CreateCursor
void func_ov136_021c7658(MailBoxWork *work);
// pokebw2's MailBox_UpdateMailList
void func_ov136_021c5c00(MailBoxWork *work);
// pokebw2's MailBox_CreateMenuResources
void func_ov136_021c59b8(MailBoxWork *work);
// pokebw2's MailBox_FreeMenuResources
void func_ov136_021c5aa8(MailBoxWork *work);
// pokebw2's MailBox_FreeMessages
void func_ov136_021c5954(MailBoxView *view);
// pokebw2's MailBox_FreeCursor
void func_ov136_021c76a8(MailBoxWork *work);
// pokebw2's MailBox_FreePMSDraw
void func_ov136_021c5e30(MailBoxWork *work);
// pokebw2's MailBox_FreeFrames
void func_ov136_021c5918(MailBoxWork *work);
// pokebw2's MailBox_FreeBGs
void func_ov136_021c5714(MailBoxWork *work);
// pokebw2's MailBox_FreeMails
void func_ov136_021c5be0(MailBoxWork *work);
// pokebw2's MailBox_UpdatePaletteFx
BOOL func_ov136_021c5c84(MailBoxWork *work);
// pokebw2's MailBox_IsCloseKeyPressed
BOOL func_ov136_021c7738(void);
// pokebw2's MailBox_ShowSelectedFrame
void func_ov136_021c5c64(MailBoxWork *work);
// pokebw2's MailBox_CreateMainMenu
void func_ov136_021c5aec(MailBoxWork *work);
// pokebw2's MailBox_FreeMenu
void func_ov136_021c5b30(MailBoxWork *work);
// pokebw2's MailBox_IsReturnPressed
BOOL func_ov136_021c770c(void);
// pokebw2's MailBox_UpdateCursor
void func_ov136_021c76d0(MailBoxWork *work, u32 pos);
// pokebw2's MailBox_CreateYesNoMenu
void func_ov136_021c5974(MailBoxWork *work);

// Black 2's, not checked against Black
// The message file of the mailbox's texts, which the battle server flow's second file holds
#define MAILBOX_MSG_FILE TEXT_BANK_BTL_SERVER_FLOW_2

#define MAILBOX_WIN_FIRST_MAIL 2
#define MAILBOX_WIN_SELECTED_MAIL 22

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MailBox_VRAMConfig
// mb_main.c
extern const BGSysVRAMConfig data_ov136_021c78e0;
// pokebw2's MailBox_Nop1
void func_ov136_021c61cc(MailBoxView *view);
// pokebw2's MailBox_Nop2
void func_ov136_021c61d0(MailBoxView *view);
// pokebw2's MailBox_SetVRAMBanks
void func_ov136_021c5508(void);
// pokebw2's MailBox_LoadBGGraphics
void func_ov136_021c574c(MailBoxWork *work);
// pokebw2's MailBox_CreateFrames
void func_ov136_021c587c(MailBoxWork *work);
// pokebw2's MailBox_LoadMessages
void func_ov136_021c5924(MailBoxView *view);
// pokebw2's MailBox_VBlank
void func_ov136_021c54fc(TCB *tcb, void *data);
// pokebw2's MailBox_GetTouch
// mb_ui.c
s32 func_ov136_021c7648(MailBoxWork *work);
// pokebw2's MailBox_LoadMailGraphics
void func_ov136_021c5d68(MailBoxWork *work);

#endif // BWPORT_APP_MAILBOX_LOCAL_H
