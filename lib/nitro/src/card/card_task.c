// TwlSDK's CARD library, card_task.c: Black's main 0x02076170-0x020762f4. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "card_internal.h"

BOOL CARDi_ExecuteOldTypeTask(void (*task)(CARDiCommon *), BOOL async) {
    CARDiCommon *p = &cardi_common;

    if (async) {
        OS_SetThreadPriority(p->thread.context, p->priority);

        p->task_func = task;
        p->flag |= CARD_STAT_TASK;
        OS_WakeupThreadDirect(p->thread.context);
    } else {
        (*task)(p);
        CARDi_EndTask(p);
    }
    return async ? TRUE : (p->cmd->result == CARD_RESULT_SUCCESS);
}

void CARDi_OldTypeTaskThread(void *arg) {
    CARDiCommon *p = &cardi_common;

    for (;;) {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        for (;;) {
            if ((p->flag & CARD_STAT_TASK) != 0) {
                break;
            }
            OS_SleepThread(NULL);
        }
        OS_RestoreInterrupts(bak_psr);

        CARDi_ExecuteOldTypeTask(p->task_func, FALSE);
    }
}

BOOL CARDi_WaitForTask(CARDiCommon *p, BOOL restart, MIDmaCallback callback, void *callback_arg) {
    OSIntrMode bak_psr = OS_DisableInterrupts();

    while ((p->flag & CARD_STAT_BUSY) != 0) {
        OS_SleepThread(p->busy_q);
    }

    if (restart) {
        p->flag |= CARD_STAT_BUSY;
        p->callback = callback;
        p->callback_arg = callback_arg;
    }
    OS_RestoreInterrupts(bak_psr);
    return (p->cmd->result == CARD_RESULT_SUCCESS);
}

void CARDi_EndTask(CARDiCommon *p) {
    MIDmaCallback callback = p->callback;
    void *userdata = p->callback_arg;

    OSIntrMode bak_psr = OS_DisableInterrupts();

    p->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK | CARD_STAT_CANCEL);
    OS_WakeupThread(p->busy_q);
    OS_RestoreInterrupts(bak_psr);

    if (callback) {
        (*callback)(userdata);
    }
}
