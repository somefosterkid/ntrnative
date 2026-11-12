// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/os.h, the lines in use so
// far.
#ifndef BWPORT_NITRO_OS_H
#define BWPORT_NITRO_OS_H

#include "types.h"
#include "stdarg.h"
#include "stddef.h"
#include "nitro/misc.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/os/common/interrupt.h"
#include "nitro/os/common/timer.h"
#include "nitro/pxi.h"
#include "nitro/cp.h"
#include "struct_decls.h"

#define OS_IE_V_BLANK 0x1
#define OS_IE_H_BLANK 0x2

static inline u32 OS_GetVBlankCount(void) {
    return *(vu32 *)HW_VBLANK_COUNT_BUF;
}

// NitroSDK's types: the interrupt state (system.h), the interrupt mask (interrupt.h), the system timer's ticks
// (tick.h), an OS heap (alloc.h), the processor's mode (system.h) and the memory arenas (arena.h)
typedef u32 OSIntrMode;
typedef enum {
    OS_INTRMODE_IRQ_DISABLE = 0x80,
    OS_INTRMODE_IRQ_ENABLE = 0
} OSIntrMode_Irq;
typedef enum {
    OS_INTRMODE_FIQ_DISABLE = 0x40,
    OS_INTRMODE_FIQ_ENABLE = 0
} OSIntrMode_Fiq;
typedef enum {
    OS_PROCMODE_USER = 0x10,
    OS_PROCMODE_FIQ = 0x11,
    OS_PROCMODE_IRQ = 0x12,
    OS_PROCMODE_SVC = 0x13,
    OS_PROCMODE_ABORT = 0x17,
    OS_PROCMODE_UNDEF = 0x1b,
    OS_PROCMODE_SYS = 0x1f
} OSProcMode;
typedef u32 OSIrqMask;
typedef u64 OSTick;
typedef int OSHeapHandle;
typedef enum {
    OS_ARENA_MAIN = 0,
    OS_ARENA_MAIN_SUBPRIV = 1,
    OS_ARENA_MAINEX = 2,
    OS_ARENA_ITCM = 3,
    OS_ARENA_DTCM = 4,
    OS_ARENA_SHARED = 5,
    OS_ARENA_WRAM_MAIN = 6,
    OS_ARENA_WRAM_SUB = 7,
    OS_ARENA_WRAM_SUBPRIV = 8,
    OS_ARENA_MAX = 9
} OSArenaId;
typedef void (*OSIrqFunction)(void);
typedef void (*OSIrqCallback)(void *arg);
typedef void (*OSAlarmHandler)(void *arg);

// The tick count of the system timer, which swan names OS_GetTick
OSTick OS_GetTick(void);
OSIntrMode OS_DisableInterrupts_IrqAndFiq(void);
OSIntrMode OS_RestoreInterrupts_IrqAndFiq(OSIntrMode state);
// The system OS_GetTick, and NitroSDK's OS_TicksToSeconds: the system timer ticks every 64 cycles of it
#define OS_SYSTEM_CLOCK 33514000
#define OS_TicksToSeconds(tick) (((tick) * 64) / OS_SYSTEM_CLOCK)
// NitroSDK's OS_WaitIrq, under swan's name
void OS_WaitIrq(BOOL clear, u32 interrupts);
void OS_WaitVBlankIntr(void);
// Stops the game after a fatal error, calling the registered handler first
void OS_Terminate(void);
// Writes a range of the data cache back to memory, before DMA reads it. NitroSDK's DC_FlushRange
void DC_FlushRange(const void *addr, u32 size);
// Writes the whole data cache back to memory and invalidates it. NitroSDK's DC_FlushAll
void DC_FlushAll(void);

// The start of DTCM, where the linker places the DTCM module
extern u32 SDK_AUTOLOAD_DTCM_START[];
#define HW_DTCM ((u32)SDK_AUTOLOAD_DTCM_START)

// Memory arenas and OS heaps, under swan's names: OS_GetArenaLo, OS_GetArenaHi, OS_SetArenaLo, OS_SetArenaHi,
// OS_AllocFromArenaLo, OS_InitAlloc, OS_CreateHeap, OS_AllocFromHeap and OS_FreeToHeap
#define OS_HEAP_INVALID (-1)

void *OS_GetArenaLo(OSArenaId id);
void *OS_GetArenaHi(OSArenaId id);
void OS_SetArenaLo(OSArenaId id, void *start);
void OS_SetArenaHi(OSArenaId id, void *end);
void *OS_GetInitArenaHi(OSArenaId id);
void *OS_GetInitArenaLo(OSArenaId id);
void func_0273126c_unk(u32 ndmaNo, OSIrqCallback callback, void *arg);
void OSi_InitArenaExTwl(void);
void *OS_AllocFromArenaLo(OSArenaId id, u32 size, u32 alignment);
void *OS_InitAlloc(OSArenaId id, void *start, void *end, int maxHeaps);
OSHeapHandle OS_CreateHeap(OSArenaId id, void *start, void *end);
// TwlSDK's: makes a heap of the arena in the extended main memory, with the same handle as the other heaps
OSHeapHandle OS_CreateExtraHeap(OSArenaId id);
void *OS_AllocFromHeap(OSArenaId id, OSHeapHandle heap, u32 size);
void OS_FreeToHeap(OSArenaId id, OSHeapHandle heap, void *ptr);
// The size of a block of a heap of the arena (NitroSDK's OS_ReferentSize)
u32 OS_ReferentSize(OSArenaId id, void *ptr);

// NitroSDK's OS_DisableInterrupts and OS_RestoreInterrupts, under swan's names
OSIntrMode OS_DisableInterrupts(void);
OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
void exit(int status);

// This machine's MAC address, 6 bytes
void OS_GetMacAddress(u8 *macAddress);

#define OS_TicksToMilliSeconds(tick) (((tick) * 64) / (OS_SYSTEM_CLOCK / 1000))
#define OS_TicksToMicroSeconds(tick) (((tick) * 64 * 1000) / (OS_SYSTEM_CLOCK / 1000))

// The interrupt flags that OS_WaitIrq checks, at the end of DTCM
#define HW_INTR_CHECK_BUF (HW_DTCM + 0x3ff8)

#define OS_IE_CARD_DATA 0x80000

// The DSi's new DMA channel 1
#define OS_IE_NDMA1 0x20000000

// NitroSDK's OS_GetLockID and OS_ReleaseLockID, under swan's names
#define OS_LOCK_ID_ERROR (-3)

// The DSi's parental controls, of its settings: TwlSDK's OSTWLParentalControl (twl/os/common/ownerInfoEx.h), its
// first word of flags (whether they are set, then what each restricts); the rating and password that follow are unused
typedef struct OSTWLParentalControl {
    struct {
        u32 isSetParentalControl : 1;
        u32 pictoChat : 1;
        u32 dsDownload : 1;
        u32 browser : 1;
        u32 prepaidPoint : 1;
        u32 photoExchange : 1;
        u32 ugc : 1;
        u32 rsv : 25;
    } flags;
} OSTWLParentalControl;

