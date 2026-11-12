// Black's code at main 0x02090f18-0x020911a0: the cartridge library's common part (TwlSDK's libctrdg, pokebw2's
// lib/nitro/src/ctrdg/ctrdg.c)

#include "types.h"
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "nitro/mi.h"
#include "system/os_spinLock.h"
#include "nitro/ctrdg.h"

typedef struct OSLockWord OSLockWord;

u16 OS_ReadOwnerOfLockWord(OSLockWord *lockp);
s32 OS_TryLockCartridge(u16 lockID);
s32 OS_UnLockCartridge(u16 lockID);

BOOL func_02090ef4(void);
void func_020910d0(u16 lockID, u32 *lockInfo);
void func_02091054(u32 *cycle);
void func_0209109c(u32 *cycle);
void func_0209112c(u16 lockID, s32 *lockInfo);

u32 data_02153b84;
u32 data_02153b80;

#pragma thumb off

void func_02090f18(void) {
    u32 zero = 0;

    CpuSet(&zero, &data_02153b84, 0x5000001);
    ((u16 *)&data_02153b80)[3] = OS_GetLockID();
}

BOOL CTRDG_IsExisting(void) {
    BOOL retval = TRUE;
    u32 lockInfo[2];
    u32 cycle[2];
    u8 *header = (u8 *)0x08000000;
    CTRDGModuleInfo *cip = CTRDG_MODULE_INFO;

    if (!func_02090ef4()) {
        return FALSE;
    }

    if (cip->pulledOut == TRUE) {
        return FALSE;
    }
    func_020910d0(((u16 *)&data_02153b80)[3], lockInfo);

    func_02091054(cycle);
    {
        u8 isRomCode = header[0xb2];

        if ((isRomCode == 0x96 && cip->romCheck != *(u16 *)(header + 0xbe))
            || (isRomCode != 0x96 && cip->romCheck != *(u16 *)0x0801fffe)
            || (cip->gameCode != *(u32 *)(header + 0xac) && cip->isAgbCartridge)) {
            cip->pulledOut = TRUE;
            retval = FALSE;
        }
    }
    func_0209109c(cycle);

    func_0209112c(((u16 *)&data_02153b80)[3], (s32 *)lockInfo);
    return retval;
}


void func_02091054(u32 *cycle) {
    cycle[0] = (reg_MI_EXMEMCNT & 0xc) >> 2;
    cycle[1] = (reg_MI_EXMEMCNT & 0x10) >> 4;
    reg_MI_EXMEMCNT = (u16)((reg_MI_EXMEMCNT & ~0xc) | 0xc);
    reg_MI_EXMEMCNT = (u16)(reg_MI_EXMEMCNT & ~0x10);
}


void func_0209109c(u32 *cycle) {
    reg_MI_EXMEMCNT = (u16)((cycle[0] << 2) | (reg_MI_EXMEMCNT & ~0xc));
    reg_MI_EXMEMCNT = (u16)((cycle[1] << 4) | (reg_MI_EXMEMCNT & ~0x10));
}

void func_020910d0(u16 param_1, u32 *param_2) {
    u16 result;

    while( TRUE ) {
        param_2[1] = OS_DisableInterrupts();
        result = OS_ReadOwnerOfLockWord((OSLockWord *)0x2ffffe8);
        *param_2 = result & 0x40;
        if ((result & 0x40) != 0) {
            return;
        }
        if (OS_TryLockCartridge(param_1) == 0) break;
        OS_RestoreInterrupts(param_2[1]);
        SVC_WaitByLoop(1);
    }
}
#pragma thumb on


#pragma thumb off
void func_0209112c(u16 param_1, s32 *param_2) {
    if (*param_2 == 0) {
        OS_UnLockCartridge(param_1);
    }
    OS_RestoreInterrupts(param_2[1]);
}
#pragma thumb on


#pragma thumb off
void func_02091150(u32 param_1) {
    s32 result;

    result = PXI_SendWordByFifo(0xd, param_1, 0);
    if (result != 0) {
        do {
            SVC_WaitByLoop(1);
            result = PXI_SendWordByFifo(0xd, param_1, 0);
        } while (result != 0);
        return;
    }
}
