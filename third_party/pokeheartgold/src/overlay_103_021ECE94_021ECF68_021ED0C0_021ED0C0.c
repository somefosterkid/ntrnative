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

struct MailBoxPrintState {
    BmpWin *window;
    u8 printing;
};

void ov103_021EE468(MailBoxWork *work, u32 frame, u32 windowIndex, u32 mailIndex);

void sub_020197F4(UnkStruct_0201956C *frames, int index);

void ov103_021ED0C0(MailBoxWork *work);

void ov103_021ED0C0(MailBoxWork *work) {
    u32 first = (u16)(10 * work->page);
    u8 *mails = &work->view->pageMails[first];
    u16 window = first;
    u16 frame = 0;
    u16 i;

    for (i = 0; i < 10; i++) {
        if (mails[i] == 0xff) {
            sub_0201980C(work->view->frames, frame);
        } else {
            ov103_021EE468(work, frame, window, mails[i]);
            sub_020197F4(work->view->frames, frame);
        }
        window++;
        frame++;
    }
}
