// The PC build's thread model (docs/port-runtime.md "Threads"; the game's calls reach these through
// port/include/port_runtime.h's macros): the game lock, which is the DS's one CPU, and the game's own threads run on
// it.
//
// A game thread is two OSThreads: the game's (its OSThread variable: the game reads its state, and libntr never sees
// it) and a host one from the table below, which libntr runs as an SDL thread and which calls the game's function
// with the lock held. The host thread is made at the game's OS_WakeupThreadDirect, so a created thread waits for its
// wakeup as on the DS. A game thread runs until it blocks (PORT_Sleep and kin release the lock), so it never
// stops midway for another game thread. Black's threads: the BGM loader (src/system/snd_sys.c: one
// NNS_SndArcLoadSeqEx per thread, put to sleep by main.c's loop around each V-blank wait, destroyed and remade when
// the BGM changes) and lib/poke_net's two (online).
#define BWPORT_PORT_RUNTIME_IMPL
#include <nitro.h>
#include <SDL2/SDL.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "port_route.h"

// The game's files see port_runtime.h through port_prefix.h, so this one may too: the real SDK calls here
#undef OS_Sleep
#undef OS_SleepThread
#undef OS_WaitIrq
#undef OS_WaitVBlankIntr
#undef OS_CreateThread
#undef OS_WakeupThreadDirect
#undef OS_SleepThreadDirect
#undef OS_DestroyThread
#undef OS_JoinThread
#undef OS_LockMutex
#undef OS_ReceiveMessage
#undef OS_SendMessage
#undef OS_SetIrqFunction

#define PORT_THREAD_MAX 16
#define PORT_HOST_STACK 256

// A slot's host thread is made once and kept: it runs one game thread after another (the BGM loader is a new
// thread per load). libntr's thread table (SIM_OS_RunThread's s_threadList) isn't safe against threads that start
// and end at the same time, so the PC build doesn't make a host thread per game thread
typedef struct PortThread {
    OSThread host;          // libntr's: an SDL thread that runs HostMain
    SDL_sem *wake;          // posted at the game thread's wakeup
    BOOL hostStarted;
    volatile BOOL busy;     // a game thread is on the slot (until it ends, or is destroyed before it ran)
    volatile BOOL dead;     // its game thread was destroyed while it slept: the host is parked for good
    OSThread *game;         // the game's OSThread, NULL once destroyed or remade
    void (*func)(void *);
    void *arg;
    u32 prio;
    volatile BOOL cancelled;
    jmp_buf home;           // HostMain's loop: where the in-process restart sends the slot back (machine.c)
    u32 stack[PORT_HOST_STACK / sizeof(u32)];   // libntr clears it at creation; the SDL thread has its own
} PortThread;

static PortThread sThreads[PORT_THREAD_MAX];
static SDL_mutex *sGameLock;    // recursive (SDL mutexes are)
static SDL_SpinLock sInitLock;
static __thread int tDepth;     // this host thread's hold count on sGameLock
static __thread PortThread *tSelf;  // this host thread's game thread, NULL on the main thread
static volatile BOOL sResetting;    // the in-process restart parks every game thread (PORT_ThreadsPark)
static OSIrqFunction sHBlank, sVBlank;   // the game's H-blank and V-blank handlers (PORT_SetIrqFunction)

static SDL_mutex *GameLockMutex(void) {
    if (sGameLock == NULL) {
        SDL_AtomicLock(&sInitLock);
        if (sGameLock == NULL) {
            sGameLock = SDL_CreateMutex();
        }
        SDL_AtomicUnlock(&sInitLock);
    }
    return sGameLock;
}

void PORT_GameLock(void) {
    SDL_LockMutex(GameLockMutex());
    tDepth++;
}

void PORT_GameUnlock(void) {
    tDepth--;
    SDL_UnlockMutex(GameLockMutex());
}

// Before a wait: the lock off, however deep; the depth to take it back to
static int ReleaseAll(void) {
    int depth = tDepth;
    while (tDepth > 0) {
        PORT_GameUnlock();
    }
    return depth;
}

