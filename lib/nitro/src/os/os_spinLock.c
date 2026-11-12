// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_spinLock.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro.h>

void _ISDbgLib_Initialize(void);
void _ISDbgLib_AllocateEmualtor(void);
void _ISDbgLib_FreeEmulator(void);

static s32 OSi_DoLockByWord(u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void), BOOL disableFiq);
static s32 OSi_DoUnlockByWord(u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void), BOOL disableFIQ);
static s32 OSi_DoTryLockByWord(u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void), BOOL disableFIQ);

static void OSi_AllocateCartridgeBus(void);
static void OSi_FreeCartridgeBus(void);
static void OSi_FreeCardBus(void);
static void OSi_WaitByLoop(void);
static void func_02084e54(void);

// TwlSDK 5's spinLock.h has these for SDK_TWL; the build's SDK 4 headers lack them
#define OSi_SYNCTYPE_SENDER 0
#define OSi_SYNCTYPE_RECVER 1
void OSi_SyncWithOtherProc(int type, void *syncBuf);

#define OSi_ASSERT_ID(id)       SDK_ASSERTMSG(id >= OS_MAINP_LOCK_ID_START && id <= OS_MAINP_SYSTEM_LOCK_ID, \
                                              "lock ID %d is out of bounds", id)

#define OSi_LOCKID_INITIAL_FLAG_0     0xffffffff
#define OSi_LOCKID_INITIAL_FLAG_1     0xffff0000

    #define OSi_ANYP_LOCK_ID_FLAG  HW_LOCK_ID_FLAG_MAIN
    #define OSi_ANYP_LOCK_ID_START OS_MAINP_LOCK_ID_START

static inline void OSi_WaitByLoop (void)
{
    SVC_WaitByLoop(0x1000 / 4);
}

void OSi_SyncWithOtherProc(int type, void *syncBuf) {
  vu8 *ptr1 = (vu8 *)syncBuf;
  vu8 *ptr2 = (vu8 *)syncBuf + 1;
  vu8 *pfinish = (vu8 *)syncBuf + 2;
  vu8 *pconf = (vu8 *)syncBuf + 3;

  if (type == OSi_SYNCTYPE_SENDER) {
    int n = 0;
    *pfinish = FALSE;
    do {
      *ptr1 = (u8)(0x80 | (n & 0xf));
      while (*ptr1 != *ptr2 && *pfinish == FALSE) {
        OSi_WaitByLoop();
      }
      n++;
    } while (*pfinish == FALSE);
    *pconf = TRUE;
  } else {
    int sum = 0;
    *ptr2 = 0;
    while (sum < 0x300) {
      if (*ptr2 != *ptr1) {
        *ptr2 = *ptr1;
        sum += *ptr2;
      } else {
        OSi_WaitByLoop();
      }
    }
    *pconf = FALSE;
    *pfinish = TRUE;
    while (*pconf == FALSE) {
      OSi_WaitByLoop();
    }
  }
}

void OS_InitLock (void)
{
    static BOOL isInitialized = FALSE;

    if (isInitialized)
        return;

    isInitialized = TRUE;

    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[0] = OSi_LOCKID_INITIAL_FLAG_0;
    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[1] = OSi_LOCKID_INITIAL_FLAG_1;

    MI_CpuClear32((void *)HW_SHARED_LOCK_BUF, HW_CTRDG_LOCK_BUF - HW_SHARED_LOCK_BUF);

    MIi_SetCardProcessor(MI_PROCESSOR_ARM7);
    MIi_SetCartridgeProcessor(MI_PROCESSOR_ARM7);

    OSi_SyncWithOtherProc(OSi_SYNCTYPE_SENDER, (void *)HW_INIT_LOCK_BUF);
    OSi_SyncWithOtherProc(OSi_SYNCTYPE_RECVER, (void *)HW_INIT_LOCK_BUF);
}

static s32 OSi_DoLockByWord (u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void), BOOL disableFiq)
{
    s32 lastLockFlag;

    while ((lastLockFlag = OSi_DoTryLockByWord(lockID, lockp, ctrlFuncp, disableFiq)) > OS_LOCK_SUCCESS) {
        OSi_WaitByLoop();
    }

    return lastLockFlag;
}

