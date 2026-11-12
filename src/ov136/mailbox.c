// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mailbox.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mailbox.h"
#include "app/mailbox_local.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "pml/mail.h"
#include "nitro/mi.h"

// The mailbox screen's proc. The ROM doesn't name this file; the name is a guess, after the screen's subject

static BOOL func_ov136_021c5480(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov136_021c54c8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov136_021c54dc(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's MAILBOX_PROC_FUNCTIONS
// pokebw2's MailBox_Exit
// pokebw2's MailBox_Init
// pokebw2's MailBox_Main
const GameProcFunctions data_ov136_021c78a4 = { func_ov136_021c5480, func_ov136_021c54c8, func_ov136_021c54dc };

static BOOL func_ov136_021c5480(GameProc *proc, u32 *state, void *param, void *work) {
    MailBoxWork *mailbox;
    MailboxProcessData *data = param;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MAILBOX_PROC, 0x10000);
    mailbox = GFL_ProcInitSubsystem(proc, sizeof(MailBoxWork), HEAPID_MAILBOX_PROC);
    MIi_CpuClearFast(0, mailbox, sizeof(MailBoxWork));
    mailbox->param = data;
    mailbox->mails = func_02008fec(data->gameData);
    mailbox->nextState = MAILBOX_STATE_WIPE_IN;
    mailbox->vblankTask = NULL;
    mailbox->procManager = CreateGameProcManager(HEAPID_MAILBOX_PROC);
    return TRUE;
}

static BOOL func_ov136_021c54c8(GameProc *proc, u32 *state, void *param, void *work) {
    if (!func_ov136_021c68a0(work, state)) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov136_021c54dc(GameProc *proc, u32 *state, void *param, void *work) {
    MailBoxWork *mailbox = work;

    FreeGameProcManager(mailbox->procManager);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapStatusValidate(HEAPID_MAILBOX_PROC);
    GFL_HeapDelete(HEAPID_MAILBOX_PROC);
    return TRUE;
}
