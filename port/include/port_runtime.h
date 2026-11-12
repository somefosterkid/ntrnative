// The PC build's thread model for the game's C (docs/port-runtime.md "Threads"). It is included after libntr's
// headers (port_prefix.h's last line) and only the game's files see it.
//
// On the DS, Black's threads share one CPU: a thread runs until it blocks (a sleep, the V-blank wait, an empty
// message queue, a held mutex), interrupt handlers run between instructions of whatever runs, and
// OS_DisableInterrupts makes a critical section. On libntr every OSThread is a host thread running at the same time
// as the others and OS_DisableInterrupts does nothing, and a thread starts running at OS_CreateThread. The PC build
// keeps the DS's rule for the game's own threads: one game lock (the DS's CPU), held by the game thread that runs and
// released only where a DS thread would block; a created thread starts at its OS_WakeupThreadDirect; a thread put
// to sleep with OS_SleepThreadDirect doesn't take the CPU until it is woken; OS_DestroyThread keeps it from running
// again; the V-blank and H-blank handlers hold the lock while they run.
#ifndef BWPORT_PORT_RUNTIME_H
#define BWPORT_PORT_RUNTIME_H

#include <nitro.h>

void PORT_Sleep(u32 msec);
void PORT_SleepThread(OSThreadQueue *queue);
void PORT_WaitIrq(BOOL clear, OSIrqMask irqFlags);
void PORT_BenchFrame(void);   // port/src/bench_pin.c: BWPORT_BENCH_SEED, off by default
void PORT_MachineFrame(void); // port/src/machine.c: BWPORT_MACHINE_CHECK, off by default
void PORT_MachineReset(void); // port/src/machine.c: the in-process restart (Apple builds)
void PORT_WaitVBlankIntr(void);
void PORT_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
void PORT_WakeupThreadDirect(OSThread *thread);
void PORT_SleepThreadDirect(OSThread *thread, OSThreadQueue *queue);
void PORT_DestroyThread(OSThread *thread);
void PORT_JoinThread(OSThread *thread);
void PORT_LockMutex(OSMutex *mutex);
BOOL PORT_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
BOOL PORT_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
void PORT_SetIrqFunction(OSIrqMask intrBit, OSIrqFunction function);
void PORT_ResetSystem(u32 parameter);   // port/src/reset.c
OSTick PORT_GetTick(void);   // port/src/bench_tick.c: frame-based under BWPORT_BENCH_CLOCK

// The game lock, for PC code that calls into the game from a host thread of its own (port/src/main_threads.c)
void PORT_GameLock(void);
void PORT_GameUnlock(void);

#ifndef BWPORT_PORT_RUNTIME_IMPL
// The game's entry: src/system/main.c's main (pokebw2's name; crt0 jumps to it) is the SDK's NitroMain, which
// libntr's main() calls. Every `main` of the game's C is renamed alike (struct fields too), after libntr's headers
#define main NitroMain
#undef OS_CreateThread
#define OS_Sleep(msec) PORT_Sleep(msec)
#define OS_SleepThread(queue) PORT_SleepThread(queue)
#define OS_WaitIrq(clear, irqFlags) PORT_WaitIrq(clear, irqFlags)
#define OS_WaitVBlankIntr() PORT_WaitVBlankIntr()
#define OS_CreateThread(thread, func, arg, stack, stackSize, prio) \
    PORT_CreateThread(thread, func, arg, stack, stackSize, prio)
#define OS_WakeupThreadDirect(thread) PORT_WakeupThreadDirect(thread)
#define OS_SleepThreadDirect(thread, queue) PORT_SleepThreadDirect(thread, queue)
#define OS_DestroyThread(thread) PORT_DestroyThread(thread)
#define OS_JoinThread(thread) PORT_JoinThread(thread)
#define OS_LockMutex(mutex) PORT_LockMutex(mutex)
#define OS_ReceiveMessage(mq, msg, flags) PORT_ReceiveMessage(mq, msg, flags)
#define OS_SendMessage(mq, msg, flags) PORT_SendMessage(mq, msg, flags)
#define OS_SetIrqFunction(intrBit, function) PORT_SetIrqFunction(intrBit, function)
#define OS_ResetSystem(parameter) PORT_ResetSystem(parameter)
#define OS_GetTick() PORT_GetTick()
#endif

#endif // BWPORT_PORT_RUNTIME_H