s32 OS_LockByWord (u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void))
{
    return OSi_DoLockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

static s32 OSi_DoUnlockByWord(u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void), BOOL disableFIQ)
{
    OSIntrMode lastInterrupts;

    OSi_ASSERT_ID(lockID);

    if (lockID != lockp->ownerID) {
        return OS_UNLOCK_ERROR;
    }

    if (disableFIQ) {
        lastInterrupts = OS_DisableInterrupts_IrqAndFiq();
    } else {
        lastInterrupts = OS_DisableInterrupts();
    }

    lockp->ownerID = 0;
    if (ctrlFuncp) {
        ctrlFuncp();
    }
    lockp->lockFlag = 0;

    if (disableFIQ) {
        OS_RestoreInterrupts_IrqAndFiq(lastInterrupts);
    } else {
        OS_RestoreInterrupts(lastInterrupts);
    }

    return OS_UNLOCK_SUCCESS;
}

s32 OS_UnlockByWord (u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void))
{
    return OSi_DoUnlockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

static s32 OSi_DoTryLockByWord(u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void), BOOL disableFIQ)
{
    s32 lastLockFlag;
    OSIntrMode lastInterrupts;

    OSi_ASSERT_ID(lockID);

    if (disableFIQ) {
        lastInterrupts = OS_DisableInterrupts_IrqAndFiq();
    } else {
        lastInterrupts = OS_DisableInterrupts();
    }

    lastLockFlag = (s32)MI_SwapWord(lockID, &lockp->lockFlag);
    if (lastLockFlag == 0) {
        if (ctrlFuncp) {
            ctrlFuncp();
        }

        lockp->ownerID = lockID;
    }

    if (disableFIQ) {
        OS_RestoreInterrupts_IrqAndFiq(lastInterrupts);
    } else {
        OS_RestoreInterrupts(lastInterrupts);
    }

    return lastLockFlag;
}

s32 OS_TryLockByWord (u16 lockID, OSLockWord * lockp, void (*ctrlFuncp)(void))
{
    return OSi_DoTryLockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

s32 OS_UnlockCartridge (u16 lockID)
{
    s32 lastLockFlag;

    OSi_ASSERT_ID(lockID);

    lastLockFlag = OSi_DoUnlockByWord(lockID, (OSLockWord *)HW_CTRDG_LOCK_BUF, OSi_FreeCartridgeBus, TRUE);

    return lastLockFlag;
}

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): the old name's jump to the new
s32 OS_UnLockCartridge(u16 lockID)
{
    return OS_UnlockCartridge(lockID);
}
#else
asm s32 OS_UnLockCartridge (u16 lockID)
{
    ldr r1, = OS_UnlockCartridge
    bx r1
}
#endif

s32 OS_TryLockCartridge (u16 lockID)
{
    s32 lastLockFlag;

    lastLockFlag = OSi_DoTryLockByWord(lockID, (OSLockWord *)HW_CTRDG_LOCK_BUF, OSi_AllocateCartridgeBus, TRUE);

    return lastLockFlag;
}

static void OSi_AllocateCartridgeBus (void)
{
    MIi_SetCartridgeProcessor(MI_PROCESSOR_ARM9);
}

static void OSi_FreeCartridgeBus(void)
{
    reg_MI_EXMEMCNT |= 0x80;
}

s32 OS_LockCard(u16 lockID)
{
    OSi_ASSERT_ID(lockID);

    return OS_LockByWord(lockID, (OSLockWord *)HW_CARD_LOCK_BUF, func_02084e54);
}


s32 OS_UnlockCard (u16 lockID)
{
    OSi_ASSERT_ID(lockID);

    return OS_UnlockByWord(lockID, (OSLockWord *)HW_CARD_LOCK_BUF, OSi_FreeCardBus);
}

#ifndef __MWERKS__
// C for a PC build: the old name's jump to the new
s32 OS_UnLockCard(u16 lockID)
{
    return OS_UnlockCard(lockID);
}
#else
asm s32 OS_UnLockCard (u16 lockID)
{
    ldr r1, = OS_UnlockCard
    bx r1
}
#endif

s32 OS_TryLockCard(u16 lockID)
{
    OSi_ASSERT_ID(lockID);

    return OS_TryLockByWord(lockID, (OSLockWord *)HW_CARD_LOCK_BUF, func_02084e54);
}