// A game thread's turn on the CPU: the lock, once its thread is awake (not put to sleep by OS_SleepThreadDirect).
// FALSE, without the lock, once it has been destroyed. Between checks it sleeps on the slot's semaphore, which every
// change it waits for posts (PORT_WakeupThreadDirect, PORT_DestroyThread, PORT_CreateThread's remake) after making it
// under the lock: a post between the check and the wait is kept by the semaphore, and an extra one only costs a check
static BOOL WaitTurn(int depth) {
    for (;;) {
        int i;

        if (tSelf->cancelled) {
            return FALSE;
        }
        for (i = 0; i < depth; i++) {
            PORT_GameLock();
        }
        if (tSelf->cancelled) {
            ReleaseAll();
            return FALSE;
        }
        if (tSelf->game == NULL || tSelf->game->state != OS_THREAD_STATE_WAITING) {
            return TRUE;
        }
        ReleaseAll();
        SDL_SemWaitTimeout(tSelf->wake, 100);   // (a backstop: a change that forgot its post costs 0.1 s, not a hang)
    }
}

static void Park(void) {
    for (;;) {
        if (sResetting && tSelf != NULL) {
            longjmp(tSelf->home, 1);   // the restart frees the slot: its host runs the next game's threads
        }
        if (tSelf != NULL) {
            SDL_SemWaitTimeout(tSelf->wake, 100);
        } else {
            SDL_Delay(100);
        }
    }
}

// After a wait. A game thread destroyed during it never comes back: C can't unwind its stack, so it parks for good
// without the lock (lib/poke_net's receive thread, online only, is the one the game destroys while it sleeps)
static void Reacquire(int depth) {
    int i;

    if (tSelf == NULL) {
        for (i = 0; i < depth; i++) {
            PORT_GameLock();
        }
    } else if (!WaitTurn(depth)) {
        if (sResetting) {
            longjmp(tSelf->home, 1);
        }
        tSelf->dead = TRUE;
        Park();
    }
}

// The main thread holds the lock from the start (main_start.c's constructor runs on it, before libntr's main)
void PORT_RuntimeInit(void) {
    if (tDepth == 0) {
        PORT_GameLock();
    }
}

void PORT_Sleep(u32 msec) {
    int depth = ReleaseAll();
    OS_Sleep(msec);
    Reacquire(depth);
}

void PORT_SleepThread(OSThreadQueue *queue) {
    // libntr's returns after a millisecond (its threads are host threads): the game's callers loop on their condition
    int depth = ReleaseAll();
    OS_SleepThread(queue);
    Reacquire(depth);
}

void PORT_WaitIrq(BOOL clear, OSIrqMask irqFlags) {
    // the V-blank wait: libntr draws the frame and runs the H-blank and V-blank handlers in it (they take the lock
    // themselves, PORT_SetIrqFunction); a game thread that is awake may run meanwhile, as on the DS
    int depth;

    PORT_BenchFrame();
    PortRoute_VBlank();   // BWPORT_PILOT before the field (route_test.c)
    PORT_MachineFrame();
    depth = ReleaseAll();
    OS_WaitIrq(clear, irqFlags);
    Reacquire(depth);
}

void PORT_WaitVBlankIntr(void) {
    int depth = ReleaseAll();
    OS_WaitVBlankIntr();
    Reacquire(depth);
}

static PortThread *FindThread(const OSThread *game) {
    int i;

    for (i = 0; i < PORT_THREAD_MAX; i++) {
        if (sThreads[i].busy && sThreads[i].game == game) {
            return &sThreads[i];
        }
    }
    return NULL;
}

static int HostMain(void *arg);
static void StartHost(PortThread *t);

static void HostEntry(void *arg) {
    (void)HostMain(arg);
}

// The host thread of a slot: waits for a game thread's wakeup, runs it on its turn, and waits for the next
static int HostMain(void *arg) {
    PortThread *t = arg;

    tSelf = t;
    if (setjmp(t->home) != 0) {
        // back from a game thread's frames at an in-process restart (Park, Reacquire): the lock is not held
        tDepth = 0;
        t->game = NULL;
        t->dead = FALSE;
        t->busy = FALSE;
    }
    for (;;) {
        SDL_SemWaitTimeout(t->wake, 100);   // (a backstop, as WaitTurn's; the checks below run again)
        if (!t->busy) {
            continue;
        }
        if (t->cancelled) {
            t->busy = FALSE;    // destroyed or remade before it ran
            continue;
        }
        if (t->game == NULL || t->game->state != OS_THREAD_STATE_READY) {
            continue;           // not woken yet
        }
        if (WaitTurn(1)) {
            t->func(t->arg);
            if (t->game != NULL) {
                t->game->state = OS_THREAD_STATE_TERMINATED;
            }
            t->game = NULL;
            t->busy = FALSE;
            ReleaseAll();
        } else {
            t->busy = FALSE;
        }
    }
    return 0;
}

