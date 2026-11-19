#include "global.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"
#include "unk_0201956C.h"

struct AppTaskMenu;

struct AppTaskMenuRes;

struct ArcTool;

struct BmpWin;

struct ClActUnit;

struct ClActor;

struct CursorMove;

struct GameData;

struct GameProcManager;

struct MailBoxPaletteFx;

struct MailBoxPrintState;

struct MailBoxView;

struct MailBoxWork;

struct MailData;

struct MailboxProcessData;

struct PMSData;

struct PMSDraw;

struct PrintQueue;

struct PrintStream;

struct StrBuf;

struct TCB;

struct TCBExManager;

struct WordSet;

typedef struct AppTaskMenu AppTaskMenu;

typedef struct AppTaskMenuRes AppTaskMenuRes;

typedef struct ArcTool ArcTool;

typedef struct BmpWin BmpWin;

typedef struct GameData GameData;

typedef struct GameProcManager GameProcManager;

typedef struct PMSData PMSData;

typedef struct PrintStream PrintStream;

typedef struct StrBuf StrBuf;

typedef struct TCBExManager TCBExManager;

typedef struct TCB TCB;

typedef struct WordSet WordSet;

typedef struct MailData MailData;

typedef struct CursorMove CursorMove;

typedef struct PMSDraw PMSDraw;

typedef struct MailboxProcessData MailboxProcessData;

typedef struct MailBoxPrintState MailBoxPrintState;

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

struct PMSData {

    u16 type;
    u16 id;
    u16 words[2];
};

struct MailData {
    u32 trainerId;
    u8 trainerGender;
    u8 region;
    u8 gameVersion;
    u8 unk07;
    u16 trainerName[8];
    u16 unk18[3];

    u16 unk1E;
    PMSData message[3];
};

typedef struct PrintQueue PrintQueue;

typedef struct {
    StrBuf *str;
    u16 color;
    u32 type;
} AppTaskMenuItem;

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

struct MailBoxPrintState {
    BmpWin *window;
    u8 printing;
};

void sub_02019B1C(UnkStruct_0201956C *frames, int index, s8 *x, s8 *y);

struct MailboxProcessData {
    GameData *gameData;
    u32 result;
};

u32 ov103_021EDA98(MailBoxWork *work, u32 frame, u32 nextState);

u32 ov103_021EDA98(MailBoxWork *work, u32 frame, u32 nextState) {
    MailBoxPaletteFx *fx;
    s8 x;
    s8 y;
    u16 width;
    u16 height;

    sub_02019B1C(work->view->frames, frame, &x, &y);
    sub_02019B44(work->view->frames, frame, &width, &height);
    fx = &work->view->paletteFx;
    fx->onScreen = 1;
    fx->target = 2;
    fx->firstPalette = 2;
    fx->secondPalette = 1;
    fx->phase = 0;
    fx->timer = 0;
    fx->x = x;
    fx->y = y;
    fx->width = width;
    fx->height = height;
    work->nextState = nextState;
    return MAILBOX_STATE_WAIT_PALETTE_FX;
}
