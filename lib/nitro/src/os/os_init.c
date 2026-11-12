// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_init.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>
#include <nitro/mi/wram.h>
#include <nitro/pxi.h>
#include <nitro/spi.h>

    #include <nitro/ctrdg.h>
#include <nitro/card.h>

#include <nitro/code32.h>

// libntr's, as it is: the SDK ships it as assembly
#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): interrupts off (the asm writes
// HW_REG_BASE's value, whose bit 0, IME's only bit, is 0) until the line counter is 0, then IME as it was
static void OSi_WaitVCount0(void)
{
    vu32 *ime = (vu32 *)(HW_REG_BASE + REG_IME_OFFSET);
    u32 prev = *ime;

    *ime = 0;
    while (*(vu16 *)(HW_REG_BASE + REG_VCOUNT_OFFSET) != 0) {
    }
    *ime = prev;
}
#else
static asm void OSi_WaitVCount0 (void)
{
    mov r12, #HW_REG_BASE
    ldr r1, [r12, #REG_IME_OFFSET]
    str r12, [r12, #REG_IME_OFFSET]
@wait_vcount_0:
    ldrh r0, [r12, #REG_VCOUNT_OFFSET]
    cmp r0, #0
    bne @wait_vcount_0
    str r1, [r12, #REG_IME_OFFSET]
    bx lr
}
#endif

#include <nitro/codereset.h>

/* Black's own symbols with no header yet: TwlSDK 5's OS_InitException, the game's CTRDG_Init, MI_InitWramManager,
   SCFG_Init and OSi_InitPrevTitleId. */
void func_02086dd8(void);
void func_02005438(void);
void func_02082198(void);
void func_0208c960(void);
void func_020883d4(void);

/* TwlSDK 5's OS_Init, and its OSi_InitCommon under Black's name */
void OS_Init(void);
void func_02086374(void);

#pragma profile off

void OS_Init (void)
{
  func_02086374();
}

#pragma profile reset

#pragma profile off
void func_02086374(void) {
  PXI_Init();
  OS_InitArena();
  OS_InitLock();
  OS_InitArenaEx();
  OS_InitIrqTable();
  OS_SetIrqStackChecker();
  func_02086dd8();
  MI_Init();
  OS_InitVAlarm();
  OSi_InitVramExclusive();
  OS_InitThread();
  OS_InitReset();
  func_02005438();
  CARD_Init();
  func_02082198();
  if (OS_IsRunOnTwl() == TRUE) {
    func_0208c960();
  }
  PM_Init();
  OSi_WaitVCount0();
  if (OS_IsRunOnTwl() == TRUE) {
    func_020883d4();
  }
}
#pragma profile reset