void PORT_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio) {
    PortThread *t = FindThread(thread);
    int i;

    (void)stack;
    (void)stackSize;
    if (t != NULL) {
        // the game remakes a thread variable that hasn't ended (the caller holds the lock, so it isn't running)
        t->cancelled = TRUE;
        t->game = NULL;
        if (t->hostStarted) {
            SDL_SemPost(t->wake);   // its host waits on it (WaitTurn, HostMain) and lets the slot go
        }
    }
    t = NULL;
    for (i = 0; i < PORT_THREAD_MAX; i++) {
        if (!sThreads[i].busy && !sThreads[i].dead) {
            t = &sThreads[i];
            break;
        }
    }
    if (t == NULL) {
        fprintf(stderr, "bwport: PORT_CreateThread: more than %d game threads\n", PORT_THREAD_MAX);
        abort();
    }
    t->game = thread;
    t->func = func;
    t->arg = arg;
    t->prio = prio;
    t->cancelled = FALSE;
    t->busy = TRUE;

    memset(thread, 0, sizeof(*thread));
    thread->priority = prio;
    thread->state = OS_THREAD_STATE_WAITING;
    thread->win_threadState = 1;    // libntr's OS_WakeupThreadDirect waits for it
    thread->id = 0x100 + (u32)(t - sThreads);
}

void PORT_WakeupThreadDirect(OSThread *thread) {
    PortThread *t = FindThread(thread);

    if (t == NULL) {
        // a thread that was never made (every game thread is ours): on the DS only its state changes, as it is in
        // no run queue. main.c's loop sleeps and wakes the BGM loader g_GFLSndThread every frame, made or not;
        // libntr's own would wait for a host thread that never starts
        thread->state = OS_THREAD_STATE_READY;
        return;
    }
    if (thread->state == OS_THREAD_STATE_TERMINATED) {
        return;
    }
    thread->state = OS_THREAD_STATE_READY;
    if (!t->hostStarted) {
        StartHost(t);
    }
    SDL_SemPost(t->wake);
}

// A slot's host thread, made at the first wakeup of a game thread on it, or before the game (PORT_ThreadsPrestart)
static void StartHost(PortThread *t) {
    t->hostStarted = TRUE;
    t->wake = SDL_CreateSemaphore(0);
    OS_CreateThreadDebug(&t->host, HostEntry, t, t->stack + sizeof(t->stack) / sizeof(t->stack[0]), sizeof(t->stack),
                         t->prio, "bwport game thread", __FILE__, __LINE__);
    OS_WakeupThreadDirect(&t->host);
}

// The in-process restart (machine.c). Apple builds make every slot's host thread before the game starts, so libntr's
// thread tables, which are the machine's and are put back at a reset, already hold them all then
void PORT_ThreadsPrestart(void) {
    int i;

    for (i = 0; i < PORT_THREAD_MAX; i++) {
        if (!sThreads[i].hostStarted) {
            sThreads[i].prio = 16;
            StartHost(&sThreads[i]);
        }
    }
}

// The main thread, at the game's reset (the lock held, however deep): every game thread leaves its frames for its
// host's loop (from a wait: Reacquire, Park), with the lock let go until they have; `unlocked` runs then too. Returns
// with the lock held once
void PORT_ThreadsPark(void (*unlocked)(void)) {
    Uint32 t0;
    int i, left;

    sResetting = TRUE;
    for (i = 0; i < PORT_THREAD_MAX; i++) {
        PortThread *t = &sThreads[i];

        if (t->busy || t->dead) {
            if (t->game != NULL) {
                t->game->state = OS_THREAD_STATE_TERMINATED;
            }
            t->cancelled = TRUE;
            t->game = NULL;
            if (t->hostStarted) {
                SDL_SemPost(t->wake);
            }
        }
    }
    ReleaseAll();
    t0 = SDL_GetTicks();
    do {
        left = 0;
        for (i = 0; i < PORT_THREAD_MAX; i++) {
            left += sThreads[i].hostStarted && (sThreads[i].busy || sThreads[i].dead);
        }
        if (left != 0) {
            SDL_Delay(1);
        }
    } while (left != 0 && SDL_GetTicks() - t0 < 2000);
    if (left != 0) {
        fprintf(stderr, "bwport: reset: %d game threads did not come back (in a wait of libntr's own)\n", left);
    }
    if (unlocked != NULL) {
        unlocked();
    }
    PORT_GameLock();
}

