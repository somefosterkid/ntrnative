// Black's code at ov009 0x0217ca20-0x0217cf5c. GameSpy's core (header core.h):
// the gsCore functions and the task functions, found by their bodies.

#include <stdlib.h>
#include "types.h"
#include "gs/nonport.h"
#include "mem.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/sceneOptErase.h"
#include "dwc/gsPlatformThread.h"

static u8 data_ov009_021acfd4[44];

void func_ov114_021c1ff8(void);
typedef struct CoreWork2 {
    u8 unk_00[0x14];
    s32 unk_14;
    s32 unk_18;
    u8 unk_1c[4];
    s32 unk_20;
    u8 unk_24[8];
    u8 unk_2c;
} CoreWork2;

typedef struct GSTask {
    s32 mId;
    u32 mTimeout;
    u32 mStartTime;
    s32 mAutoThink;
    s32 mIsStarted;
    s32 mIsRunning;
    s32 mIsCancelled;
    s32 mIsCallbackPending;
    s32 mTaskData;
    void (*mExecuteFunc)(s32);
    void (*mCallbackFunc)(s32);
    void (*mCancelFunc)(s32);
    s32 (*mCleanupFunc)(s32);
    s32 (*mThinkFunc)(s32);
} GSTask;

typedef struct GSCoreMgr {
    s32 mRefCount;
    // volatile in GameSpy's source: another thread sets the flags while gsCoreInitialize waits for them
    volatile s32 critSecCreated;
    volatile s32 mIsStaticInitComplete;
    volatile s32 mIsShuttingDown;
    OSMutex mQueueCrit;
    void *mTaskArray;
} GSCoreMgr;

void gsiCoreCancelTask(CoreWork2 *param_1);

#pragma thumb off
u8 *func_ov009_0217ca20(void) {
    return (void *)data_ov009_021acfd4;
}
#pragma thumb on

#pragma thumb off
void func_ov009_0217ca2c(void) {
    func_ov009_0217f120((OSMutex *)(func_ov009_0217ca20() + 0x10));
}

void gsCoreInitialize(void) {
    GSCoreMgr *core = (GSCoreMgr *)func_ov009_0217ca20();

    while (core->mIsShuttingDown) {
        func_ov009_0217f2bc(100);
    }
    if (func_ov009_0217f0b4(&core->mRefCount) == 1) {
        if (!core->critSecCreated) {
            func_ov009_0217f0fc(&core->mQueueCrit);
            atexit(func_ov009_0217ca2c);
            core->critSecCreated = TRUE;
        }
        func_ov009_0217f108(&core->mQueueCrit);
        func_ov009_0217f114(&core->mQueueCrit);
        while (core->mIsShuttingDown) {
            func_ov009_0217f2bc(100);
        }
        core->mTaskArray = ArrayNew(sizeof(GSTask *), 10, NULL);
        func_ov114_021c1ff8();
        core->mIsStaticInitComplete = TRUE;
    } else {
        while (!core->critSecCreated) {
            func_ov009_0217f2bc(100);
        }
        func_ov009_0217f108(&core->mQueueCrit);
        func_ov009_0217f114(&core->mQueueCrit);
        while (!core->mIsStaticInitComplete) {
            func_ov009_0217f2bc(100);
        }
    }
}
#pragma thumb on

#pragma thumb off
s64 func_ov009_0217cb5c(void *arg0, s32 result) {
    s32 (*func)(s32);

    if (*(s32 *)((u8 *)(u8 *)arg0 + 0x1C) != 0) {
        func = *(s32 (**)(s32))((u8 *)(u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)(u8 *)arg0 + 0x1C) = 0;
        func = *(s32 (**)(s32))((u8 *)((u8 *)arg0) + 0x28);
        if (func != NULL) {
            func(*(s32 *)((u8 *)(u8 *)arg0 + 0x20));
        }
    }
}

s32 func_ov009_0217cb88(GSTask *task) {
    u8 *core = func_ov009_0217ca20();
    s32 result = 0;
    s32 finished;
    s32 count;
    long i;

    if (task == NULL) {
        return 4;
    }
    if (task->mIsRunning != 0 && task->mThinkFunc != NULL) {
        result = task->mThinkFunc(task->mTaskData);
    }
    if (task->mIsCancelled == 0 && result == 1) {
        if (task->mTimeout != 0 && current_time() - task->mStartTime > task->mTimeout) {
            gsiCoreCancelTask((CoreWork2 *)task);
            func_ov009_0217cb5c(task, 3);
        }
    } else if (result != 1) {
        finished = 1;
        if (task->mIsRunning != 0) {
            func_ov009_0217cb5c(task, result);
            task->mIsRunning = 0;
        }
        if (task->mCleanupFunc != NULL) {
            finished = task->mCleanupFunc(task->mTaskData);
        }
        if (finished != 0) {
            func_ov009_0217f108((OSMutex *)(core + 0x10));
            count = ArrayLength(*(void **)(core + 0x28));
            for (i = 0; i < count; i++) {
                if (*(GSTask **)ArrayNth(*(void **)(core + 0x28), i) == task) {
                    ArrayRemoveAt(*(void **)(core + 0x28), i);
                    gsifree(task);
                    break;
                }
            }
            func_ov009_0217f114((OSMutex *)(core + 0x10));
            return 4;
        }
    }
    return 1;
}


