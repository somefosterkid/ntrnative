// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_proc_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's screens: a stack of procs, each started with the parameter that its before function fills
// from the screen it follows. The name is the ROM's string, from GFL_HeapAllocate's asserts

#include "types.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"

// The steps of func_ov186_021f9a80
enum {
    BR_PROC_SYS_SEQ_CHANGE,
    BR_PROC_SYS_SEQ_START,
    BR_PROC_SYS_SEQ_MAIN,
    BR_PROC_SYS_SEQ_END,
    BR_PROC_SYS_SEQ_EXIT,
};

// A screen of the stack
typedef struct {
    BOOL is_use;
    void *param;
    const BrProcData *cp_data;
    u32 procID;
} BrProcStack;

// Black 2's; twinned code confirms the offsets of procMgr, tbl, tbl_max, work, heapId, prev, now, next, stack,
// stack_num, recovery and isRecovery, and its size in Black
struct BrProcSys {
    GameProcManager *procMgr;
    const BrProcData *tbl;
    u16 tbl_max;
    void *work;
    HeapID heapId;
    BOOL isEnd;
    u32 seq;
    BrProcStack prev;
    BrProcStack now;
    BrProcStack next;
    u32 stack[BR_PROC_SYS_STACK_MAX];
    u32 stack_num;
    BrProcSysRecovery *recovery;
    // Set until the first screen of a restored stack starts
    BOOL isRecovery;
};

static void func_ov186_021f9c84(BrProcStack *p_stack, u32 procID, const BrProcData *cp_data);
static void func_ov186_021f9ca4(BrProcStack *p_stack);

BrProcSys *func_ov186_021f99cc(u16 procID, const BrProcData *tbl, u16 tbl_max, void *work, BrProcSysRecovery *recovery,
                          HeapID heapId) {
    BrProcSys *p_wk;
    u32 i = 0;

    p_wk = GFL_HeapAllocate(heapId, sizeof(BrProcSys), FALSE, "br_proc_sys.c", 119);
    MI_CpuFill8(p_wk, 0, sizeof(BrProcSys));
    p_wk->heapId = heapId;
    p_wk->tbl_max = tbl_max;
    p_wk->work = work;
    p_wk->tbl = tbl;
    p_wk->recovery = recovery;
    p_wk->procMgr = CreateGameProcManager(heapId);
    func_ov186_021f9ca4(&p_wk->now);
    func_ov186_021f9ca4(&p_wk->next);
    func_ov186_021f9ca4(&p_wk->prev);

    if (p_wk->recovery->stack_num != 0) {
        for (i = 0; i < p_wk->recovery->stack_num; i++) {
            func_ov186_021f9bf4(p_wk, p_wk->recovery->stack[i]);
        }
        MI_CpuFill8(p_wk->recovery, 0, sizeof(BrProcSysRecovery));
        p_wk->isRecovery = TRUE;
    } else {
        func_ov186_021f9bf4(p_wk, procID);
    }
    return p_wk;
}

// pokebw2's BrProcSys_Exit
void func_ov186_021f9a6c(BrProcSys *p_wk) {
    FreeGameProcManager(p_wk->procMgr);
    GFL_HeapFree(p_wk);
}

