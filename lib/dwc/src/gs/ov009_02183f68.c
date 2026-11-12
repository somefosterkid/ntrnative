// Black's code at ov009 0x02183f68-0x021840cc. No name is known: the module and address (the screen text console that
// gsiDebug's default callback prints to, and a tick-to-milliseconds helper).

#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"

static s8 data_ov009_021ad7c8[2048];
static u8 data_ov009_021ad788[64];
u64 __aeabi_uldivmod(u32 param_1, u32 param_2, u32 param_3, u32 param_4);

static u16 data_ov009_021ad1c8[736];
void func_ov009_02183fb4(s32 param_1);
static u32 data_ov009_021ad1c4;

#pragma thumb off
void func_ov009_02183f68(void) {
    s32 i;

    i = 0;
    do {
        MIi_CpuCopyFast((void *)data_ov009_021ad1c8 + (i + 1) * 0x40, (void *)data_ov009_021ad1c8 + i * 0x40,
                                        0x40);
        i = i + 1;
    } while (i < 0x17);
    MIi_CpuClearFast(0, (void *)data_ov009_021ad788, 0x40);
}

void func_ov009_02183fb4(s32 param_1) {
    if (param_1 == 0xD) {
        return;
    }
    if (data_ov009_021ad1c4 == 0x20) {
        func_ov009_02183f68();
        data_ov009_021ad1c4 = 0;
        if (param_1 == 0xA) {
            return;
        }
    }
    if (param_1 == 0xA) {
        data_ov009_021ad1c4 = 0x20;
        return;
    }
    data_ov009_021ad1c8[23 * 32 + data_ov009_021ad1c4] = param_1 | 0xF000;
    data_ov009_021ad1c4++;
}

#pragma thumb on

#pragma thumb off
static u32 data_ov009_021a3fbc = 2;

void func_ov009_0218402c(s8 *param_1, s8 *param_2) {
    s32 i;

    if (!(data_ov009_021a3fbc & 1)) {
        return;
    }
    OS_VSNPrintf((void *)data_ov009_021ad7c8, 0x7FFU, param_1, param_2);
    i = 0;
    *(s8 *)((u8 *)((void *)data_ov009_021ad7c8) + 0x7FF) = 0;
    if (*(s8 *)((void *)data_ov009_021ad7c8) != 0) {
        do {
            func_ov009_02183fb4((s32) data_ov009_021ad7c8[i]);
            i += 1;
        } while (data_ov009_021ad7c8[i] != 0);
    }
    OS_WaitVBlankIntr();
}

s32 func_ov009_02184098(u32 *param_1) {
    u64 tick;

    tick = OS_GetTick();
    tick = __aeabi_uldivmod((u32)(tick << 6), (u32)((tick << 6) >> 0x20), 0x1ff6210, 0);
    if (param_1 != (u32 *)0x0) {
        *param_1 = (u32)tick;
    }
    return (s32)tick;
}
#pragma thumb on
