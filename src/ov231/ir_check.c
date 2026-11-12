// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov338/ir_check.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "nitro/card.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "system/ir_check.h"

// The infrared chip's command for its ID, and the ID a genuine chip answers with
#define IR_COMMAND_ID 0x08
#define IR_ID 0xaa

// Where the bytes of a transfer come from and go to
typedef struct {
    u32 unk0;
    u8 *send;
    u8 *recv;
    u32 unkC;
} IrTransfer;

static u8 func_ov231_02180820(void);
static void func_ov231_02180904(void);
static void func_ov231_02180914(u16 flags);
static u8 func_ov231_02180924(IrTransfer *transfer);
static void func_ov231_02180984(IrTransfer *transfer);

// pokebw2's sLockId
static u16 data_ov231_02180a00 = 0xfffd;
// pokebw2's sTransfer
// Aligned: MWCC leaves IrCheck_ReadId's 64-bit multiplies by 64 and 1000 apart only when the function reads an object
// with an explicit alignment (pokebw2's ir_check.c and irc_spi.c)
static IrTransfer data_ov231_02180a20 ATTRIBUTE_ALIGN(4);
// pokebw2's sSendBuffer
static u8 data_ov231_02180a30[0xd0];

// pokebw2's IrCheck_ReadId
static u8 func_ov231_02180820(void) {
    u8 id;

    data_ov231_02180a20.send = data_ov231_02180a30;
    data_ov231_02180a20.recv = &id;
    data_ov231_02180a30[0] = IR_COMMAND_ID;
    CARD_LockBackup(data_ov231_02180a00);
    OS_LockCard(data_ov231_02180a00);

    {
        u64 start = OS_GetTick();

        while (OS_TicksToMicroSeconds(OS_GetTick() - start) < 60) {
        }
    }
    func_ov231_02180904();
    func_ov231_02180914(REG_MI_MCCNT0_MODE_MASK | 0x2);
    id = func_ov231_02180924(&data_ov231_02180a20);

    {
        u64 start = OS_GetTick();

        while (OS_TicksToMicroSeconds(OS_GetTick() - start) < 60) {
        }
    }
    func_ov231_02180914(0x2);
    func_ov231_02180984(&data_ov231_02180a20);
    func_ov231_02180904();

    OS_UnLockCard(data_ov231_02180a00);
    CARD_UnlockBackup(data_ov231_02180a00);
    return id;
}

// pokebw2's IrCheck_WaitBusy
static void func_ov231_02180904(void) {
    while (reg_MI_MCCNT0 & REG_MI_MCCNT0_BUSY_MASK) {
    }
}

// pokebw2's IrCheck_SetControl
static void func_ov231_02180914(u16 flags) {
    reg_MI_MCCNT0 = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | flags;
}

// pokebw2's IrCheck_SendByte
static u8 func_ov231_02180924(IrTransfer *transfer) {
    u64 start;
    // Read from the data register into a volatile copy, which the original keeps on the stack
    /* FAKEMATCH(volatile): the original keeps the read on the stack; not shown that it was volatile */
    vu16 data;

    start = OS_GetTick();
    while (OS_TicksToMicroSeconds(OS_GetTick() - start) < 50) {
    }
    reg_MI_MCD0 = *transfer->send;
    transfer->send++;
    func_ov231_02180904();
    data = reg_MI_MCD0;
    return data;
}

// pokebw2's IrCheck_RecvByte
static void func_ov231_02180984(IrTransfer *transfer) {
    u64 start;

    start = OS_GetTick();
    while (OS_TicksToMicroSeconds(OS_GetTick() - start) < 50) {
    }
    reg_MI_MCD0 = 0;
    func_ov231_02180904();
    *transfer->recv = reg_MI_MCD0;
    transfer->recv++;
}

// pokebw2's IrCheck_IsGenuineCard
BOOL func_ov231_021809d8(void) {
    func_ov231_02180820();
    if (func_ov231_02180820() == IR_ID) {
        return TRUE;
    }
    return FALSE;
}
