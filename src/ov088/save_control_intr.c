// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov162/save_control_intr.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/backup_system.h"
#include "gfl/heap.h"
#include "gfl/ui.h"
#include "save/save_control.h"
#include "save/save_control_intr.h"
#include "save/trainer_data_savedata.h"

// Saving a new game in the background. A soft reset cancels it, and it does nothing if save data is already present

enum {
    STATE_IDLE,
    STATE_SAVING,
    STATE_PAUSED,
    STATE_DONE,
};

struct SaveControlIntr {
    SaveControl *save;
    u8 state;
    // The state to go back to after a pause
    u8 resumeState;
    u8 pauseRequested;
    u8 disabled;
    // The save control's flags, cleared while saving and put back afterwards
    u32 flag4;
    u32 flag5;
};

static void func_ov088_021bd3b8(void *work);

// pokebw2's SaveControlIntr_Create
SaveControlIntr *func_ov088_021bd25c(HeapID heapId, SaveControl *save) {
    SaveControlIntr *intr = GFL_HeapAllocate(heapId, sizeof(SaveControlIntr), TRUE, "save_control_intr.c", 69);

    intr->save = save;
    if (SaveControl_IsDataAlreadyPresent(save) == TRUE) {
        intr->disabled = TRUE;
    } else {
        GCTX_HIDSetSoftResetCallback(func_ov088_021bd3b8, intr);
    }
    return intr;
}

// pokebw2's SaveControlIntr_Free
void func_ov088_021bd29c(SaveControlIntr *intr) {
    GCTX_HIDSetSoftResetCallback(NULL, NULL);
    GFL_HeapFree(intr);
}

void func_02007150(SaveControl *save);
void func_02008764(SaveControl *save);
u8 func_02007218(SaveControl *save);
void func_02007214(SaveControl *save, u32 arg1);
void func_0200721c(SaveControl *save, u32 arg1);
void func_ov088_021bd2b0(SaveControlIntr *intr) {
    if (intr->disabled == TRUE) {
        return;
    }
    func_02007150(intr->save);
    if (intr->disabled == FALSE) {
        func_02008764(intr->save);
        intr->flag4 = func_02007210(intr->save);
        intr->flag5 = func_02007218(intr->save);
        func_02007214(intr->save, 0);
        func_0200721c(intr->save, 0);
    }
    intr->state = STATE_SAVING;
}

u32 getLockIDStatus_inline_stub(void);
u32 func_02007168(SaveControl *save);

u32 func_ov088_021bd2f4(SaveControlIntr *intr) {
    u32 result;

    if (intr->disabled == TRUE) {
        return 2;
    }
    switch (intr->state) {
    case STATE_SAVING:
        if (intr->pauseRequested == TRUE && getLockIDStatus_inline_stub() == 0) {
            intr->resumeState = intr->state;
            intr->state = STATE_PAUSED;
            return 0;
        }
        result = func_02007168(intr->save);
        if (result - 2 <= 1) {
            intr->state = STATE_DONE;
            func_02007214(intr->save, intr->flag4);
            func_0200721c(intr->save, intr->flag5);
        }
        return result;
    case STATE_PAUSED:
        if (intr->pauseRequested == FALSE) {
            intr->state = intr->resumeState;
        }
        return 0;
    case STATE_DONE:
        return 2;
    }
    return 0;
}



// pokebw2's SaveControlIntr_Pause
void func_ov088_021bd368(SaveControlIntr *intr) {
    if (intr->state != STATE_DONE) {
        intr->pauseRequested = TRUE;
    }
}

// pokebw2's SaveControlIntr_Resume
void func_ov088_021bd374(SaveControlIntr *intr) {
    if (intr->state != STATE_DONE) {
        intr->pauseRequested = FALSE;
    }
}

// pokebw2's SaveControlIntr_IsPausedOrDone
BOOL func_ov088_021bd380(SaveControlIntr *intr) {
    if (intr->disabled == TRUE || intr->state == STATE_DONE) {
        return TRUE;
    }
    if (intr->state == STATE_PAUSED) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's SaveControlIntr_Nop
void func_ov088_021bd39c(SaveControlIntr *intr) {
}

// pokebw2's SaveControlIntr_IsDone
BOOL func_ov088_021bd3a0(SaveControlIntr *intr) {
    if (intr->disabled == TRUE) {
        return TRUE;
    }
    if (intr->state == STATE_DONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's SaveControlIntr_OnSoftReset
static void func_ov088_021bd3b8(void *work) {
    SaveControlIntr *intr = work;

    if (intr->disabled == FALSE && intr->state != STATE_DONE) {
        func_020071a8(intr->save);
        intr->disabled = TRUE;
    }
}