// The DSi's settings block; NULL on a DS
OSTWLParentalControl *OS_GetParentalControlInfoPtr(void);

// The buttons the ARM7 reads, X, Y and the lid among them, in shared memory. NitroSDK's PAD_DetectFold
#define HW_BUTTON_XY_BUF 0x02ffffa8

#define PAD_DETECT_FOLD_MASK 0x8000
#define PAD_DETECT_FOLD_SHIFT 15

// The DS's owner settings
#define OS_OWNERINFO_NICKNAME_MAX 10

#define OS_OWNERINFO_COMMENT_MAX 26

typedef struct {
    u8 month;
    u8 day;
} OSBirthday;

typedef struct {
    u8 language;
    u8 favoriteColor;
    OSBirthday birthday;
    u16 nickName[OS_OWNERINFO_NICKNAME_MAX + 1];
    u16 nickNameLength;
    u16 comment[OS_OWNERINFO_COMMENT_MAX + 1];
    u16 commentLength;
} OSOwnerInfo;

// TwlSDK's owner settings, which add the country
typedef struct {
    u8 language;
    u8 favoriteColor;
    OSBirthday birthday;
    u16 nickName[OS_OWNERINFO_NICKNAME_MAX + 1];
    u16 nickNameLength;
    u16 comment[OS_OWNERINFO_COMMENT_MAX + 1];
    u16 commentLength;
    u8 country;
} OSOwnerInfoEx;

// The NVRAM user info the DSi reads its country and language from, the fields by offset
typedef struct {
    u8 unk_0[5];
    u8 country;
    u8 language;
} OSNvramUserInfo;

// The main memory's system work (TwlSDK's OSSystemWork), the parts the owner settings copy, the rest by offset
typedef struct {
    u8 unk_0[2];
    u8 favoriteColor : 4;
    u8 unk_2_4 : 4;
    OSBirthday birthday;
    u8 unk_5;
    u16 nickName[OS_OWNERINFO_NICKNAME_MAX];
    u8 nickNameLength;
    u8 unk_1b;
    u16 comment[OS_OWNERINFO_COMMENT_MAX];
    u8 commentLength;
    u8 unk_51[0x64 - 0x51];
    u16 language : 3; // the DS's owner language, in its low three bits
    u16 unk_64_3 : 13;
    u8 unk_66[0x68 - 0x66];
    s64 rtcOffset; // the RTC offset the owner set, in seconds
    u8 unk_70[0x17c - 0x70];
    OSNvramUserInfo *nvramUserInfo;
} OSSystemWork;

// The TWL settings data at the pointer in main memory (libntr's OSTWLSettingsData): bit 24 of its first word is the
// EULA agreed flag
typedef struct {
    u32 unk_0;
} OSTWLSettingsData;

// NitroSDK's message queues: a ring of messages, with the threads waiting to send and to receive
typedef void *OSMessage;

#define OS_MESSAGE_NOBLOCK 0

typedef struct OSThreadQueue {
    struct OSThread *head;
    struct OSThread *tail;
} OSThreadQueue;

typedef struct OSMessageQueue {
    OSThreadQueue queueSend;
    OSThreadQueue queueReceive;
    OSMessage *msgArray;
    s32 msgCount;
    s32 firstIndex;
    s32 usedCount;
} OSMessageQueue;

// NitroSDK's threads, under swan's names: OS_CreateThread, whose stack is its top, OS_SleepThread, which sleeps on
// queue until OS_WakeupThread wakes it, and OS_WakeupThreadDirect, which starts a new thread. The fields past the
// context and the state are NitroSDK's (pokebw2's names): the link of the list of all threads, the mutex OS_LockMutex
// waits for, the specific words (NitroWiFi's CPS keeps a thread's socket in the first)

// The neighbors of a thread in the queue it sleeps in, and the list of the mutexes that a thread has
typedef struct OSThreadLink {
    struct OSThread *prev;
    struct OSThread *next;
} OSThreadLink;

typedef struct OSMutexQueue {
    struct OSMutex *head;
    struct OSMutex *tail;
} OSMutexQueue;

// A thread's destructor runs in the thread, when it ends, with the value that it ended with
typedef void (*OSThreadDestructor)(void *arg);

typedef struct OSThread {
    u8 unk_00[0x68];
    struct OSThread *next;
    u32 id;
    u32 priority;
    void *profiler;
    OSThreadQueue *queue;
    OSThreadLink link;
    struct OSMutex *mutex;
    OSMutexQueue mutexQueue;
    u32 stackTop;
    u32 stackBottom;
    u32 stackWarningOffset;
    OSThreadQueue joinQueue;
    void *specific[3];
    struct OSAlarm *alarmForSleep;
    OSThreadDestructor destructor;
    void *userParameter;
    int systemErrno;
} OSThread;

// NitroSDK's mutexes, which a thread may lock again; OS_LockMutex sleeps until the mutex is free
typedef struct OSMutexLink {
    struct OSMutex *next;
    struct OSMutex *prev;
} OSMutexLink;

typedef struct OSMutex {
    OSThreadQueue queue;
    OSThread *thread;
    s32 count;
    OSMutexLink link;
} OSMutex;

// Waits for one of the interrupts, after clearing their flags if clear is set. NitroSDK's OS_WaitIrq, under swan's
// name
void OS_WaitInterrupt(BOOL clear, u32 interrupts);
// NitroSDK's STD_GetStringLength
int STD_GetStringLength(const char *str);
BOOL OS_IsRunOnTwl(void);
OSIrqMask OS_EnableIrqMask(OSIrqMask mask);
void OS_GetOwnerInfo(OSOwnerInfo *info);
void OS_GetOwnerInfoEx(OSOwnerInfoEx *info);
// The RTC offset the owner set in the settings, in seconds, from the shared main memory
s64 OS_GetOwnerRtcOffset(void);
void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
void OS_WakeupThreadDirect(OSThread *thread);
// NitroSDK's OS_IsThreadTerminated, under swan's name, and OS_DestroyThread, by its code
BOOL OS_IsThreadTerminated(const OSThread *thread);

// The ROM header of the running program, which the card puts at the end of main memory. Only the fields that
// NitroSDK's multiboot library reads are named
typedef struct ROMHeader {
    u8 unk_00[0x12];
    u8 unitCode;
    u8 unk_13[0xd];
    u32 arm9RomOffset;
    u32 arm9Entry;
    u32 arm9RamAddr;
    u32 arm9Size;
    u32 arm7RomOffset;
    u32 arm7Entry;
    u32 arm7RamAddr;
    u32 arm7Size;
    u8 unk_40[0x20];
    u32 unk_60;
    u8 unk_64[0x1c];
    u32 romSize;
    u8 unk_84[0xdc];
} ROMHeader;

