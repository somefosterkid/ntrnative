// TwlSDK 5's mi_sharedWram.c, the ARM9's DSi part in the LTD autoload's .ltdmain (lib/nitro/src/mi/mi_sharedWram.c
// has the NITRO part in ARM9 main): the WRAM A, B and C banks' registers, and the manager of which processor has
// each bank or has reserved it, which the ARM7 asks through the PXI FIFO. From the SDK's text (libntr's
// libraries/mi/src/mi_sharedWram.c), with Black's names; the SDK's are in the comments. Black's link drops what nothing
// calls (the bank setters of all three fields, the offset, enable, map end and image getters, the slot locks, the
// public alloc, switch and reserve requests, the slot cancel, the dump), so it isn't here.
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "twl/mi.h"

#define PXI_FIFO_TAG_MI 18

#define MIi_PXI_COMMAND_ALLOC 1
#define MIi_PXI_COMMAND_ALLOC_SLOT 2
#define MIi_PXI_COMMAND_FREE 3
#define MIi_PXI_COMMAND_FREE_SLOT 4
#define MIi_PXI_COMMAND_SWITCH 5
#define MIi_PXI_COMMAND_SWITCH_SLOT 6
#define MIi_PXI_COMMAND_RESERVE 7
#define MIi_PXI_COMMAND_RESERVE_SLOT 8
#define MIi_PXI_COMMAND_CANCEL 9
#define MIi_PXI_COMMAND_CANCEL_SLOT 10
#define MIi_PXI_COMMAND_GET_RESERVE 11
#define MIi_PXI_COMMAND_GET_ALLOCATABLE 14

#define MIi_SENDTYPE_COMMAND (FALSE)
#define MIi_SENDTYPE_RESULT (TRUE)

// The lock word that the two processors share for the manager (TwlSDK's HW_WRAM_EX_LOCK_BUF)
#define HW_WRAM_EX_LOCK_BUF 0x02fff680
#define HW_WRAM_BASE 0x03000000

static void func_0273063c(int tag, uPtr data, BOOL sendType);
static void func_02731154(BOOL sendType, u32 data);
static void func_027311a0(u32 pxiData, u32 *command, MIWram *wram, MIWramProc *proc, u32 *data);
static u32 func_02730868(MIWram wram, MIWramSize size, MIWramProc proc);
static u32 func_027309fc(MIWram wram, int slot, MIWramSize size, MIWramProc proc);
static int func_02730b48(MIWram wram, MIWramProc proc);
static int func_02730c2c(MIWram wram, int slot, MIWramSize size, MIWramProc proc);
static int func_02730cc0(MIWram wram, MIWramProc proc, MIWramProc newProc);
static int func_02730d50(MIWram wram, int slot, MIWramSize size, MIWramProc proc, MIWramProc newProc);
static u32 func_02730de4(MIWram wram, MIWramSize size, MIWramProc proc);
static u32 func_02730eec(MIWram wram, int slot, MIWramSize size, MIWramProc proc);
static int func_02731000(MIWram wram, MIWramProc proc);
static int func_02731060(MIWram wram, int slot, MIWramSize size, MIWramProc proc);
static int func_027310c0(MIWram wram, MIWramProc proc);
static OSIntrMode func_02731234(u16 *lockid);
static void func_02731254(u16 lockid, OSIntrMode e);

#include "twl/ltdmain_begin.h"

// The manager's state, in .ltdmain.bss as the SDK has it (where the function reaches each through a pool word of
// its own, as Black does; in .bss, before the pragma, it reaches them from one base): the SDK's MIi_Info, resultPtr,
// MIi_InitWramManager's sInitialized, finishPtr and lock. MWCC lays them out as Black has them (sInitialized,
// finishPtr, resultPtr, lock, MIi_Info) and the tables below in Black's order only in this order, here
static struct {
    u16 wram[3][MI_WRAM_B_MAX_NUM];
} data_02732f6c;
static u32 *data_02732f64 = NULL;
static BOOL data_02732f5c = FALSE;
static volatile BOOL *data_02732f60 = FALSE;
static BOOL data_02732f68 = FALSE;