void func_02084e54(void)
{
    if (((*(vu16 *)0x04004010 & 0xc) >> 2) == 2) {
        *(vu32 *)0x040001a4 |= 0x20000000;
    }

    reg_MI_EXMEMCNT = reg_MI_EXMEMCNT & ~0x0800;
}



static void OSi_FreeCardBus (void)
{
    MIi_SetCardProcessor(MI_PROCESSOR_ARM7);
}

u16 OS_ReadOwnerOfLockWord (OSLockWord * lockp)
{
    return lockp->ownerID;
}

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): the first free ID of the two flag
// words (a set bit is free, the top bit the lowest ID), taken
static u32 OSi_LeadingZeros(u32 x)
{
    u32 n = 0;

    if (x == 0) {
        return 32;
    }
    while (!(x & 0x80000000)) {
        x <<= 1;
        n++;
    }
    return n;
}

s32 OS_GetLockID(void)
{
    u32 *flag = (u32 *)OSi_ANYP_LOCK_ID_FLAG;
    s32 id = OSi_ANYP_LOCK_ID_START;
    u32 n = OSi_LeadingZeros(flag[0]);

    if (n == 32) {
        flag++;
        n = OSi_LeadingZeros(flag[0]);
        if (n == 32) {
            return OS_LOCK_ID_ERROR;
        }
        id = OSi_ANYP_LOCK_ID_START + 32;
    }
    *flag &= ~(0x80000000 >> n);
    return id + (s32)n;
}
#else
asm s32 OS_GetLockID (void)
{
    ldr r3, = OSi_ANYP_LOCK_ID_FLAG
    ldr r1, [r3, #0]
    clz r2, r1
    cmp r2, #32
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    bne @movne1
    b @movne2
@movne1:
    mov  r0, #OSi_ANYP_LOCK_ID_START
@movne2:
#else
    movne r0, #OSi_ANYP_LOCK_ID_START
#endif
    bne _1
    add r3, r3, #4
    ldr r1, [r3, #0]
    clz r2, r1
    cmp r2, #32
    ldr r0, = OS_LOCK_ID_ERROR
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    beq @bxeq1
    b @bxeq2
@bxeq1:
    bx lr
@bxeq2:
#else
    bxeq lr
#endif
    mov r0, #OSi_ANYP_LOCK_ID_START + 32
_1:
    add r0, r0, r2
    mov r1, #0x80000000
    mov r1, r1, lsr r2
    ldr r2, [r3, #0]
    bic r2, r2, r1
    str r2, [r3, #0]
    bx lr
}
#endif

#ifndef __MWERKS__
// C for a PC build: the ID's bit set again
void OS_ReleaseLockID(u16 lockID)
{
    u32 *flag = (u32 *)OSi_ANYP_LOCK_ID_FLAG;

    if (lockID >= OSi_ANYP_LOCK_ID_START + 32) {
        flag++;
        lockID -= OSi_ANYP_LOCK_ID_START + 32;
    } else {
        lockID -= OSi_ANYP_LOCK_ID_START;
    }
    *flag |= 0x80000000 >> lockID;
}
#else
asm void OS_ReleaseLockID (register u16 lockID)
{
#pragma unused(lockID)
    ldr r3, = OSi_ANYP_LOCK_ID_FLAG
    cmp r0, #OSi_ANYP_LOCK_ID_START + 32
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    bpl @addpl1
    b @addpl2
@addpl1:
    add r3, r3, #4
@addpl2:
    bpl @subpl1
    b @subpl2
@subpl1:
    sub r0, r0, #OSi_ANYP_LOCK_ID_START+32
@subpl2:
    bmi @submi1
    b @submi2
@submi1:
    sub r0, r0, #OSi_ANYP_LOCK_ID_START
@submi2:
#else
    addpl r3, r3, #4
    subpl r0, r0, #OSi_ANYP_LOCK_ID_START + 32
    submi r0, r0, #OSi_ANYP_LOCK_ID_START
#endif
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3, #0]
    orr r2, r2, r1
    str r2, [r3, #0]
    bx lr
}
#endif

#include <nitro/codereset.h>