// An empty function that the program's autoload step calls
void AutoloadCallback(void);

// NitroSDK's OS_LockCard and OS_UnlockCard
s32 OS_LockCard(u16 lockId);
s32 OS_UnLockCard(u16 lockId);

static inline BOOL PAD_DetectFold(void) {
    return (*(vu16 *)HW_BUTTON_XY_BUF & PAD_DETECT_FOLD_MASK) >> PAD_DETECT_FOLD_SHIFT;
}

#define PAD_ALL_MASK 0x2fff

// NitroSDK's PAD_Read: the keys and buttons pressed, X and Y from shared memory
static inline u16 PAD_Read(void) {
    return (u16)(((reg_PAD_KEYINPUT | *(vu16 *)HW_BUTTON_XY_BUF) ^ PAD_ALL_MASK) & PAD_ALL_MASK);
}

#define OS_MESSAGE_BLOCK 1

// NitroSDK's events, by their code: a word of flags, and the threads waiting for some of them to be set
typedef struct OSEvent {
    vu32 flag;
    OSThreadQueue queue;
} OSEvent;

#define OS_EVENT_MODE_AND 0
#define OS_EVENT_MODE_OR 1

// NitroSDK's OS_Sleep: waits for some milliseconds, letting other threads run
void OS_Sleep(u32 msec);

// Restarts the game
void OS_ResetSystem(u32 parameter);

// The console type (os_emulator.c): its low bits the hardware, its higher ones a development or debugging unit
u32 OS_GetConsoleType(void);
// How the game was started: 1 from a card (TwlSDK's OS_BOOTTYPE_ROM)
u16 OS_GetBootType(void);
// The DSi's owner settings (os_ownerInfoEx.c): whether wireless is on, the EULA, the parental controls
s32 OS_IsAvailableWireless(void);
s32 OS_IsForceDisableWireless(void);
BOOL OS_IsAgreeEULA(void);
u32 OS_GetAgreedEULAVersion(void);
u32 OS_GetROMHeaderEULAVersion(void);
// The colors of the owner's favorite color setting, one by setting
const u16 *OS_GetFavoriteColorTable(void);

static inline void OS_SetIrqCheckFlag(u32 interrupts) {
    *(vu32 *)HW_INTR_CHECK_BUF |= interrupts;
}

// Whether the system timer was started, and starting it
BOOL OS_IsTickAvailable(void);
void OS_InitTick(void);

// pokebw2's OS_GetLowEntropyData
// Fills 32 bytes with values that differ from run to run
#define OS_LOW_ENTROPY_DATA_SIZE 32
void OS_GetLowEntropyData(u32 buffer[OS_LOW_ENTROPY_DATA_SIZE / sizeof(u32)]);

// pokebw2's OS_LockMutex
void OS_LockMutex(OSMutex *mutex);
// pokebw2's OS_UnlockMutex
void OS_UnlockMutex(OSMutex *mutex);
// pokebw2's OS_InitMutex
void OS_InitMutex(OSMutex *mutex);
void OS_DestroyThread(OSThread *thread);

// Text formatting. TwlSDK's OS_ functions (os_printf.c) call STD_TVSNPrintf (std_sprintf.c), which has the code
int OS_SPrintf(char *dst, const char *fmt, ...);
int OS_VSPrintf(char *dst, const char *fmt, va_list vlist);
int OS_SNPrintf(char *dst, u32 len, const char *fmt, ...);
int OS_VSNPrintf(char *dst, u32 len, const char *fmt, va_list vlist);
int STD_TSPrintf(char *dst, const char *fmt, ...);
int STD_TSNPrintf(char *dst, u32 len, const char *fmt, ...);
int STD_TVSNPrintf(char *dst, size_t len, const char *fmt, va_list vlist);

#define OS_MilliSecondsToTicks(msec) ((u64)(((OS_SYSTEM_CLOCK / 1000) * (u64)(msec)) / 64))

// NitroSDK's alarms, by their code: OS_CreateAlarm, OS_SetAlarm (the alarm fires once after the ticks and calls the
// function with the argument) and OS_CancelAlarm
typedef struct OSAlarm {
    u8 unk_00[0x2c];
} OSAlarm;

void SVC_WaitByLoop(u32 cycles);
void CpuSet(const void *src, void *dest, u32 mode);

// Stops the CPU until an interrupt, which swan names OS_Halt. NitroSDK's OS_Halt
void OS_Halt(void);
// Discards a range of the data cache, after DMA writes it. NitroSDK's DC_InvalidateRange
void DC_InvalidateRange(void *addr, u32 size);
// Writes a range of the data cache back to memory, keeping it cached. NitroSDK's DC_StoreRange
void DC_StoreRange(const void *addr, u32 size);
// Discards a range of the instruction cache, or all of it. NitroSDK's IC_InvalidateRange and IC_InvalidateAll
void IC_InvalidateRange(void *addr, u32 size);
void IC_InvalidateAll(void);
// NitroSDK's OS_GetLockID: a lock ID for OS_LockCard and the like, or OS_LOCK_ID_ERROR
s32 OS_GetLockID(void);
void OS_ReleaseLockID(u16 lockId);
BOOL OS_SendMessage(OSMessageQueue *queue, OSMessage message, s32 flags);
BOOL OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
void OS_SleepThread(OSThreadQueue *queue);
void OS_WakeupThread(OSThreadQueue *queue);
// NitroSDK's OSi_ThreadInfo, the scheduler's state (swan's g_IsThreadSwitchRequested): whether a thread
// switch was asked for while the scheduler could not do it, in an interrupt handler or while it was locked; the depth
// of the interrupts that are being handled; the thread that runs; the list of all threads, linked through their next,
// sorted by priority; and the function that is called with the old and the new thread at each switch
typedef void (*OSSwitchThreadCallback)(OSThread *from, OSThread *to);

typedef struct OSThreadInfo {
    u16 isNeedRescheduling;
    u16 irqDepth;
    OSThread *current;
    OSThread *list;
    OSSwitchThreadCallback switchCallback;
} OSThreadInfo;
extern OSThreadInfo OSi_ThreadInfo;

// Sets flags and wakes the threads waiting on the event (OS_SignalEvent's code)
void OS_SignalEvent(OSEvent *event, u32 setPattern);
// Clears an event's flags and its queue (OS_InitEvent's code)
void OS_InitEvent(OSEvent *event);
// Sleeps until all or any (mode) of the flags of pattern are set, then clears the flags of clearBits and returns the
// flags that were set (OS_WaitEventEx's code)
u32 OS_WaitEventEx(OSEvent *event, u32 pattern, u32 mode, u32 clearBits);