// The dispatchers' tables of the three WRAMs' functions (the SDK's f of MI_GetWramBankMaster, MI_GetWramMapStart,
// MIi_SetWramBankEnable and MIi_SetWramBankMaster)
static void *(*data_027301ec[3])(void) = {func_027303a4, MI_GetWramMapStart_B, MI_GetWramMapStart_C};
static MIWramProc (*data_027301e0[3])(int) = {func_0273034c, func_02730360, func_02730374};
static void (*data_027301f8[3])(int, MIWramEnable) = {func_027302b4, func_027302dc, func_02730304};
static void (*data_02730204[3])(int, MIWramProc) = {func_02730240, func_0273025c, func_02730278};

#define MI_MAN_WRAM_RESERVE_SHIFT 8
#define MI_MAN_WRAM_MASTER_SHIFT 0

#define MI_MAN_WRAM_RESERVE_MASK (3 << MI_MAN_WRAM_RESERVE_SHIFT)
#define MI_MAN_WRAM_MASTER_MASK (3 << MI_MAN_WRAM_MASTER_SHIFT)

static inline int MIi_GetReservation(MIWram wram, int num) {
    return (data_02732f6c.wram[wram][num] & MI_MAN_WRAM_RESERVE_MASK) >> MI_MAN_WRAM_RESERVE_SHIFT;
}

static inline void MIi_SetReservation(MIWram wram, int num, int val) {
    data_02732f6c.wram[wram][num] =
        (u16)(data_02732f6c.wram[wram][num] & ~MI_MAN_WRAM_RESERVE_MASK | (val << MI_MAN_WRAM_RESERVE_SHIFT));
}

static inline int MIi_GetMaster(MIWram wram, int num) {
    return (data_02732f6c.wram[wram][num] & MI_MAN_WRAM_MASTER_MASK) >> MI_MAN_WRAM_MASTER_SHIFT;
}

static inline void MIi_SetMaster(MIWram wram, int num, int val) {
    data_02732f6c.wram[wram][num] =
        (u16)(data_02732f6c.wram[wram][num] & ~MI_MAN_WRAM_MASTER_MASK | (val << MI_MAN_WRAM_MASTER_SHIFT));
}

// MIi_GetWramBank_A/B/C
vu8 func_02730210(int num) {
    return *(vu8 *)(REG_MBK_A0_ADDR + num);
}

vu8 func_02730220(int num) {
    return *(vu8 *)(REG_MBK_B0_ADDR + num);
}

vu8 func_02730230(int num) {
    return *(vu8 *)(REG_MBK_C0_ADDR + num);
}

// MIi_SetWramBankMaster_A/B/C
void func_02730240(int num, MIWramProc proc) {
    vu8 *p = (vu8 *)(REG_MBK_A0_ADDR + num);

    *p = (u8)(*p & ~MI_WRAM_MASTER_MASK_A | proc);
}

void func_0273025c(int num, MIWramProc proc) {
    vu8 *p = (vu8 *)(REG_MBK_B0_ADDR + num);

    *p = (u8)(*p & ~MI_WRAM_MASTER_MASK_B | proc);
}

void func_02730278(int num, MIWramProc proc) {
    vu8 *p = (vu8 *)(REG_MBK_C0_ADDR + num);

    *p = (u8)(*p & ~MI_WRAM_MASTER_MASK_C | proc);
}

// MIi_SetWramBankMaster
void func_02730294(MIWram wram, int num, MIWramProc proc) {
    (data_02730204[wram])(num, proc);
}

// MIi_SetWramBankEnable_A/B/C
void func_027302b4(int num, MIWramEnable enable) {
    vu8 *p = (vu8 *)(REG_MBK_A0_ADDR + num);

    *p = (u8)(*p & ~MI_WRAM_ENABLE_MASK_A | (enable ? MI_WRAM_ENABLE_MASK_A : 0));
}

