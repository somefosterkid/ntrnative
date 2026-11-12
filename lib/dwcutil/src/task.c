// libdwcutil's task.c (Black's ov016 0x021d7484-0x021d77ac): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct TaskCb TaskCb;

struct TaskCb {
    TaskCb *prev;
    TaskCb *next;
    void (*func)(void *, void *);
    void *work;
    u8 prio;
    u8 alloc;
};

typedef struct TaskKind {
    u16 *freeQueue;
    u16 *delQueue;
    u32 list;
    TaskCb head;
    TaskCb tail;
    s32 num;
    u8 active;
    void *buf;
} TaskKind;

TaskCb *func_ov016_021d7694(s32 kind, void (*func)(void *, void *), void *work, u8 prio, u8 alloc);
static TaskKind *data_ov016_021de6e4;
static const u8 data_ov016_021daa84[2] = { 0x80, 0x20 };
void deleteTcb(s32 kind, TaskCb *tcb);

void DWCi_TSKlInit(void) {
    s32 i;
    u32 num;

    data_ov016_021de6e4 = DWCi_HEAPlAllocEx(sizeof(TaskKind) * 2, 4);
    for (i = 0; i < 2; i++) {
        num = data_ov016_021daa84[i];
        data_ov016_021de6e4[i].num = num;
        data_ov016_021de6e4[i].buf = DWCi_HEAPlAllocEx(num * sizeof(TaskCb), 4);
        data_ov016_021de6e4[i].freeQueue = (u16 *)DWCi_QUElFormEx(num, (s32)data_ov016_021de6e4[i].buf, sizeof(TaskCb));
        data_ov016_021de6e4[i].delQueue = (u16 *)DWCi_QUElForm(num);
        data_ov016_021de6e4[i].list = DWCi_LSTlForm();
        data_ov016_021de6e4[i].head.prio = 0;
        data_ov016_021de6e4[i].tail.prio = 0xff;
        DWCi_LSTlPushFront(data_ov016_021de6e4[i].list, (s32 *)&data_ov016_021de6e4[i].head);
        DWCi_LSTlPushBack(data_ov016_021de6e4[i].list, (s32 *)&data_ov016_021de6e4[i].tail);
        data_ov016_021de6e4[i].active = TRUE;
    }
}

void DWCi_TSKlEnd(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        DWCi_LSTlDelete(data_ov016_021de6e4[i].list);
        DWCi_QUElDelete((u32)data_ov016_021de6e4[i].freeQueue);
    }
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de6e4);
}

void DWCi_TSKlAct(s32 kind) {
    TaskCb *t;
    s32 i;

    if (!data_ov016_021de6e4[kind].active) {
        return;
    }
    for (t = data_ov016_021de6e4[kind].head.next; t != &data_ov016_021de6e4[kind].tail; t = t->next) {
        t->func(t, t->work);
    }
    for (i = 0; i < data_ov016_021de6e4[kind].num; i++) {
        t = (TaskCb *)DWCi_QUElPopBack(data_ov016_021de6e4[kind].delQueue);
        if (t == NULL) {
            break;
        }
        deleteTcb(kind, t);
    }
}

s32 *DWCi_TSKlForm(s32 kind, s32 func, void *work, u8 prio) {
    return (s32 *)func_ov016_021d7694(kind, (void (*)(void *, void *))func, (void *)work, prio, 0);
}

TaskCb *func_ov016_021d7694(s32 kind, void (*func)(void *, void *), void *work, u8 prio, u8 alloc) {
    TaskCb *tcb;
    TaskCb *t;
    s32 mask;

    tcb = (TaskCb *)DWCi_QUElPopBack(data_ov016_021de6e4[kind].freeQueue);
    tcb->func = func;
    tcb->work = work;
    tcb->prio = prio;
    tcb->alloc = alloc;
    mask = OS_DisableIrqMask(1);
    t = data_ov016_021de6e4[kind].head.next;
    for (;;) {
        if (prio < t->prio) {
            DWCi_LSTlInsert((s32 *)t, (s32 *)tcb);
            break;
        }
        t = t->next;
    }
    OS_EnableIrqMask(mask);
    return tcb;
}

void DWCi_TSKlChangeFunc(void *task, u32 func) {
    ((TaskCb *)task)->func = (void (*)(void *, void *))func;
}

void DWCi_TSKlDelete(s32 kind, u32 task) {
    DWCi_QUElPushBack(data_ov016_021de6e4[kind].delQueue, task);
}

void DWCi_TSKlDeleteEx(s32 kind, s32 *task) {
    deleteTcb(kind, (TaskCb *)task);
}

void deleteTcb(s32 kind, TaskCb *tcb) {
    if (tcb->alloc) {
        DWCi_HEAPlFree_((u32 *)&tcb->work);
    }
    DWCi_LSTlErase((s32 *)tcb);
    DWCi_QUElPushBack(data_ov016_021de6e4[kind].freeQueue, (u32)tcb);
}

void DWCi_TSKlOperate(s32 kind, u8 active) {
    data_ov016_021de6e4[kind].active = active;
}