// Sets up a queue that holds count messages in the array, by OS_InitMessageQueue's code
void OS_InitMessageQueue(OSMessageQueue *queue, OSMessage *messages, s32 count);
// Whether a bit of a word of the shared memory is set, which the other processor sets for a FIFO tag once it is
// ready for it
BOOL PXI_IsCallbackReady(int fifoTag, PXIProc proc);

static inline void OS_InitThreadQueue(OSThreadQueue *queue) {
    queue->head = queue->tail = NULL;
}

BOOL OS_ReadMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags); // The first message, left in the queue

// TwlSDK 5's OS_SpinWaitSysCycles: waits for system clock cycles (twice the DS's on a DSi at double speed), by
// looping OS_SpinWaitCpuCycles
void OS_SpinWaitCpuCycles(u32 cycle);
void OS_SpinWaitSysCycles(u32 cycle);
OSIrqMask OS_DisableIrqMask(OSIrqMask mask);
OSIrqMask OS_SetIrqMask(OSIrqMask mask);

// pokebw2's CPU_GetMode
// NitroSDK's OS_GetProcMode: the processor mode, 0x12 in an IRQ handler
OSProcMode OS_GetProcMode(void);

// NitroSDK's OS_JoinThread, by its code: waits for a thread's end
void OS_JoinThread(OSThread *thread);
// OS_DisableScheduler and OS_EnableScheduler's roles
u32 OS_DisableScheduler(void);
u32 OS_EnableScheduler(void);
// NitroSDK's OS_RescheduleThread, by its place before OS_YieldThread and by its use after OS_EnableScheduler: runs the
// highest priority thread that is ready
void OS_RescheduleThread(void);

// NitroSDK's STD_CopyLString: copies at most size - 1 characters and a NUL, returning the length of src
int STD_CopyLString(char *dst, const char *src, int size);

// NitroSDK's OS_TryLockMutex: TRUE when it got the mutex, without waiting
BOOL OS_TryLockMutex(OSMutex *mutex);

// NitroSDK's OS_GetCurrentThread
static inline OSThread *OS_GetCurrentThread(void) {
    return OSi_ThreadInfo.current;
}

// NitroSDK's OS_GetThreadId
static inline u32 OS_GetThreadId(const OSThread *thread) {
    return thread->id;
}

// Does nothing: a library calls it with its signature ("[SDK+NINTENDO:WiFi3.3.30052.201006221129]"), which keeps the
// string in the link (pokebw2's prototype)
void OSi_ReferSymbol(const char *signature);

// NitroSDK's OS_YieldThread, by its place between OS_JoinThread and OS_SetThreadPriority: lets the other threads of the
// same priority run
void OS_YieldThread(void);
void OS_CreateAlarm(OSAlarm *alarm);
void OS_CancelAlarm(OSAlarm *alarm);
// Sets and gets a thread's priority, and ends the running thread (which does not return)
BOOL OS_SetThreadPriority(OSThread *thread, u32 priority);
void OS_SetAlarm(OSAlarm *alarm, OSTick tick, OSAlarmHandler handler, void *arg);

u32 OS_GetThreadPriority(const OSThread *thread);

// Whether OS_InitAlarm has run (os_alarm.c); the alarms on a V-count (os_valarm.c)
BOOL OS_IsAlarmAvailable(void);
typedef struct OSVAlarm OSVAlarm;
typedef void (*OSVAlarmHandler)(void *arg);
void OS_CreateVAlarm(OSVAlarm *alarm);
void OS_SetVAlarm(OSVAlarm *alarm, s16 count, s16 delay, OSVAlarmHandler handler, void *arg);
// Clears interrupt requests, returning the ones there were (os_interrupt.c)
OSIrqMask OS_ResetRequestIrqMask(OSIrqMask intr);
// The DTCM's address (os_tcm.c), and the data cache's lines unlocked (os_cache.c)
u32 OS_GetDTCMAddress(void);
void DC_WaitWriteBufferEmpty(void);

// Black's 0x02086368 in os_init.c, which OS_Init calls; OS_InitAlarm, OS_SetIrqFunction, OS_EnableInterrupts, and
// TwlSDK's OS_SleepThreadDirect, which OS_WakeupThreadDirect undoes. Black 2's prototypes, unchecked against Black's code
void OS_Init(void);
void OS_InitAlarm(void);
void OS_SetIrqFunction(OSIrqMask interrupt, OSIrqFunction function);
OSIntrMode OS_EnableInterrupts(void);
void OS_SleepThreadDirect(OSThread *thread, OSThreadQueue *queue);

// Where a ROM header's program is loaded in RAM, and its size. NitroSDK's multiboot library reads the pair through a
// pointer to it
typedef struct ROMHeaderLoad {
    u32 ramAddr;
    u32 size;
} ROMHeaderLoad;

// The boot type that the card's boot code leaves in the shared memory (NitroSDK's OS_GetBootType): 1 when the game
// booted from the ROM of a cartridge, 2 when it was downloaded as a DS Download Play child
#define HW_BOOT_CHECK_INFO_BUF 0x02fffc40

#define OS_BOOTTYPE_ROM 1
#define OS_BOOTTYPE_DOWNLOAD_MB 2
#define OS_BOOTTYPE_NAND 3 // TwlSDK's: started from the DSi's flash

// The word that OS_ResetSystem leaves for the next program
#define HW_RESET_PARAMETER_BUF 0x02fffc20

// The commands that the ARM7 and the ARM9 send each other with the OS's tag of the PXI FIFO (PXI_FIFO_TAG_OS), in the
// bits 8 to 14
#define OS_PXI_COMMAND_MASK 0x7f00

#define OS_PXI_COMMAND_SHIFT 8
#define OS_PXI_COMMAND_RESET 0x10
#define OS_PXI_COMMAND_TERMINATE 0x20

// NitroSDK's OS_SetProtectionRegion (os_protectionRegion.c): sets the address and size of a region
// of the memory protection unit, and enables it. The size is the size field of the region's register in place (bits 1
// to 5, n for 2^(n + 1) bytes): 4 KB is n = 11 and 4 MB is n = 21
#define OS_PROTECTION_SIZE_4KB (11 << 1)

#define OS_PROTECTION_SIZE_4MB (21 << 1)

// The IRQ stack in DTCM: its top, and its end where it would overflow
#define HW_IRQ_STACK (HW_DTCM + 0x3f80)

extern void SDK_IRQ_STACKSIZE(void);
#define HW_IRQ_STACK_SIZE ((u32)SDK_IRQ_STACKSIZE)
#define HW_IRQ_STACK_END (HW_IRQ_STACK - HW_IRQ_STACK_SIZE)
#define HW_SYS_STACK_SIZE ((int)SDK_SYS_STACKSIZE)