void func_027302dc(int num, MIWramEnable enable) {
    vu8 *p = (vu8 *)(REG_MBK_B0_ADDR + num);

    *p = (u8)(*p & ~MI_WRAM_ENABLE_MASK_B | (enable ? MI_WRAM_ENABLE_MASK_B : 0));
}

void func_02730304(int num, MIWramEnable enable) {
    vu8 *p = (vu8 *)(REG_MBK_C0_ADDR + num);

    *p = (u8)(*p & ~MI_WRAM_ENABLE_MASK_C | (enable ? MI_WRAM_ENABLE_MASK_C : 0));
}

// MIi_SetWramBankEnable
void func_0273032c(MIWram wram, int num, MIWramEnable enable) {
    (data_027301f8[wram])(num, enable);
}

// MI_GetWramBankMaster_A/B/C
MIWramProc func_0273034c(int num) {
    return (MIWramProc)(*(vu8 *)(REG_MBK_A0_ADDR + num) & MI_WRAM_MASTER_MASK_A);
}

MIWramProc func_02730360(int num) {
    return (MIWramProc)(*(vu8 *)(REG_MBK_B0_ADDR + num) & MI_WRAM_MASTER_MASK_B);
}

MIWramProc func_02730374(int num) {
    return (MIWramProc)(*(vu8 *)(REG_MBK_C0_ADDR + num) & MI_WRAM_MASTER_MASK_C);
}

MIWramProc MI_GetWramBankMaster(MIWram wram, int num) {
    return (data_027301e0[wram])(num);
}

// MI_GetWramMapStart_A
void *func_027303a4(void) {
    u32 wramReg = reg_MI_MBK6;

    return (void *)(HW_WRAM_BASE + (((wramReg & REG_MI_MBK6_WA_SADDR_MASK) >> REG_MI_MBK6_WA_SADDR_SHIFT) << 16));
}

void *MI_GetWramMapStart_B(void) {
    u32 wramReg = reg_MI_MBK7;

    return (void *)(HW_WRAM_BASE + (((wramReg & REG_MI_MBK7_WB_SADDR_MASK) >> REG_MI_MBK7_WB_SADDR_SHIFT) << 15));
}

void *MI_GetWramMapStart_C(void) {
    u32 wramReg = reg_MI_MBK8;

    return (void *)(HW_WRAM_BASE + (((wramReg & REG_MI_MBK8_WC_SADDR_MASK) >> REG_MI_MBK8_WC_SADDR_SHIFT) << 15));
}

void *MI_GetWramMapStart(MIWram wram) {
    return (data_027301ec[wram])();
}

// MIi_GetAddress_A/B/C: the address of a bank
static u32 func_0273042c(int num) {
    return (u32)((u32)func_027303a4() + MI_WRAM_A_SLOT_SIZE * num);
}

static u32 func_02730440(int num) {
    return (u32)((u32)MI_GetWramMapStart_B() + MI_WRAM_B_SLOT_SIZE * num);
}

static u32 func_02730454(int num) {
    return (u32)((u32)MI_GetWramMapStart_C() + MI_WRAM_C_SLOT_SIZE * num);
}

static inline u32 MIi_GetAddress(MIWram wram, int num) {
    if (wram == MI_WRAM_A) {
        return func_0273042c(num);
    }
    if (wram == MI_WRAM_B) {
        return func_02730440(num);
    }
    if (wram == MI_WRAM_C) {
        return func_02730454(num);
    }
    return 0;
}

