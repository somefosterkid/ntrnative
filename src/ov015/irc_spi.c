// Black's code at ov015 0x0217a5b4-0x0217a9c8: the card's infrared chip over the backup SPI bus (pokebw2's irc_spi.c, the
// last file of the overlay's four: see net_devirc.c). A transfer is a command byte, 1 to read and 2 to write, and then the
// bytes, with a pause of 60 microseconds between.

#include "types.h"
#include "field/irc_spi.h"
#include "nitro/card.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "system/os_spinLock.h"

// The most bytes the chip answers with (pokebw2's irc_spi.c)
#define IRC_RECV_MAX 0xb5

// Where the bytes of an SPI transfer come from and go to (pokebw2's irc_spi.c)
typedef struct {
    u32 unk0;
    const u8 *send;
    u8 *recv;
    u32 unkC;
} IrcSpiData;

static u16 data_ov015_0217aab4 = 0xfffd;
// Aligned, as pokebw2 has it: at -O4,p MWCC rotates the loops of the functions that read it by name unless an
// object they read has an explicit alignment (the original enters them with a branch to the test)
static IrcSpiData data_ov015_0217ac90 ATTRIBUTE_ALIGN(4);
static u8 data_ov015_0217aca0[0xb8];
static u8 data_ov015_0217ad58[0xc8];

#pragma thumb off
void func_ov015_0217a5b4(void) {
    u64 start = OS_GetTick();

    while (OS_TicksToMicroSeconds(OS_GetTick() - start) < 60) {
    }
}

#pragma thumb on

#pragma thumb off
void func_ov015_0217a60c(void) {
    data_ov015_0217ac90.unk0 = 1;
    if (data_ov015_0217aab4 != 0xfffd) {
        return;
    }
    data_ov015_0217aab4 = OS_GetLockID();
}
#pragma thumb on

#pragma thumb off
void func_ov015_0217a648(void) {
    if (data_ov015_0217aab4 == 0xfffffffd) {
        return;
    }
    OS_ReleaseLockID(data_ov015_0217aab4);
    data_ov015_0217aab4 = 0xfffd;
}

u8 func_ov015_0217a674(IrcSpiData *transfer) {
    u64 start;
    vu16 data;

    start = OS_GetTick();
    while (OS_TicksToMicroSeconds(OS_GetTick() - start) < 50) {
    }
    reg_MI_MCD0 = *transfer->send;
    transfer->send++;
    while (reg_MI_MCCNT0 & REG_MI_MCCNT0_BUSY_MASK) {
    }
    data = reg_MI_MCD0;
    return data;
}

void func_ov015_0217a718(IrcSpiData *transfer) {
    u64 start = OS_GetTick();

    while (OS_TicksToMicroSeconds(OS_GetTick() - start) < 50) {
    }
    reg_MI_MCD0 = 0;
    while (reg_MI_MCCNT0 & REG_MI_MCCNT0_BUSY_MASK) {
    }
    *transfer->recv = reg_MI_MCD0;
    transfer->recv++;
}

u8 func_ov015_0217a7ac(u8 *buf) {
    u8 size;
    int i;

    data_ov015_0217ac90.send = data_ov015_0217aca0;
    data_ov015_0217ac90.recv = buf;
    data_ov015_0217aca0[0] = 1;
    buf[0] = 0;
    CARD_LockBackup(data_ov015_0217aab4);
    OS_LockCard(data_ov015_0217aab4);
    while (reg_MI_MCCNT0 & REG_MI_MCCNT0_BUSY_MASK) {
    }
    reg_MI_MCCNT0 = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | REG_MI_MCCNT0_MODE_MASK | 0x2;
    func_ov015_0217a674(&data_ov015_0217ac90);
    func_ov015_0217a5b4();
    func_ov015_0217a718(&data_ov015_0217ac90);
    func_ov015_0217a5b4();
    size = buf[0];
    if (size > IRC_RECV_MAX) {
        size = 0;
    }
    data_ov015_0217ac90.send = NULL;
    data_ov015_0217ac90.recv = buf;
    buf[0] = 0;
    if (size != 0) {
        for (i = 0; i < size; i++) {
            if (i == size - 1) {
                reg_MI_MCCNT0 = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | 0x2;
            }
            func_ov015_0217a718(&data_ov015_0217ac90);
        }
    } else {
        reg_MI_MCCNT0 = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | 0x2;
        func_ov015_0217a718(&data_ov015_0217ac90);
    }
    for (i = 0; i < size; i++) {
        buf[i] ^= IRC_XOR;
    }
    while (reg_MI_MCCNT0 & REG_MI_MCCNT0_BUSY_MASK) {
    }
    OS_UnLockCard(data_ov015_0217aab4);
    CARD_UnlockBackup(data_ov015_0217aab4);
    return size;
}

void func_ov015_0217a8ec(u8 *data, s32 size) {
    int i;

    data_ov015_0217ac90.send = data_ov015_0217ad58;
    data_ov015_0217ac90.recv = NULL;
    data_ov015_0217ad58[0] = 2;
    CARD_LockBackup(data_ov015_0217aab4);
    OS_LockCard(data_ov015_0217aab4);
    while (reg_MI_MCCNT0 & REG_MI_MCCNT0_BUSY_MASK) {
    }
    reg_MI_MCCNT0 = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | REG_MI_MCCNT0_MODE_MASK | 0x2;
    func_ov015_0217a674(&data_ov015_0217ac90);
    func_ov015_0217a5b4();
    data_ov015_0217ac90.send = data;
    for (i = 0; i < size; i++) {
        if (i == size - 1) {
            reg_MI_MCCNT0 = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | 0x2;
        }
        func_ov015_0217a674(&data_ov015_0217ac90);
    }
    while (reg_MI_MCCNT0 & REG_MI_MCCNT0_BUSY_MASK) {
    }
    OS_UnLockCard(data_ov015_0217aab4);
    CARD_UnlockBackup(data_ov015_0217aab4);
}




#pragma thumb on