// The end of the part of DTCM that the system keeps for itself (the table of interrupt handlers, the queue of threads
// that wait for one and the callbacks' words), where the data of the game would start
#define HW_DTCM_SYSRV_END 0x02fe00c0

// Where each processor's thread info (NitroSDK's OSi_ThreadInfo) is announced in the shared main memory
#define HW_THREADINFO_MAIN 0x02ffffa0

#define HW_MAIN_MEM_SHARED_WORK 0x02fffc00
#define OS_GetSystemWork() ((OSSystemWork *)HW_MAIN_MEM_SHARED_WORK)
#define OS_IE_V_COUNT 0x4
#define OS_IE_TIMER0 0x8
#define OS_IE_TIMER1 0x10

// The timers' bits start here, and the DMA channels' and the DSi's new DMA channels', each a bit after the last
#define OS_IE_TIMER0_SHIFT 3

#define OS_IE_DMA0_SHIFT 8
#define OS_IE_NDMA0_SHIFT 28

// The FIFO to the other processor has a word to receive
#define OS_IE_FIFO_RECV 0x40000

#define OS_IE_CARD_IREQ 0x100000

// The geometry engine's command FIFO
#define OS_IE_GXFIFO 0x200000

// The DSi's DSP and camera
#define OS_IE_DSP 0x01000000

#define OS_IE_CAMERA 0x02000000

// The arena's current heap, as a heap handle
#define OS_CURRENT_HEAP_HANDLE (-1)


// NitroSDK's spin locks, which the two processors share through words of the shared main memory. A lock word has the
// flag that OS_LockByWord swaps the locker's ID into and the ID of the one who has it. The cartridge and the card have
// one each, and their ctrl functions give the bus to the ARM9 when it locks and back to the ARM7 when it unlocks
typedef struct OSLockWord {
    vu32 lockFlag;
    u16 ownerID;
    u16 extension;
} OSLockWord;

#define HW_INIT_LOCK_BUF 0x02ffffb0
#define HW_CARD_LOCK_BUF 0x02ffffe0
#define HW_CTRDG_LOCK_BUF 0x02ffffe8
#define OS_UNLOCK_SUCCESS 0
#define OS_UNLOCK_ERROR (-2)

// NitroSDK's OS_GetConsoleType and OS_IsRunOnEmulator (the console type has the emulator's bit set), and a function
// that calls OS_IsRunOnTwl
#define OS_CONSOLE_ENSATA 0x10000000

// The bits of a development console's hardware, 0 on a retail one (NitroSDK's OS_CONSOLE_DEV_MASK)
#define OS_CONSOLE_DEV_MASK 0x03000000

// The user settings that the ARM7 reads from the firmware into the shared memory (NitroSDK's NVRAMConfig), with the
// console's MAC address after them. Of the settings, the version and the alarm are not named
#define HW_NVRAM_USER_INFO 0x02fffc80

typedef struct NVRAMOwnerInfo {
    u8 favoriteColor : 4;
    u8 : 4;
    OSBirthday birthday;
    u8 unk_03;
    u16 nickName[OS_OWNERINFO_NICKNAME_MAX];
    u8 nickNameLength;
    u8 unk_19;
    u16 comment[OS_OWNERINFO_COMMENT_MAX];
    u8 commentLength;
    u8 unk_4f;
} NVRAMOwnerInfo;

// The touch screen's calibration: the raw values of two points, and where they are on the screen
typedef struct NVRAMTouchCalibration {
    u16 rawX1;
    u16 rawY1;
    u8 screenX1;
    u8 screenY1;
    u16 rawX2;
    u16 rawY2;
    u8 screenX2;
    u8 screenY2;
} NVRAMTouchCalibration;

typedef struct NVRAMConfig {
    u16 unk_00;
    NVRAMOwnerInfo owner;
    u8 unk_52[6]; // the alarm
    NVRAMTouchCalibration touch;
    u16 language : 3;
    u16 unk_64_3 : 13;
    u16 unk_66;
    s64 rtcOffset;
    u32 unk_70;
    u8 macAddress[6];
} NVRAMConfig;

// NitroSDK's contexts: the registers of a thread that does not run, with the divider's and the square root unit's. They
// are hand-written in assembly (os_context.c): OS_InitContext sets one up to run from newpc with the stack at newsp,
// OS_SaveContext stores the registers (it returns 0, and 1 when the context is loaded) and OS_LoadContext, which is
// NitroSDK's OS_LoadContext, continues from one
typedef struct OSContext {
    u32 cpsr;
    u32 r[13];
    u32 sp;
    u32 lr;
    u32 pc_plus4;
    u32 sp_svc;
    CPContext cp_context;
} OSContext;

// NitroSDK's threads: OS_CreateThread, whose stack is its top, OS_SleepThread, which sleeps on
// queue until OS_WakeupThread wakes it, and OS_WakeupThreadDirect, which starts a new thread. The priority is smaller
// for the more urgent threads: 0 to 31, the launcher thread that the game's main runs in has 16 and the idle thread 32
#define OS_THREAD_PRIORITY_MAX 31

#define OS_THREAD_LAUNCHER_PRIORITY 16

typedef enum OSThreadState {
    OS_THREAD_STATE_WAITING = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_TERMINATED = 2,
} OSThreadState;

// The numbers at the ends of a stack that the stack overflow checks look for: at the beginning of a stack, 8 bytes
// under the address that a thread is given (4 under the IRQ stack's), and at the end where it would overflow
#define OS_STACK_BEGIN_MAGIC 0xfddb597d

#define OS_STACK_END_MAGIC 0x7bf9dd5b

// NitroSDK's interrupts. The table of the 32 interrupt handlers (OS_IRQTable) and the queue of the threads that sleep
// in OS_WaitIrq (OSi_IrqThreadQueue) are in DTCM. The DMA channels, the DSi's new DMA channels and the timers (indexes
// 0 to 3, 4 to 7 and 8 to 11) have a callback and its argument in OSi_IrqCallbackInfo instead: their handlers call
// OSi_IrqCallback with the index. enable says whether the interrupt stays enabled after the callback
#define OSi_IRQCALLBACK_NUM 12
#define OSi_IRQCALLBACK_NO_DMA0 0
#define OSi_IRQCALLBACK_NO_NDMA0 4
#define OSi_IRQCALLBACK_NO_TIMER0 8

// The block of arguments that a program leaves in the main memory for the one that starts next (os_deliverArg.c, after
// TwlSDK's DeliverArg API): who wrote it, a CRC16 of the block, and the arguments as a list of strings that ends with
// an empty one
typedef struct {
    u64 titleId;
    u8 reserved1;
    u8 flag;
    u16 makerCode;
    u16 argBufferSize;
    u16 binarySize;
    u16 crc;
    u16 sysParam;
} OSDeliverArgHeader;