s32 func_ov009_0217cb88(GSTask *task);
void gsCoreThink(u32 timeout) {
    u8 *core = func_ov009_0217ca20();
    s32 i;
    s32 isFinished = 1;
    u32 startTime;
    s32 count;

    if (*(s32 *)(core + 8) == 0) {
        return;
    }
    func_ov009_0217f108((OSMutex *)(core + 0x10));
    startTime = current_time();
    count = ArrayLength(*(void **)(core + 0x28));
    if (count > 0) {
        isFinished = 0;
    }
    for (i = count - 1; i >= 0; i--) {
        GSTask *task = *(GSTask **)ArrayNth(*(void **)(core + 0x28), i);

        if (task->mAutoThink != 0) {
            func_ov009_0217cb88(task);
        }
        if (timeout != 0 && current_time() - startTime > timeout) {
            break;
        }
    }
    if (*(s32 *)(core + 0xc) != 0 && isFinished != 0) {
        procInit1_sceneOptErase();
        if (*(void **)(core + 0x28) != NULL) {
            ArrayFree(*(void **)(core + 0x28));
            *(void **)(core + 0x28) = NULL;
        }
        *(s32 *)(core + 0xc) = 0;
    }
    func_ov009_0217f114((OSMutex *)(core + 0x10));
}

#pragma thumb on

#pragma thumb off
void gsCoreShutdown(void) {
    s32 *ptr;
    s32 arrayLength;
    s32 *elem;
    s32 index;

    ptr = (s32 *)func_ov009_0217ca20();
    if (ptr[2] == 0) {
        return;
    }
    func_ov009_0217f108((OSMutex *)(ptr + 4));
    arrayLength = func_ov009_0217f0d8(ptr);
    if (arrayLength != 0) {
        func_ov009_0217f114((OSMutex *)(ptr + 4));
        return;
    }
    ptr[3] = 1;
    arrayLength = ArrayLength((void *)ptr[10]);
    index = 0;
    if (0 < arrayLength) {
        do {
            elem = ArrayNth((void *)ptr[10], index);
            gsiCoreCancelTask((CoreWork2 *)(*elem));
            index = index + 1;
        } while (index < arrayLength);
    }
    func_ov009_0217f114((OSMutex *)(ptr + 4));
}
#pragma thumb on

#pragma thumb off
u32 gsCoreIsShutdown(void) {
    s32 *ptr;
    u32 val;
    ptr = (s32 *)func_ov009_0217ca20();
    if (ptr[3] != 0) {
        return 1;
    }
    if (*ptr == 0) {
        val = 2;
    } else {
        val = 0;
    }
    return val;
}

void gsiCoreExecuteTask(GSTask *task, u32 timeout) {
    u8 *core = func_ov009_0217ca20();

    task->mIsCallbackPending = 1;
    task->mIsStarted = 1;
    task->mIsRunning = 1;
    task->mTimeout = timeout;
    task->mStartTime = current_time();
    if (task->mExecuteFunc != NULL) {
        task->mExecuteFunc(task->mTaskData);
    }
    func_ov009_0217f108((OSMutex *)(core + 0x10));
    ArrayAppend(*(void **)(core + 0x28), &task);
    func_ov009_0217f114((OSMutex *)(core + 0x10));
}

#pragma thumb on

#pragma thumb off
void gsiCoreCancelTask(CoreWork2 *param_1) {
    s32 (*func)(s32);
    u8 *ptr;

    ptr = func_ov009_0217ca20();
    func_ov009_0217f108((OSMutex *)(ptr + 0x10));
    if ((param_1->unk_14 != 0) && (param_1->unk_18 == 0)) {
        func = *(s32 (**)(s32))(u8 *)&param_1->unk_2c;
        param_1->unk_18 = 1;
        if (func != NULL) {
            func(param_1->unk_20);
        }
    }
    func_ov009_0217f114((OSMutex *) (ptr + 0x10));
}
#pragma thumb on

#pragma thumb off
void *gsiCoreCreateTask(void) {
    void *dest;

    dest = gsimalloc(0x38);
    if (dest != (void *)0x0) {
        memset(dest, 0, 0x38);
        *(u32 *)((s32)(u8 *)dest + 0xc) = 1;
        return dest;
    }
    return (void *)0x0;
}
#pragma thumb on