// After the machine is back: the slots free, the interrupt handlers the game set gone (it sets them again at boot)
void PORT_ThreadsReset(void) {
    int i;

    for (i = 0; i < PORT_THREAD_MAX; i++) {
        PortThread *t = &sThreads[i];

        if (!t->busy && !t->dead) {
            t->cancelled = FALSE;
            t->game = NULL;
            while (t->hostStarted && SDL_SemTryWait(t->wake) == 0) {
            }
        }
    }
    sHBlank = sVBlank = NULL;
    sResetting = FALSE;
}

void PORT_SleepThreadDirect(OSThread *thread, OSThreadQueue *queue) {
    PortThread *t = FindThread(thread);

    if (t == NULL) {
        // never made: its state only, as PORT_WakeupThreadDirect
        thread->state = OS_THREAD_STATE_WAITING;
        return;
    }
    // it is not running (the caller holds the lock): it stays off the CPU until its wakeup
    if (thread->state != OS_THREAD_STATE_TERMINATED) {
        thread->state = OS_THREAD_STATE_WAITING;
    }
}

void PORT_DestroyThread(OSThread *thread) {
    PortThread *t = FindThread(thread);

    if (t == NULL) {
        OS_DestroyThread(thread);
        return;
    }
    thread->state = OS_THREAD_STATE_TERMINATED;
    t->cancelled = TRUE;
    t->game = NULL;
    if (t == tSelf) {
        // a thread that destroys itself: its function's end
        t->dead = TRUE;
        ReleaseAll();
        Park();
    }
    if (t->hostStarted) {
        SDL_SemPost(t->wake);
    } else {
        t->busy = FALSE;
    }
}

void PORT_JoinThread(OSThread *thread) {
    if (FindThread(thread) == NULL && thread->state != OS_THREAD_STATE_TERMINATED) {
        int depth = ReleaseAll();
        OS_JoinThread(thread);
        Reacquire(depth);
        return;
    }
    while (thread->state != OS_THREAD_STATE_TERMINATED) {
        PORT_Sleep(1);
    }
}

void PORT_LockMutex(OSMutex *mutex) {
    int depth;

    if (OS_TryLockMutex(mutex)) {
        return;
    }
    depth = ReleaseAll();
    OS_LockMutex(mutex);
    Reacquire(depth);
}

BOOL PORT_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags) {
    // libntr's queue isn't safe against another host thread; under the lock the game's own users are one at a time
    while (!OS_ReceiveMessage(mq, msg, OS_MESSAGE_NOBLOCK)) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            return FALSE;
        }
        PORT_Sleep(1);
    }
    return TRUE;
}

BOOL PORT_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags) {
    while (!OS_SendMessage(mq, msg, OS_MESSAGE_NOBLOCK)) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            return FALSE;
        }
        PORT_Sleep(1);
    }
    return TRUE;
}

// The H-blank and V-blank handlers: libntr calls them from OS_WaitIrq's frame (PORT_WaitIrq has let the lock go),
// so they take the lock as an interrupt takes the DS's CPU
static void HBlankTrampoline(void) {
    PORT_GameLock();
    if (sHBlank != NULL) {
        sHBlank();
    }
    PORT_GameUnlock();
}

static void VBlankTrampoline(void) {
    PORT_GameLock();
    if (sVBlank != NULL) {
        sVBlank();
    }
    PORT_GameUnlock();
}

void PORT_SetIrqFunction(OSIrqMask intrBit, OSIrqFunction function) {
    if (function == HBlankTrampoline || function == VBlankTrampoline) {
        // a handler the game read back with OS_GetIrqFunction and puts back
        OS_SetIrqFunction(intrBit, function);
        return;
    }
    if (intrBit == OS_IE_H_BLANK) {
        sHBlank = function;
        OS_SetIrqFunction(intrBit, HBlankTrampoline);
    } else if (intrBit == OS_IE_V_BLANK) {
        sVBlank = function;
        OS_SetIrqFunction(intrBit, VBlankTrampoline);
    } else {
        OS_SetIrqFunction(intrBit, function);
    }
}