// MIi_InitWramManager
void MIi_InitWramManagerCore(void) {
    int n;

    if (data_02732f5c) {
        return;
    }
    data_02732f5c = TRUE;

    for (n = 0; n < MI_WRAM_A_MAX_NUM; n++) {
        u8 val = func_02730210(n);
        switch (val & 1) {
        case 0:
            MIi_SetReservation(MI_WRAM_A, n, MI_WRAM_ARM9);
            MIi_SetMaster(MI_WRAM_A, n, MI_WRAM_ARM9);
            break;
        case 1:
            MIi_SetReservation(MI_WRAM_A, n, MI_WRAM_ARM7);
            MIi_SetMaster(MI_WRAM_A, n, MI_WRAM_ARM7);
            break;
        }
    }

    for (n = 0; n < MI_WRAM_B_MAX_NUM; n++) {
        u8 val = func_02730220(n);
        switch (val & 3) {
        case 0:
            MIi_SetReservation(MI_WRAM_B, n, MI_WRAM_ARM9);
            MIi_SetMaster(MI_WRAM_B, n, MI_WRAM_ARM9);
            break;
        case 1:
            MIi_SetReservation(MI_WRAM_B, n, MI_WRAM_ARM7);
            MIi_SetMaster(MI_WRAM_B, n, MI_WRAM_ARM7);
            break;
        default:
            MIi_SetReservation(MI_WRAM_B, n, MI_WRAM_DSP);
            MIi_SetMaster(MI_WRAM_B, n, MI_WRAM_DSP);
            break;
        }
    }

    for (n = 0; n < MI_WRAM_C_MAX_NUM; n++) {
        u8 val = func_02730230(n);
        switch (val & 3) {
        case 0:
            MIi_SetReservation(MI_WRAM_C, n, MI_WRAM_ARM9);
            MIi_SetMaster(MI_WRAM_C, n, MI_WRAM_ARM9);
            break;
        case 1:
            MIi_SetReservation(MI_WRAM_C, n, MI_WRAM_ARM7);
            MIi_SetMaster(MI_WRAM_C, n, MI_WRAM_ARM7);
            break;
        default:
            MIi_SetReservation(MI_WRAM_C, n, MI_WRAM_DSP);
            MIi_SetMaster(MI_WRAM_C, n, MI_WRAM_DSP);
            break;
        }
    }

    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_MI, func_0273063c);
}

// MIi_CallbackForPxi: a request of the ARM7's, answered with the result, or the reply to one of the ARM9's
static void func_0273063c(int tag, uPtr data, BOOL sendType) {
#pragma unused(tag)

    if (sendType == MIi_SENDTYPE_COMMAND) {
        u32 command, param;
        MIWram wram;
        MIWramProc proc;
        u32 retval = 0;

        func_027311a0(data, &command, &wram, &proc, &param);

        switch (command) {
        case MIi_PXI_COMMAND_ALLOC: {
            u32 size = (param >> 8) & 0xff;
            retval = func_02730868(wram, (MIWramSize)size, proc);
        } break;
        case MIi_PXI_COMMAND_ALLOC_SLOT: {
            int slot = (int)(param >> 8) & 0xff;
            MIWramSize size = (MIWramSize)(param & 0xff);
            retval = func_027309fc(wram, slot, size, proc);
        } break;
        case MIi_PXI_COMMAND_FREE: {
            retval = (u32)func_02730b48(wram, proc);
        } break;
        case MIi_PXI_COMMAND_FREE_SLOT: {
            int slot = (int)((param >> 8) & 0xff);
            MIWramSize size = (MIWramSize)(param & 0xff);
            retval = (u32)func_02730c2c(wram, slot, size, proc);
        } break;
        case MIi_PXI_COMMAND_SWITCH: {
            MIWramProc newProc = (MIWramProc)((param >> 12) & 3);
            retval = (u32)func_02730cc0(wram, proc, newProc);
        } break;
        case MIi_PXI_COMMAND_SWITCH_SLOT: {
            int slot = (int)((param >> 8) & 0xf);
            MIWramSize size = (MIWramSize)(param & 0xff);
            MIWramProc newProc = (MIWramProc)((param >> 12) & 3);
            retval = (u32)func_02730d50(wram, slot, size, proc, newProc);
        } break;
        case MIi_PXI_COMMAND_RESERVE: {
            u32 size = (param & 0xff);
            retval = func_02730de4(wram, (MIWramSize)size, proc);
        } break;
        case MIi_PXI_COMMAND_RESERVE_SLOT: {
            int slot = (int)((param >> 8) & 0xff);
            MIWramSize size = (MIWramSize)(param & 0xff);
            retval = func_02730eec(wram, slot, size, proc);
        } break;
        case MIi_PXI_COMMAND_CANCEL: {
            retval = (u32)func_02731000(wram, proc);
        } break;
        case MIi_PXI_COMMAND_CANCEL_SLOT: {
            int slot = (int)((param >> 8) & 0xff);
            MIWramSize size = (MIWramSize)(param & 0xff);
            retval = (u32)func_02731060(wram, slot, size, proc);
        } break;
        case MIi_PXI_COMMAND_GET_RESERVE: {
            retval = (u32)MIi_GetReservation(wram, (int)param);
        } break;
        case MIi_PXI_COMMAND_GET_ALLOCATABLE: {
            retval = (u8)func_027310c0(wram, proc);
        } break;
        }
        func_02731154(MIi_SENDTYPE_RESULT, retval);
    } else {
        if (data_02732f64) {
            *data_02732f64 = data;
        }
        if (data_02732f60) {
            *data_02732f60 = TRUE;
        }

        data_02732f68 = FALSE;
    }
}