typedef struct OSDeliverArgInfo {
    OSDeliverArgHeader header;
    u8 buf[0x2ec];
} OSDeliverArgInfo;

struct OSVAlarm {
    OSVAlarmHandler handler;
    void *arg;
    u32 tag;
    u32 frame;
    s16 vcount;
    s16 delay;
    struct OSVAlarm *prev;
    struct OSVAlarm *next;
    BOOL periodic;
    u32 unk_20;
    BOOL canceled;
};

// NitroSDK's OSi_SetTimerReserved: marks a hardware timer as taken by the OS
void OSi_SetTimerReserved(int timerNo);
void OS_InitReset(void);
// The loop that OS_Terminate ends in, which disables the interrupts and halts the CPU for good. The name is a guess at
// TwlSDK's; swan's is abort
void OSi_TerminateCore(void);
// NitroSDK's OSi_DoResetSystem, in ITCM: the function that OS_ResetSystem ends in
void OSi_DoResetSystem(void);
// NitroSDK's OS_EnableProtectionUnit and OS_DisableProtectionUnit: switch the memory protection
// unit
void OS_EnableProtectionUnit(void);  // swan's cp15_mmu_enable
void OS_DisableProtectionUnit(void); // swan's cp15_mmu_disable
// NitroSDK's OS_InitArena, which sets the limits of the arenas, and OS_InitArenaEx, which on a DSi also protects the
// main memory that a DS does not have
void OS_InitArena(void);
void OS_InitArenaEx(void);
void OS_InitVAlarm(void);
// NitroSDK's OSi_InitVramExclusive, and OSi_UnlockVram: gives back the banks of the mask (a bit for each of A to I)
// that the lock ID has
void OSi_InitVramExclusive(void);
void OSi_UnlockVram(u16 bank, u16 lockId);
// NitroSDK's OS_LockByWord, OS_UnlockByWord and OS_TryLockByWord: lock the word with the ID (waiting, with ctrl run
// once it is locked), unlock it (OS_UNLOCK_ERROR if the ID is not its owner) or try to lock it, giving the previous
// flag
s32 OS_LockByWord(u16 lockID, OSLockWord *lockp, void (*ctrl)(void));
s32 OS_UnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrl)(void));
s32 OS_TryLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrl)(void));
// NitroSDK's OS_UnlockCartridge, OS_UnLockCartridge (the same under another spelling) and OS_TryLockCartridge
s32 OS_UnlockCartridge(u16 lockID);
s32 OS_UnLockCartridge(u16 lockID);
s32 OS_TryLockCartridge(u16 lockID);
s32 OS_UnlockCard(u16 lockID);
s32 OS_TryLockCard(u16 lockID);
// NitroSDK's OS_ReadOwnerOfLockWord: the ID of the one who has the lock
u16 OS_ReadOwnerOfLockWord(OSLockWord *lockp);
// NitroSDK's OS_GetCpsrIrq: the CPSR IRQ-disable bit, 0x80 while IRQs are off
OSIntrMode_Irq OS_GetCpsrIrq(void);
BOOL OS_IsRunOnEmulator(void);
// TwlSDK's OSi_IsCodecTwlMode: whether the game runs with the DSi's codec in TWL mode (on a DSi, with the TWL ROM
// header's codec flag set)
BOOL OSi_IsCodecTwlMode(void);
// Sends a message to the front of the queue, where the next OS_ReceiveMessage gets it
BOOL OS_JamMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
void OS_InitContext(OSContext *context, u32 newpc, u32 newsp);
BOOL OS_SaveContext(OSContext *context);
void OS_LoadContext(OSContext *context);
// NitroSDK's OS_InitThread, which makes the thread that runs the launcher thread and creates the idle thread, and
// OS_IsThreadAvailable, which says whether it has run (a flag that other code reads, OSi_ThreadInitialized)
void OS_InitThread(void); // swan's scheduler_init
BOOL OS_IsThreadAvailable(void);
extern BOOL OSi_IsThreadInitialized;
// NitroSDK's OS_SetSwitchThreadCallback, which gives back the one it replaces, and OS_SetThreadDestructor
OSSwitchThreadCallback OS_SetSwitchThreadCallback(OSSwitchThreadCallback callback);
void OS_SetThreadDestructor(OSThread *thread, OSThreadDestructor destructor);
void OS_ExitThread(void); // swan's scheduler_thread_exit_proc
void OS_KillThread(OSThread *thread, void *arg);
// OS_IrqDummy, and the handlers that DTCM's table has for DMA0 to 3, Timer0 to 3 and NDMA0 to 3 (OSi_IrqDma0 to 3,
// OSi_IrqTimer0 to 3 and OSi_IrqNDma0 to 3), in that order
void OS_IrqDummy(void);
void OSi_IrqDma0(void);
void OSi_IrqDma1(void);
void OSi_IrqDma2(void);
void OSi_IrqDma3(void);
void OSi_IrqTimer0(void);
void OSi_IrqTimer1(void);
void OSi_IrqTimer2(void);
void OSi_IrqTimer3(void);
void OSi_IrqNDma0(void);
void OSi_IrqNDma1(void);
void OSi_IrqNDma2(void);
void OSi_IrqNDma3(void);
// NitroSDK's OS_InitIrqTable, which clears the queue of the threads that wait for an interrupt
void OS_InitIrqTable(void);
// NitroSDK's OS_GetIrqFunction: the function of the first interrupt of the mask, or NULL
OSIrqFunction OS_GetIrqFunction(u32 interrupts);
// NitroSDK's OSi_EnterDmaCallback, OSi_EnterNDmaCallback, which tail-calls TwlSDK's body in the LTD
// autoload, and OSi_EnterTimerCallback: set the callback of a DMA channel, new DMA channel or timer, and enable its
// interrupt
void OSi_EnterDmaCallback(u32 dmaNo, void *callback, void *arg);
void func_020848ec(u32 ndmaNo, OSIrqCallback callback, void *arg);
void OSi_EnterTimerCallback(int timerNo, OSIrqCallback callback, void *arg);
void OSi_CommonCallback(int fifoTag, uPtr data, BOOL err);
void OSi_SendToPxi(s32 arg);
// Sets the buffer up with no arguments (leaving reserved bytes of it unused), sends its content to the next program
// and clears it (-1 when it was not set up), takes the block that the last program left into it (0 whether the CRC16
// was right or not, -1 when there is none or no buffer), forgets the buffer, and gives the title ID of the sender
void OS_InitDeliverArg(void *buf, u16 param);
s32 OS_EncodeDeliverArg(void);
s32 OS_DecodeDeliverArg(void);
void OS_FinalizeDeliverArg(void);
u64 OS_GetTitleIdFromDeliverArg(void);
// Takes the first mutex off a thread's list of them and gives it (os_thread.c's OSi_RemoveMutexLinkFromQueue)
OSMutex *OSi_RemoveMutexLinkFromQueue(OSMutexQueue *queue);
// NitroSDK's OSi_UnlockAllMutex: unlocks the mutexes that a thread has, waking the threads that wait for them
void OSi_UnlockAllMutex(OSThread *thread);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/os/ARM9/cache.h, include/nitro/os/common/alloc.h, include/nitro/os/common/arena.h, include/nitro/os/common/event.h, include/nitro/os/common/interrupt.h, include/nitro/os/common/message.h, include/nitro/os/common/reset.h, include/nitro/os/common/thread.h
static inline void DC_CleanAndInvalidateAll (void)
{
	DC_FlushAll();
}
static inline void DC_CleanRange (const void * startAddr, u32 nBytes)
{
	DC_StoreRange(startAddr, nBytes);
}
static inline void DC_CleanAndInvalidateRange (const void * startAddr, u32 nBytes)
{
	DC_FlushRange(startAddr, nBytes);
}
static inline void * OS_AllocFromMain (u32 size)
{
	return OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, size);
}
static inline void OS_FreeToMain (void * ptr)
{
	OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, ptr);
}
static inline void * OS_AllocFromSubPriv (u32 size)
{
	return OS_AllocFromHeap(OS_ARENA_MAIN_SUBPRIV, OS_CURRENT_HEAP_HANDLE, size);
}
static inline void OS_FreeToSubPriv (void * ptr)
{
	OS_FreeToHeap(OS_ARENA_MAIN_SUBPRIV, OS_CURRENT_HEAP_HANDLE, ptr);
}
static inline void * OS_AllocFromMainEx (u32 size)
{
	return OS_AllocFromHeap(OS_ARENA_MAINEX, OS_CURRENT_HEAP_HANDLE, size);
}
static inline void OS_FreeToMainEx (void * ptr)
{
	OS_FreeToHeap(OS_ARENA_MAINEX, OS_CURRENT_HEAP_HANDLE, ptr);
}
    static inline void * OS_AllocFromITCM (u32 size)
    {
        return OS_AllocFromHeap(OS_ARENA_ITCM, OS_CURRENT_HEAP_HANDLE, size);
    }
    static inline void OS_FreeToITCM (void * ptr)
    {
        OS_FreeToHeap(OS_ARENA_ITCM, OS_CURRENT_HEAP_HANDLE, ptr);
    }
    static inline void * OS_AllocFromDTCM (u32 size)
    {
        return OS_AllocFromHeap(OS_ARENA_DTCM, OS_CURRENT_HEAP_HANDLE, size);
    }
    static inline void OS_FreeToDTCM (void * ptr)
    {
        OS_FreeToHeap(OS_ARENA_DTCM, OS_CURRENT_HEAP_HANDLE, ptr);
    }
