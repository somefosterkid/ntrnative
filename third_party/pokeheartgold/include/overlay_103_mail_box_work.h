#ifndef POKEHEARTGOLD_OVERLAY_103_MAIL_BOX_WORK_H
#define POKEHEARTGOLD_OVERLAY_103_MAIL_BOX_WORK_H

#include "global.h"

#include "mail.h"
#include "mail_misc.h"
#include "overlay_103.h"
#include "overlay_manager.h"
#include "player_data.h"

struct MailBoxView;

// MailBoxWork: the 0x2c object ov103_021EC940 allocates with OverlayManager_CreateAndGetData(HEAP_ID_156); DS offsets.
typedef struct MailBoxWork {
    Options *options;
    Mailbox *mailbox;
    UnkStruct_0203F074 *args;
    struct MailBoxView *view;
    OverlayManager *childMan;
    void *childParam;
    s32 childProc;
    u16 page;
    u8 cursor;
    u8 mailIndex;
    u8 partyResult;
    u8 partyPos : 7;
    u8 bagFull : 1;
    u16 item;
    BOOL mailGiven;
    u32 nextState;
} MailBoxWork;

// sizeof(MailBoxWork) == 0x2c

typedef u32 (*MailBoxStateFunc)(MailBoxWork *work);

typedef struct MailBoxChildProc {
    MailBoxStateFunc start;
    MailBoxStateFunc end;
    u32 nextState;
} MailBoxChildProc;

// ov103_021EEC68: the yes/no answer handlers, {yes, no} per view->yesNoIndex.
extern const MailBoxStateFunc ov103_021EEC68[][2];
extern const MailBoxChildProc ov103_021EEC78[];

#endif // POKEHEARTGOLD_OVERLAY_103_MAIL_BOX_WORK_H