// MIi_DoAllocWram: gives proc the first free run of banks of the size that nobody else has reserved
static u32 func_02730868(MIWram wram, MIWramSize size, MIWramProc proc) {
    int blocks = (int)((wram == MI_WRAM_A) ? (size / 2) : size);
    int allocated = 0;
    int firstBlock;
    int n;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = 0; n < maxNum; n++) {
        int master = MIi_GetMaster(wram, n);
        int reserve = MIi_GetReservation(wram, n);

        if (master == MI_WRAM_FREE && (reserve == proc || reserve == MI_WRAM_FREE)) {
            if (allocated == 0) {
                firstBlock = n;
            }

            if (++allocated >= blocks) {
                break;
            }
        } else {
            allocated = 0;
        }
    }

    if (allocated < blocks) {
        return 0;
    }

    for (n = 0; n < blocks; n++) {
        MIi_SetMaster(wram, firstBlock + n, proc);
        func_02730294(wram, firstBlock + n, proc);
        func_0273032c(wram, firstBlock + n, MI_WRAM_ENABLE);
    }

    return MIi_GetAddress(wram, firstBlock);
}

int MI_AllocWramSlot(MIWram wram, int slot, int size, MIWramProc proc) {
    u32 result;
    u16 lockid;

    OSIntrMode e = func_02731234(&lockid);
    result = func_027309fc(wram, slot, (MIWramSize)size, proc);
    func_02731254(lockid, e);

    return result;
}

// MIi_DoAllocWramSlot: gives proc the banks of the size from slot if they are all free
static u32 func_027309fc(MIWram wram, int slot, MIWramSize size, MIWramProc proc) {
    int blocks = (int)((wram == MI_WRAM_A) ? (size / 2) : size);
    int n;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = slot; n < slot + blocks; n++) {
        int reserve = MIi_GetReservation(wram, n);
        int master = MIi_GetMaster(wram, n);

        if (master == MI_WRAM_FREE && (reserve == proc || reserve == MI_WRAM_FREE)) {
        } else {
            return 0;
        }
    }

    for (n = slot; n < slot + blocks; n++) {
        MIi_SetMaster(wram, n, proc);
        func_02730294(wram, n, proc);
        func_0273032c(wram, n, MI_WRAM_ENABLE);
    }

    return MIi_GetAddress(wram, slot);
}

int MI_FreeWram(MIWram wram, MIWramProc proc) {
    int result;
    u16 lockid;

    OSIntrMode e = func_02731234(&lockid);
    result = func_02730b48(wram, proc);
    func_02731254(lockid, e);

    return result;
}

// MIi_DoFreeWram: frees every bank that proc has
static int func_02730b48(MIWram wram, MIWramProc proc) {
    int n;
    int freeCount = 0;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = 0; n < maxNum; n++) {
        int master = MIi_GetMaster(wram, n);
        if (master == proc) {
            MIi_SetMaster(wram, n, MI_WRAM_FREE);
            func_02730294(wram, n, MI_WRAM_ARM7 /*dummy*/);
            func_0273032c(wram, n, MI_WRAM_DISABLE);
            freeCount++;
        }
    }

    return freeCount;
}