static inline void * OS_AllocFromShared (u32 size)
{
	return OS_AllocFromHeap(OS_ARENA_SHARED, OS_CURRENT_HEAP_HANDLE, size);
}
static inline void OS_FreeToShared (void * ptr)
{
	OS_FreeToHeap(OS_ARENA_SHARED, OS_CURRENT_HEAP_HANDLE, ptr);
}
static inline void * OS_AllocFromWramMain (u32 size)
{
	return OS_AllocFromHeap(OS_ARENA_WRAM_MAIN, OS_CURRENT_HEAP_HANDLE, size);
}
static inline void OS_FreeToWramMain (void * ptr)
{
	OS_FreeToHeap(OS_ARENA_WRAM_MAIN, OS_CURRENT_HEAP_HANDLE, ptr);
}
static inline void * OS_AllocFromWramSub (u32 size)
{
	return OS_AllocFromHeap(OS_ARENA_WRAM_SUB, OS_CURRENT_HEAP_HANDLE, size);
}
static inline void OS_FreeToWramSub (void * ptr)
{
	OS_FreeToHeap(OS_ARENA_WRAM_SUB, OS_CURRENT_HEAP_HANDLE, ptr);
}
static inline void * OS_AllocFromSubPrivWram (u32 size)
{
	return OS_AllocFromHeap(OS_ARENA_WRAM_SUBPRIV, OS_CURRENT_HEAP_HANDLE, size);
}
static inline void OS_FreeToSubPrivWram (void * ptr)
{
	OS_FreeToHeap(OS_ARENA_WRAM_SUBPRIV, OS_CURRENT_HEAP_HANDLE, ptr);
}
static inline void OS_SetMainArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_MAIN, newHi);
}
static inline void OS_SetMainArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_MAIN, newLo);
}
static inline void * OS_GetMainArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_MAIN);
}
static inline void * OS_GetMainArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_MAIN);
}
static inline void * OS_AllocFromMainArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_MAIN, size, align);
}
static inline void OS_SetSubPrivArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_MAIN_SUBPRIV, newHi);
}
static inline void OS_SetSubPrivArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_MAIN_SUBPRIV, newLo);
}
static inline void * OS_GetSubPrivArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_MAIN_SUBPRIV);
}
static inline void * OS_GetSubPrivArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_MAIN_SUBPRIV);
}
static inline void * OS_AllocFromSubPrivArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_MAIN_SUBPRIV, size, align);
}
static inline void OS_SetMainExArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_MAINEX, newHi);
}
static inline void OS_SetMainExArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_MAINEX, newLo);
}
static inline void * OS_GetMainExArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_MAINEX);
}
static inline void * OS_GetMainExArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_MAINEX);
}
static inline void * OS_AllocFromMainExArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_MAINEX, size, align);
}
static inline void OS_SetITCMArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_ITCM, newHi);
}
static inline void OS_SetITCMArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_ITCM, newLo);
}
static inline void * OS_GetITCMArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_ITCM);
}
static inline void * OS_GetITCMArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_ITCM);
}
static inline void * OS_AllocFromITCMArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_ITCM, size, align);
}
static inline void OS_SetDTCMArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_DTCM, newHi);
}
static inline void OS_SetDTCMArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_DTCM, newLo);
}
static inline void * OS_GetDTCMArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_DTCM);
}
static inline void * OS_GetDTCMArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_DTCM);
}
static inline void * OS_AllocFromDTCMArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_DTCM, size, align);
}
static inline void OS_SetSharedArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_SHARED, newHi);
}
static inline void OS_SetSharedArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_SHARED, newLo);
}
static inline void * OS_GetSharedArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_SHARED);
}
static inline void * OS_GetSharedArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_SHARED);
}
static inline void * OS_AllocFromSharedArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_SHARED, size, align);
}
static inline void OS_SetWramMainArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_WRAM_MAIN, newHi);
}
static inline void OS_SetWramMainArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_WRAM_MAIN, newLo);
}
static inline void * OS_GetWramMainArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_WRAM_MAIN);
}
static inline void * OS_GetWramMainArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_WRAM_MAIN);
}
static inline void * OS_AllocFromWramMainArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_WRAM_MAIN, size, align);
}
static inline void OS_SetWramSubArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_WRAM_SUB, newHi);
}
static inline void OS_SetWramSubArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_WRAM_SUB, newLo);
}
static inline void * OS_GetWramSubArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_WRAM_SUB);
}
static inline void * OS_GetWramSubArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_WRAM_SUB);
}
static inline void * OS_AllocFromWramSubArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_WRAM_SUB, size, align);
}
static inline void OS_SetWramSubPrivArenaHi (void * newHi)
{
	OS_SetArenaHi(OS_ARENA_WRAM_SUBPRIV, newHi);
}
static inline void OS_SetWramSubPrivArenaLo (void * newLo)
{
	OS_SetArenaLo(OS_ARENA_WRAM_SUBPRIV, newLo);
}
static inline void * OS_GetWramSubPrivArenaHi (void)
{
	return OS_GetArenaHi(OS_ARENA_WRAM_SUBPRIV);
}
static inline void * OS_GetWramSubPrivArenaLo (void)
{
	return OS_GetArenaLo(OS_ARENA_WRAM_SUBPRIV);
}
static inline void * OS_AllocFromWramSubPrivArenaLo (u32 size, u32 align)
{
	return OS_AllocFromArenaLo(OS_ARENA_WRAM_SUBPRIV, size, align);
}
static inline void OS_InitArenaHi (OSArenaId id)
{
	OS_SetArenaHi((id), OS_GetInitArenaHi((OSArenaId)id));
}
static inline void OS_InitArenaLo (OSArenaId id)
{
	OS_SetArenaLo((id), OS_GetInitArenaLo(id));
}
static inline u32 OS_WaitEvent_And(OSEvent *event, u32 pattern) {
  return OS_WaitEventEx(event, pattern, OS_EVENT_MODE_AND, 0);
}
static inline u32 OS_WaitEvent_Or(OSEvent *event, u32 pattern) {
  return OS_WaitEventEx(event, pattern, OS_EVENT_MODE_OR, 0);
}
static inline u32 OS_WaitEventEx_And(OSEvent *event, u32 pattern,
                                     u32 clearBit) {
  return OS_WaitEventEx(event, pattern, OS_EVENT_MODE_AND, clearBit);
}
static inline u32 OS_WaitEventEx_Or(OSEvent *event, u32 pattern, u32 clearBit) {
  return OS_WaitEventEx(event, pattern, OS_EVENT_MODE_OR, clearBit);
}
static inline BOOL OS_RestoreIrq (BOOL enable)
{
	u16 prep = reg_OS_IME;
	reg_OS_IME = (u16)enable;
	return (BOOL)prep;
}
static inline OSIrqMask OS_GetIrqMask (void)
{
	return reg_OS_IE;
}
static inline OSIrqMask OS_GetRequestIrqMask (void)
{
	return reg_OS_IF;
}
static inline void OS_ClearIrqCheckFlag (OSIrqMask intr)
{
	*(vu32 *)HW_INTR_CHECK_BUF &= (u32) ~intr;
}
static inline OSIrqMask OS_GetIrqCheckFlag (void)
{
	return *(OSIrqMask *)HW_INTR_CHECK_BUF;
}
static inline void OSi_SetVBlankCount (u32 count)
{
	*(u32 *)HW_VBLANK_COUNT_BUF = count;
}
static inline s32 OS_GetMessageCount (OSMessageQueue * mq)
{
	SDK_ASSERT(mq);
	return mq->msgCount;
}
static inline BOOL OS_IsMessageQueueFull (OSMessageQueue * mq)
{
	SDK_ASSERT(mq);
	return (mq->usedCount >= mq->msgCount);
}
static inline u32 OS_GetResetParameter (void)
{
	return (u32) * (u32 *)HW_RESET_PARAMETER_BUF;
}
static inline OSThread * OS_GetNextThread (const OSThread * thread)
{
    SDK_ASSERT(thread);
    return thread->next;
}
static inline void OS_InitArenaHiAndLo (OSArenaId id)
{
	OS_InitArenaHi(id);
	OS_InitArenaLo(id);
}

