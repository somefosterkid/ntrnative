// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_mutex.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/types.h>
#include <nitro.h>

void OSi_EnqueueTail(OSThread * thread, OSMutex * mutex);
void OSi_DequeueItem(OSThread * thread, OSMutex * mutex);

void OSi_UnlockAllMutex(OSThread *thread)
{
    OSMutex *mutex;
    u32 tag;

    if (thread->mutexQueue.head == NULL) {
        return;
    }
    do {
        mutex = OSi_RemoveMutexLinkFromQueue(&thread->mutexQueue);
        tag = mutex->count & 0xff000000;
        mutex->thread = NULL;
        mutex->count = tag & 0xffffff;
        OS_WakeupThread(&mutex->queue);
    } while (thread->mutexQueue.head != NULL);
}

BOOL OS_TryLockMutex(OSMutex *mutex)
{
    u32 state;
    BOOL result;
    OSThread *currentThread;
    u32 combined;
    u32 tag;

    state = OS_DisableInterrupts();
    currentThread = OSi_ThreadInfo.current;
    if (mutex->thread == NULL) {
        combined = (mutex->count & 0xffffff) | 0x10000000;
        tag = combined & 0xff000000;
        mutex->thread = currentThread;
        mutex->count = tag | ((combined + 1) & 0xffffff);
        OSi_EnqueueTail(currentThread, mutex);
        result = TRUE;
    } else if (mutex->thread != currentThread) {
        result = FALSE;
    } else {
        result = TRUE;
        tag = mutex->count & 0xff000000;
        mutex->count = tag | ((mutex->count + 1) & 0xffffff);
    }
    OS_RestoreInterrupts(state);
    return result;
}

#define OSi_MUTEX_TYPE_MASK 0xff000000
#define OSi_MUTEX_COUNT_MASK 0x00ffffff
#define OSi_MUTEX_TYPE_NONE 0x00000000
#define OSi_MUTEX_TYPE_STD 0x10000000
#define OSi_MUTEX_TYPE_R 0x20000000
#define OSi_MUTEX_TYPE_W 0x30000000

static inline u32 OSi_GetMutexType(OSMutex *mutex)
{
    return mutex->count & OSi_MUTEX_TYPE_MASK;
}

static inline s32 OSi_GetMutexCount(OSMutex *mutex)
{
    return mutex->count & OSi_MUTEX_COUNT_MASK;
}

static inline void OSi_SetMutexCount(OSMutex *mutex, s32 count)
{
    mutex->count = (mutex->count & OSi_MUTEX_TYPE_MASK) | (count & OSi_MUTEX_COUNT_MASK);
}

static inline void OSi_SetMutexType(OSMutex *mutex, u32 type)
{
    mutex->count = (mutex->count & OSi_MUTEX_COUNT_MASK) | type;
}

static inline void OSi_DecreaseMutexCount(OSMutex *mutex)
{
    u32 type = mutex->count & OSi_MUTEX_TYPE_MASK;
    mutex->count--;
    mutex->count = type | (mutex->count & OSi_MUTEX_COUNT_MASK);
}
void func_02086164(OSMutex *mutex, u32 type)
{
    u32 state = OS_DisableInterrupts();
    BOOL unlock = FALSE;
    OSThread *current = OSi_ThreadInfo.current;

    if (type != OSi_MUTEX_TYPE_NONE && type != OSi_GetMutexType(mutex)) {
        OS_RestoreInterrupts(state);
        return;
    }

    switch (OSi_GetMutexType(mutex)) {
    case OSi_MUTEX_TYPE_STD:
    case OSi_MUTEX_TYPE_W:
        if (mutex->thread == current) {
            OSi_DecreaseMutexCount(mutex);
            if (OSi_GetMutexCount(mutex) == 0) {
                unlock = TRUE;
            }
        }
        break;
    case OSi_MUTEX_TYPE_R:
        OSi_DecreaseMutexCount(mutex);
        if (OSi_GetMutexCount(mutex) == 0) {
            unlock = TRUE;
        }
        break;
    default:
        OS_RestoreInterrupts(state);
        return;
    }

    if (unlock) {
        OSi_DequeueItem(current, mutex);
        mutex->thread = NULL;
        OSi_SetMutexType(mutex, OSi_MUTEX_TYPE_NONE);
        OS_WakeupThread(&mutex->queue);
    }
    OS_RestoreInterrupts(state);
}



void OSi_EnqueueTail (OSThread * thread, OSMutex * mutex)
{
#ifndef SDK_THREAD_INFINITY
    OSMutex * prev = thread->mutexQueueTail;

    SDK_ASSERT(thread && mutex);

    if (!prev) {
        thread->mutexQueueHead = mutex;
    } else {
        prev->next = mutex;
    }

    mutex->prev = prev;
    mutex->next = NULL;
    thread->mutexQueueTail = mutex;
#else
    OSMutex * prev = thread->mutexQueue.tail;

    SDK_ASSERT(thread && mutex);

    if (!prev) {
        thread->mutexQueue.head = mutex;
    } else {
        prev->link.next = mutex;
    }

    mutex->link.prev = prev;
    mutex->link.next = NULL;
    thread->mutexQueue.tail = mutex;
#endif
}

void OSi_DequeueItem (OSThread * thread, OSMutex * mutex)
{
#ifndef SDK_THREAD_INFINITY
    OSMutex * next = mutex->next;
    OSMutex * prev = mutex->prev;

    SDK_ASSERT(thread && mutex);

    if (!next) {
        thread->mutexQueueTail = prev;
    } else {
        next->prev = prev;
    }

    if (!prev) {
        thread->mutexQueueHead = next;
    } else {
        prev->next = next;
    }
#else
    OSMutex * next = mutex->link.next;
    OSMutex * prev = mutex->link.prev;

    SDK_ASSERT(thread && mutex);

    if (!next) {
        thread->mutexQueue.tail = prev;
    } else {
        next->link.prev = prev;
    }

    if (!prev) {
        thread->mutexQueue.head = next;
    } else {
        prev->link.next = next;
    }
#endif
}