int MI_FreeWramSlot(MIWram wram, int slot, int size, MIWramProc proc) {
    int result;
    u16 lockid;

    OSIntrMode e = func_02731234(&lockid);
    result = func_02730c2c(wram, slot, (MIWramSize)size, proc);
    func_02731254(lockid, e);

    return result;
}

// MIi_DoFreeWramSlot: frees the banks from slot that proc has
static int func_02730c2c(MIWram wram, int slot, MIWramSize size, MIWramProc proc) {
    int blocks = (int)((wram == MI_WRAM_A) ? (size / 2) : size);
    int n;
    int freeCount = 0;

    for (n = slot; n < slot + blocks; n++) {
        int master = MIi_GetMaster(wram, n);
        if (master == proc) {
            MIi_SetMaster(wram, n, MI_WRAM_FREE);
            func_02730294(wram, n, MI_WRAM_ARM7 /*dummy*/);
            func_0273032c(wram, n, MI_WRAM_DISABLE);
            freeCount++;
        }
    }

    return freeCount;
}

// MIi_DoSwitchWram: gives every bank that proc has to newProc
static int func_02730cc0(MIWram wram, MIWramProc proc, MIWramProc newProc) {
    int n;
    int retval = 0;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = 0; n < maxNum; n++) {
        int master = MIi_GetMaster(wram, n);
        if (master == proc) {
            MIi_SetMaster(wram, n, newProc);
            func_02730294(wram, n, newProc);
            func_0273032c(wram, n, MI_WRAM_ENABLE);
            retval++;
        }
    }
    return retval;
}

// MIi_DoSwitchWramSlot: gives the banks from slot that proc has to newProc
static int func_02730d50(MIWram wram, int slot, MIWramSize size, MIWramProc proc, MIWramProc newProc) {
    int blocks = (int)((wram == MI_WRAM_A) ? (size / 2) : size);
    int retval = 0;
    int n;

    for (n = slot; n < slot + blocks; n++) {
        int master = MIi_GetMaster(wram, n);
        if (master == proc) {
            MIi_SetMaster(wram, n, newProc);
            func_02730294(wram, n, newProc);
            func_0273032c(wram, n, MI_WRAM_ENABLE);
            retval++;
        }
    }
    return retval;
}

// MIi_DoReserveWram: reserves for proc the first run of banks of the size that are free and not reserved
static u32 func_02730de4(MIWram wram, MIWramSize size, MIWramProc proc) {
    int blocks = (int)((wram == MI_WRAM_A) ? (size / 2) : size);
    int allocated = 0;
    int firstBlock;
    int n;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = 0; n < maxNum; n++) {
        int master = MIi_GetMaster(wram, n);
        int reserve = MIi_GetReservation(wram, n);

        if (master == MI_WRAM_FREE && reserve == MI_WRAM_FREE) {
            if (allocated == 0) {
                firstBlock = n;
            }

            if (++allocated >= blocks) {
                break;
            }
        } else {
            allocated = 0;
        }
    }

    if (allocated < blocks) {
        return 0;
    }

    for (n = 0; n < blocks; n++) {
        MIi_SetReservation(wram, firstBlock + n, proc);
    }

    return MIi_GetAddress(wram, firstBlock);
}

// MIi_DoReserveWramSlot: reserves for proc the banks of the size from slot if they are free and not reserved. Each
// pass checks the first bank, as the SDK has it
static u32 func_02730eec(MIWram wram, int slot, MIWramSize size, MIWramProc proc) {
    int blocks = (int)((wram == MI_WRAM_A) ? (size / 2) : size);
    int n;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = slot; n < slot + blocks; n++) {
        int master = MIi_GetMaster(wram, slot);
        int reserve = MIi_GetReservation(wram, slot);

        if (master == MI_WRAM_FREE && reserve == MI_WRAM_FREE) {
        } else {
            return 0;
        }
    }

    for (n = slot; n < slot + blocks; n++) {
        MIi_SetReservation(wram, n, proc);
    }

    return MIi_GetAddress(wram, slot);
}