// The part of NitroSDK's reset and terminate handling that is Thumb code here (NitroSDK's OS_IsResetOccurred, the
// callback for the OS tag of the PXI FIFO, and the function that sends the ARM7 a command). The callback gets the
// command in the bits 8 to 14 of the word
u16 OS_IsResetOccurred(void);
// The rest of the boot code in ITCM, which OSi_DoResetSystem calls and ends by jumping to OSi_DoBoot with: the check of
// the images that the card loaded against the hashes of the DSi header, the load of the program's images, the card
// read, the word fill and the jump to the program's entry. OSi_DoBoot, OSi_ReadCardRom32 and OSi_CpuClear32 have
// NitroSDK's names; the others are ours, from what their code does
BOOL OSi_VerifyHmac(const void *data, u32 size, const void *hash);
void OSi_VerifyRomBlocks(void);
// pokebw2's OSi_DoBoot
void OSi_StartProgram(void);
// pokebw2's OSi_CpuClear32
void OSi_Clear32(u32 value, void *dest, u32 size);
void OSi_LoadProgram(BOOL twl);
// pokebw2's OSi_ReadCardRom32
void func_01ff8700(u32 address, void *dest, u32 size);
// pokebw2's OSi_IsAddressInMainMemoryEx
BOOL func_02086c70(u32 addr);
// pokebw2's OSi_IsAddressInVideoMemory
BOOL func_02086cbc(u32 addr);

// pokebw2's OS_IrqHandler
// NitroSDK's OS_IrqHandler (swan's CPU_IRQInvoke), in ITCM: the IRQ dispatcher. WCM also uses its address as a special
// mutex-owner marker
void func_01ff80f0(void);
// NitroSDK's OS_IrqHandler_ThreadSwitch, in ITCM: the dispatcher's second half, which an interrupt handler returns
// into, and which switches to the highest-priority ready thread
void OS_IrqHandler_ThreadSwitch(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
extern OSIrqFunction OS_IRQTable[32];
// NitroSDK's OSi_IrqThreadQueue (os_irqHandler_dtcm.c): the threads that sleep in OS_WaitIrq
extern OSThreadQueue OSi_IrqThreadQueue;

// NitroSDK's OSi_SyncWithOtherProc (os_spinLock.c): the handshake with the other processor
void OSi_SyncWithOtherProc(int type, void *syncBuf);

#endif // BWPORT_NITRO_OS_H
