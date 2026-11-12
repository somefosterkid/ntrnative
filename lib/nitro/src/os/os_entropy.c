// TwlSDK's OS library, os_entropy.c: Black's main 0x02088168-0x02088234. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "vct_dsp.h"

extern volatile u64 OSi_TickCounter;
// The shared work area at 0x02fffc00, the fields OS_GetLowEntropyData reads, at Black's offsets
typedef struct SystemWorkPart {
    u8 unk_000[0x3c];
    u32 unk_03c;
    u8 unk_040[0xb4];
    u32 unk_0f4;
    u16 unk_0f8;
    u8 unk_0fa[0x1e8 - 0x0fa];
    u32 unk_1e8;
    u32 unk_1ec;
    u8 unk_1f0[0x390 - 0x1f0];
    u32 unk_390;
    u16 unk_394;
    u8 unk_396[0x398 - 0x396];
    u16 unk_398;
    u8 unk_39a[0x3aa - 0x39a];
    u16 unk_3aa;
    u16 unk_3ac;
} SystemWorkPart;

void OS_GetLowEntropyData(u32 *buffer)
{
    const SystemWorkPart *work = (SystemWorkPart *)0x02fffc00;
    const u32 *macAddress = &work->unk_0f4;

    buffer[0] = (u32)((GX_GetVCount() << 16) | OS_GetTickLo());
    buffer[1] = (u32)(*(u16 *)(macAddress + 1) << 16) ^ (u32)OSi_TickCounter;
    buffer[2] = (u32)(OSi_TickCounter >> 32) ^ *macAddress ^ work->unk_03c;
    buffer[2] ^= reg_G3X_GXSTAT;
    buffer[3] = work->unk_1e8;
    buffer[4] = work->unk_1ec;
    buffer[5] = (work->unk_394 << 16) ^ work->unk_390;
    buffer[6] = (u32)((work->unk_3aa << 16) | work->unk_3ac);
    buffer[7] = (u32)((work->unk_398 << 16) | (reg_PAD_KEYINPUT | *(vu16 *)HW_BUTTON_XY_BUF));
}