// MI_CancelWram
int func_02730fbc(MIWram wram, MIWramProc proc) {
    int result;
    u16 lockid;

    OSIntrMode e = func_02731234(&lockid);
    result = func_02731000(wram, proc);
    func_02731254(lockid, e);

    return result;
}

// MIi_DoCancelWram: cancels every reservation of proc's
static int func_02731000(MIWram wram, MIWramProc proc) {
    int retval = 0;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;
    int n;

    for (n = 0; n < maxNum; n++) {
        if (MIi_GetReservation(wram, n) == proc) {
            MIi_SetReservation(wram, n, MI_WRAM_FREE);
            retval++;
        }
    }
    return retval;
}

// MIi_DoCancelWramSlot: cancels proc's reservations of the banks from slot
static int func_02731060(MIWram wram, int slot, MIWramSize size, MIWramProc proc) {
    int blocks = (int)((wram == MI_WRAM_A) ? (size / 2) : size);
    int retval = 0;
    int n;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = slot; n < slot + blocks; n++) {
        if (MIi_GetReservation(wram, n) == proc) {
            MIi_SetReservation(wram, n, MI_WRAM_FREE);
            retval++;
        }
    }
    return retval;
}

// MIi_DoGetAllocatableWramSlot: a bit for each bank that proc could be given
static int func_027310c0(MIWram wram, MIWramProc proc) {
    int retval = 0;
    int n;
    int maxNum = (wram == MI_WRAM_A) ? MI_WRAM_A_MAX_NUM : MI_WRAM_B_MAX_NUM;

    for (n = 0; n < maxNum; n++) {
        int master = MIi_GetMaster(wram, n);
        int reserve = MIi_GetReservation(wram, n);

        if (master == MI_WRAM_FREE && (reserve == proc || reserve == MI_WRAM_FREE)) {
            retval |= (1 << n);
        }
    }
    return retval;
}

// MI_IsWramSlotUsed
BOOL MIi_IsWramSlotUsed(MIWram wram, int slot) {
    return (MIi_GetMaster(wram, slot) == MI_WRAM_FREE) ? FALSE : TRUE;
}

// MIi_SendToPxi
static void func_02731154(BOOL sendType, u32 data) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_MI, data, sendType) != PXI_FIFO_SUCCESS) {
        SVC_WaitByLoop(1);
    }
}

// MIi_TakeApartData
static void func_027311a0(u32 pxiData, u32 *command, MIWram *wram, MIWramProc *proc, u32 *data) {
    if (command) {
        *command = (pxiData >> 20) & 0xf;
    }
    if (wram) {
        *wram = (MIWram)((pxiData >> 18) & 3);
    }
    if (proc) {
        *proc = (MIWramProc)((pxiData >> 16) & 3);
    }
    if (data) {
        *data = (pxiData) & 0xffff;
    }
}

// MI_LockWramManager
void func_027311e8(u16 *lockid) {
    *lockid = (u16)OS_GetLockID();
    (void)OS_LockByWord(*lockid, (OSLockWord *)(HW_WRAM_EX_LOCK_BUF), NULL);
}

// MI_UnlockWramManager
void func_02731210(u16 lockid) {
    (void)OS_UnlockByWord(lockid, (OSLockWord *)(HW_WRAM_EX_LOCK_BUF), NULL);
    OS_ReleaseLockID(lockid);
}

// MIi_LockWramManagerDI
static OSIntrMode func_02731234(u16 *lockid) {
    OSIntrMode e = OS_DisableInterrupts();
    func_027311e8(lockid);
    return e;
}

// MIi_UnlockWramManagerRI
static void func_02731254(u16 lockid, OSIntrMode e) {
    func_02731210(lockid);
    (void)OS_RestoreInterrupts(e);
}

#include "twl/ltdmain_end.h"