// pokebw2's BrProcSys_Main
void func_ov186_021f9a80(BrProcSys *p_wk) {
    BrProcStack *p_now = &p_wk->now;
    BrProcStack *p_prev = &p_wk->prev;
    BrProcStack *p_next = &p_wk->next;

    switch (p_wk->seq) {
    case BR_PROC_SYS_SEQ_CHANGE:
        *p_prev = *p_now;
        *p_now = *p_next;
        p_wk->seq = BR_PROC_SYS_SEQ_START;
        break;
    case BR_PROC_SYS_SEQ_START: {
        u32 preID;

        GFL_ASSERT(p_now->is_use);
        GFL_ASSERT(p_now->cp_data->before_func);
        p_now->param = GFL_HeapAllocate(p_wk->heapId, p_now->cp_data->paramSize, FALSE, "br_proc_sys.c", 210);
        MI_CpuFill8(p_now->param, 0, p_now->cp_data->paramSize);
        preID = BR_PROCID_RECOVERY;
        if (!p_wk->isRecovery) {
            preID = p_prev->procID;
        }
        p_now->cp_data->before_func(p_now->param, p_wk->work, p_prev->param, preID);
        p_wk->isRecovery = FALSE;
        if (p_prev->param != NULL) {
            GFL_HeapFree(p_prev->param);
        }
        QueueGameProc(p_wk->procMgr, p_now->cp_data->overlayId, p_now->cp_data->procFuncs, p_now->param);
        p_wk->seq = BR_PROC_SYS_SEQ_MAIN;
        break;
    }
    case BR_PROC_SYS_SEQ_MAIN:
        if (!GFL_ProcMgrUpdate(p_wk->procMgr)) {
            p_wk->seq = BR_PROC_SYS_SEQ_END;
        }
        break;
    case BR_PROC_SYS_SEQ_END:
        GFL_ASSERT(p_now->is_use);
        if (p_now->cp_data->after_func != NULL) {
            p_now->cp_data->after_func(p_now->param, p_wk->work);
        }
        if (p_wk->stack_num != 0) {
            p_wk->seq = BR_PROC_SYS_SEQ_CHANGE;
        } else {
            if (p_now->param != NULL) {
                GFL_HeapFree(p_now->param);
            }
            p_wk->seq = BR_PROC_SYS_SEQ_EXIT;
        }
        break;
    case BR_PROC_SYS_SEQ_EXIT:
        p_wk->isEnd = TRUE;
        break;
    }
}

// pokebw2's BrProcSys_IsEnd
BOOL func_ov186_021f9b98(BrProcSys *p_wk) {
    return p_wk->isEnd;
}

// pokebw2's BrProcSys_GetHeapID
HeapID func_ov186_021f9b9c(BrProcSys *p_wk) {
    return p_wk->heapId;
}

// pokebw2's BrProcSys_Pop
void func_ov186_021f9ba0(BrProcSys *p_wk) {
    BrProcStack stack;

    if (p_wk->stack_num != 0) {
        p_wk->stack_num--;
        if (p_wk->stack_num != 0) {
            u16 procID = *(p_wk->stack + p_wk->stack_num - 1);

            func_ov186_021f9c84(&stack, procID, &p_wk->tbl[procID]);
            p_wk->next = stack;
        } else {
            func_ov186_021f9ca4(&p_wk->next);
        }
    } else {
        func_ov186_021f9ca4(&p_wk->next);
    }
}

// pokebw2's BrProcSys_Push
void func_ov186_021f9bf4(BrProcSys *p_wk, u16 procID) {
    BrProcStack stack;
    const BrProcData *cp_data = &p_wk->tbl[procID];

    GFL_ASSERT(procID < p_wk->tbl_max);
    func_ov186_021f9c84(&stack, procID, cp_data);
    GFL_ASSERT(p_wk->stack_num < BR_PROC_SYS_STACK_MAX);
    p_wk->stack[p_wk->stack_num] = procID;
    p_wk->stack_num++;
    p_wk->next = stack;
}

// pokebw2's BrProcSys_Interrupt
void func_ov186_021f9c50(BrProcSys *p_wk) {
    MI_CpuCopy8(p_wk->stack, p_wk->recovery->stack, sizeof(p_wk->stack));
    p_wk->recovery->stack_num = p_wk->stack_num;
    func_ov186_021f9c6c(p_wk);
}

// pokebw2's BrProcSys_Abort
void func_ov186_021f9c6c(BrProcSys *p_wk) {
    while (p_wk->stack_num != 0) {
        func_ov186_021f9ba0(p_wk);
    }
}

// pokebw2's BrProcSys_SetStack
static void func_ov186_021f9c84(BrProcStack *p_stack, u32 procID, const BrProcData *cp_data) {
    MI_CpuFill8(p_stack, 0, sizeof(BrProcStack));
    p_stack->is_use = TRUE;
    p_stack->param = NULL;
    p_stack->procID = procID;
    p_stack->cp_data = cp_data;
}

// pokebw2's BrProcSys_ClearStack
static void func_ov186_021f9ca4(BrProcStack *p_stack) {
    MI_CpuFill8(p_stack, 0, sizeof(BrProcStack));
    p_stack->procID = BR_PROCID_NONE;
}
