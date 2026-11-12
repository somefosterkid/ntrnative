// Black's code at ov021 0x021c6cbc-0x021c6ea8. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "field/field_task.h"
#include "field/ov021_021c6ea8.h"


u8 func_ov021_021c6e20(FieldTaskManager *mgr);
void FieldTask_Free(FieldTask *task);
void FieldTask_Run(FieldTask *task);
void func_ov021_021c6cf4(FieldTask *task);

void FieldTaskManager_EndTask(FieldTaskManager *mgr, u8 index);
void FieldTaskManager_RunTask(FieldTaskManager *mgr, u8 index);
void FieldTaskManager_SetTask(FieldTaskManager *mgr, u8 index, FieldTask *task, FieldTask *parent);
void FieldTask_SetStarted(FieldTask *task);
void func_ov021_021c6ce4(FieldTask *task);


void FieldTask_Run(FieldTask *task) {
    func_ov021_021c6cf4(task);
}

void FieldTask_SetStarted(FieldTask *task) {
    func_ov021_021c6ce4(task);
}


void *FieldTask_GetData(FieldTask *task) {
    return task->data;
}

u32 FieldTask_CheckStateReqEnd(FieldTask *task) {
    if (task->state == 2) {
        return 1;
    }
    return 0;
}

u32 FieldTask_GetState(FieldTask *task) {
    return task->state;
}

void func_ov021_021c6ce4(FieldTask *task) {
    task->state = 1;
}

void FieldTask_ReqEnd(FieldTask *task) {
    task->state = 2;
}

void func_ov021_021c6cf4(FieldTask *task) {
    if (task->state == 1) {
        if (task->callback(task->data) == 1) {
            FieldTask_ReqEnd(task);
        }
    }
}


FieldTaskManager *func_ov021_021c6d10(u32 count, HeapID heapId) {
    FieldTaskManager *mgr;

    mgr = GFL_HeapAllocate(heapId, sizeof(FieldTaskManager), 0, "field_task_manager.c", 0x34);
    mgr->count = count;
    mgr->tasks = GFL_HeapAllocate(heapId, count * sizeof(FieldTask *), 1, "field_task_manager.c", 0x36);
    mgr->parents = GFL_HeapAllocate(heapId, count * sizeof(FieldTask *), 1, "field_task_manager.c", 0x37);
    return mgr;
}

void func_ov021_021c6d5c(FieldTaskManager *mgr) {
    u32 i;
    i = 0;
    while ((s32)i < (s32)(u32)mgr->count) {
        FieldTaskManager_EndTask(mgr, i);
        i = i + 1;
    }
    GFL_HeapFree(mgr->tasks);
    GFL_HeapFree(mgr->parents);
    GFL_HeapFree(mgr);
}

void func_ov021_021c6d90(FieldTaskManager *mgr) {
    u32 i;

    for (i = 0; (s32)i < (s32)(u32)mgr->count; i++) {
        FieldTaskManager_RunTask(mgr, i);
    }
    for (i = 0; (s32)i < (s32)(u32)mgr->count; i++) {
        FieldTaskManager_UpdateParentControlTransfer(mgr, i);
    }
    for (i = 0; (s32)i < (s32)(u32)mgr->count; i++) {
        FieldTaskManager_UpdateEndTasks(mgr, i);
    }
}


void FieldTaskManager_AddTask(FieldTaskManager *mgr, FieldTask *task, FieldTask *parent) {
    u32 i;
    i = 0;
    while ((s32)i < (s32)(u32)mgr->count) {
        if (mgr->tasks[i] == NULL) {
            FieldTaskManager_SetTask(mgr, i, task, parent);
            if (parent != NULL) {
                return;
            }
            FieldTask_SetStarted(task);
            return;
        }
        i = i + 1;
    }
    FieldTask_Free(task);
}

u8 func_ov021_021c6e20(FieldTaskManager *mgr) {
    s32 i;
    u8 count;

    count = 0;
    for (i = 0; i < mgr->count; i++) {
        if (mgr->tasks[i] != NULL) {
            count++;
        }
    }
    return count;
}


BOOL FieldTaskManager_IsIdle(FieldTaskManager *taskManager) {
    if (func_ov021_021c6e20(taskManager) == 0) {
        return 1;
    }
    return 0;
}

void FieldTaskManager_SetTask(FieldTaskManager *mgr, u8 index, FieldTask *task, FieldTask *parent) {
    if (mgr->tasks[index] == NULL) {
        mgr->tasks[index] = task;
        mgr->parents[index] = parent;
    }
}

void FieldTaskManager_EndTask(FieldTaskManager *mgr, u8 index) {
    if (mgr->tasks[index] != NULL) {
        FieldTask_Free(mgr->tasks[index]);
        mgr->tasks[index] = NULL;
        mgr->parents[index] = NULL;
    }
}

void FieldTaskManager_RunTask(FieldTaskManager *mgr, u8 index) {
    if (mgr->tasks[index] != NULL) {
        FieldTask_Run(mgr->tasks[index]);
    }
}
