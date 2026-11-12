// Black's code at main 0x02083644-0x0208459c, between TwlSDK's MI and OS in the link: a library that talks to the ARM7
// over the PXI FIFO's NVRAM tag (4) and writes in 256-byte pages. Its name and source are not known.

#include "types.h"
#include "stddef.h"
#include "system/main_02083644.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/pxi.h"

#pragma thumb off

// The address of a buffer as the ARM7 is given it: the PXI words carry 32 bits of it
#define NVRAM_PXI_ADDRESS(buf) ((u32)(uintptr_t)(buf))

void func_02084204(PXIFifoTag tag, uPtr data, BOOL err);
void func_020839e8(u32 data, BOOL err);
void func_02083fa0(u32 data, BOOL err);
BOOL OS_IsAlarmAvailable(void);
void OS_InitAlarm(void);
// What a call calls back with when it ends: its result, and the argument it was given
typedef void (*NvramCallback)(u32 result, void *arg);
// The state block at 0x02150ee0 (0x58 bytes): the state, the callback and its argument, the alarm
// at 0x02150eec, the step of a write or a verify and the NVRAM address, size and buffer it goes on with.
typedef struct PxiWaitState {
    u32 state;
    NvramCallback callback;
    void *arg;
    OSAlarm alarm;
    u64 unk_38;
    u32 unk_40;
    u32 address;
    u32 size;
    const u8 *buf;
    OSThreadQueue queue;
} PxiWaitState;
// The .bss lays them out in the reverse of this order: the state of the initialization, the buffer (32-byte aligned
// for the cache), the state block
static PxiWaitState data_02150ee0;
static u8 data_02150ec0[0x20] ATTRIBUTE_ALIGN(32);
static s32 data_02150ea0;
BOOL func_020841e8(void);
BOOL func_020843b4(u32 param_1, u32 param_2, const void *param_3);
BOOL func_020842d8(void);
void func_02084260(u32 result, void *arg);
BOOL func_020842a0(const void *a, const void *b, u32 size);
BOOL func_020844a8(u32 param_1, u32 param_2, const void *param_3);
void func_02084288(void *arg);
BOOL func_02084570(void);
BOOL func_02084304(void);
BOOL func_02084330(void *param_1);

void NVRAM_Init(void) {
    u32 enabled;

    enabled = OS_DisableInterrupts();
    if (data_02150ea0 != 0) {
        OS_RestoreInterrupts(enabled);
        return;
    }
    data_02150ea0 = 1;
    OS_RestoreInterrupts(enabled);
    PXI_Init();
    do {
    } while (PXI_IsCallbackReady(4, 1) == FALSE);
    PXI_SetFifoRecvCallback(4, func_02084204);
    MIi_CpuClearFast(0, &data_02150ee0, sizeof(data_02150ee0));
    if (OS_IsTickAvailable() == FALSE) {
        OS_InitTick();
    }
    if (OS_IsAlarmAvailable() == FALSE) {
        OS_InitAlarm();
    }
    OS_CreateAlarm(&data_02150ee0.alarm);
    data_02150ee0.queue.tail = NULL;
    data_02150ee0.queue.head = NULL;
    data_02150ea0 = 2;
}

u32 func_02083704(u32 param_1, u32 param_2, void *param_3, NvramCallback param_4, void *param_5) {
    u32 enabled;

    if (!func_020841e8()) {
        return 4;
    }
    enabled = OS_DisableInterrupts();
    if (data_02150ee0.state != 0) {
        OS_RestoreInterrupts(enabled);
        return 1;
    }
    data_02150ee0.state = 1;
    OS_RestoreInterrupts(enabled);
    data_02150ee0.callback = param_4;
    data_02150ee0.arg = param_5;
    data_02150ee0.unk_40 = 0;
    enabled = OS_DisableInterrupts();
    if (func_020843b4(param_1, param_2, param_3) == TRUE) {
        OS_RestoreInterrupts(enabled);
        return 0;
    }
    data_02150ee0.state = 0;
    OS_RestoreInterrupts(enabled);
    OS_WakeupThread(&data_02150ee0.queue);
    return 3;
}

void func_020837b8(u32 param_1, BOOL param_2) {
    u32 result = 7;
    NvramCallback callback;
    u8 kind = param_1;

    if (param_2 == 0 && kind == 0) {
        result = 0;
    } else if (param_2 == 1) {
        result = 3;
    } else {
        switch (kind) {
        case 2:
            result = 2;
            break;
        case 3:
            result = 4;
            break;
        case 4:
            result = 1;
            break;
        }
    }

    data_02150ee0.state = 0;
    data_02150ee0.unk_40 = 0;
    callback = data_02150ee0.callback;
    if (callback != NULL) {
        data_02150ee0.callback = NULL;
        callback(result, data_02150ee0.arg);
    }
    OS_WakeupThread(&data_02150ee0.queue);
}

u32 func_02083844(u32 param_1, u32 param_2, void *param_3) {
    OSMessageQueue queue;
    void *message;
    OSMessage msgArray[1];
    u32 enabled;

    if (OSi_ThreadInfo.current == NULL) {
        return 4;
    }
    OS_InitMessageQueue(&queue, msgArray, 1);
    for (;;) {
        enabled = OS_DisableInterrupts();
        message = (OSMessage)(uintptr_t)func_02083704(param_1, param_2, param_3, func_02084260, &queue);
        if ((uintptr_t)message == 1) {
            OS_SleepThread(&data_02150ee0.queue);
        } else if ((uintptr_t)message == 3) {
            OS_Sleep(1);
        } else {
            OS_RestoreInterrupts(enabled);
            break;
        }
        OS_RestoreInterrupts(enabled);
    }
    if (message == NULL) {
        if (!OS_ReceiveMessage(&queue, &message, 1)) {
            OS_Terminate();
        }
    }
    return (u32)(uintptr_t)message;
}

u32 func_02083934(u32 param_1, u32 param_2, const void *param_3, NvramCallback param_4, void *param_5) {
    u32 enabled;

    if (!func_020841e8()) {
        return 4;
    }
    enabled = OS_DisableInterrupts();
    if (data_02150ee0.state != 0) {
        OS_RestoreInterrupts(enabled);
        return 1;
    }
    data_02150ee0.state = 2;
    OS_RestoreInterrupts(enabled);
    data_02150ee0.callback = param_4;
    data_02150ee0.arg = param_5;
    data_02150ee0.address = param_1;
    data_02150ee0.size = param_2;
    data_02150ee0.buf = param_3;
    data_02150ee0.unk_40 = 0;
    enabled = OS_DisableInterrupts();
    if (func_020842d8() == TRUE) {
        OS_RestoreInterrupts(enabled);
        return 0;
    }
    data_02150ee0.state = 0;
    OS_RestoreInterrupts(enabled);
    OS_WakeupThread(&data_02150ee0.queue);
    return 3;
}

// Calls back with the result, ending the call, and wakes the threads that wait for it
static inline void NVRAMi_EndCommand(u32 result) {
    NvramCallback callback;

    data_02150ee0.state = 0;
    data_02150ee0.unk_40 = 0;
    callback = data_02150ee0.callback;
    if (callback) {
        data_02150ee0.callback = NULL;
        callback(result, data_02150ee0.arg);
    }
    OS_WakeupThread(&data_02150ee0.queue);
}

// Fires the alarm that stands in for an answer of the ARM7, after the poll time
static inline void NVRAMi_SetAlarm(u32 pxiResult) {
    OS_SetAlarm(&data_02150ee0.alarm, OS_MilliSecondsToTicks(1), func_02084288, (void *)(uintptr_t)pxiResult);
}

void func_020839e8(u32 param_1, BOOL param_2) {
    u32 result = 7;
    u16 status = param_1 & 0xff;
    u16 room = 0x100 - (u8)data_02150ee0.address;
    u32 size = data_02150ee0.size;
    u16 chunk;
    u8 flash;

    chunk = room > size ? size : room;
    switch (data_02150ee0.unk_40) {
    case 0:
        if (param_2 == FALSE && status == 0) {
            if (func_020844a8(data_02150ee0.address, chunk, data_02150ee0.buf) == TRUE) {
                data_02150ee0.unk_40 = 1;
                return;
            }
            result = 3;
        } else if (param_2 == TRUE) {
            result = 3;
        } else if (status == 4) {
            result = 1;
        }
        break;
    case 1:
        if (param_2 == FALSE && status == 0) {
            data_02150ee0.address += chunk;
            data_02150ee0.size -= chunk;
            data_02150ee0.buf += chunk;
            data_02150ee0.unk_38 = OS_GetTick();
            data_02150ee0.unk_40 = 5;
            NVRAMi_SetAlarm(0);
            return;
        } else if (param_2 == TRUE) {
            result = 3;
        } else {
            switch (status) {
            case 2:
                result = 2;
                break;
            case 3:
                result = 4;
                break;
            case 4:
                if (func_020844a8(data_02150ee0.address, chunk, data_02150ee0.buf) == TRUE) {
                    return;
                }
                result = 3;
                break;
            }
        }
        break;
    case 2:
        if (param_2 == FALSE && status == 0) {
            flash = data_02150ec0[0];
            if (flash & 0x20) {
                if (func_02084570() == TRUE) {
                    data_02150ee0.unk_40 = 4;
                    return;
                }
                data_02150ee0.unk_40 = 4;
                NVRAMi_SetAlarm(4);
                return;
            }
            if (!(flash & 1)) {
                if (chunk != 0) {
                    if (func_020842d8() == TRUE) {
                        data_02150ee0.unk_40 = 0;
                        return;
                    }
                    result = 3;
                    break;
                }
                if (func_02084304() == TRUE) {
                    data_02150ee0.unk_40 = 3;
                    return;
                }
                data_02150ee0.unk_40 = 3;
                NVRAMi_SetAlarm(4);
                return;
            }
        } else if (param_2 == FALSE && status != 4) {
            break;
        }
        data_02150ee0.unk_40 = 5;
        NVRAMi_SetAlarm(0);
        return;
    case 3:
        if (param_2 == FALSE && status == 0) {
            result = 0;
            break;
        }
        if (param_2 == FALSE && status != 4) {
            break;
        }
        if (func_02084304() == TRUE) {
            return;
        }
        data_02150ee0.unk_40 = 3;
        NVRAMi_SetAlarm(4);
        return;
    case 4:
        if (param_2 == FALSE && status == 0) {
            result = 5;
            break;
        }
        if (param_2 == FALSE && status != 4) {
            break;
        }
        if (func_02084570() == TRUE) {
            return;
        }
        data_02150ee0.unk_40 = 4;
        NVRAMi_SetAlarm(4);
        return;
    case 5:
        DC_InvalidateRange(data_02150ec0, 0x20);
        if (func_02084330(data_02150ec0) == TRUE) {
            data_02150ee0.unk_40 = 2;
            return;
        }
        data_02150ee0.unk_40 = 2;
        data_02150ec0[0] = 1;
        DC_StoreRange(data_02150ec0, 0x20);
        NVRAMi_SetAlarm(0);
        return;
    }
    NVRAMi_EndCommand(result);
}

u32 func_02083dc8(u32 param_1, u32 param_2, const void *param_3) {
    OSMessageQueue queue;
    void *message;
    OSMessage msgArray[1];
    u32 enabled;

    if (OSi_ThreadInfo.current == NULL) {
        return 4;
    }
    OS_InitMessageQueue(&queue, msgArray, 1);
    for (;;) {
        enabled = OS_DisableInterrupts();
        message = (OSMessage)(uintptr_t)func_02083934(param_1, param_2, param_3, func_02084260, &queue);
        if ((uintptr_t)message == 1) {
            OS_SleepThread(&data_02150ee0.queue);
        } else if ((uintptr_t)message == 3) {
            OS_Sleep(1);
        } else {
            OS_RestoreInterrupts(enabled);
            break;
        }
        OS_RestoreInterrupts(enabled);
    }
    if (message == NULL) {
        if (!OS_ReceiveMessage(&queue, &message, 1)) {
            OS_Terminate();
        }
    }
    return (u32)(uintptr_t)message;
}

u32 func_02083eb8(u32 param_1, u32 param_2, const void *param_3, NvramCallback param_4, void *param_5) {
    u32 enabled;
    u32 size;

    if (!func_020841e8()) {
        return 4;
    }
    enabled = OS_DisableInterrupts();
    if (data_02150ee0.state != 0) {
        OS_RestoreInterrupts(enabled);
        return 1;
    }
    data_02150ee0.state = 3;
    OS_RestoreInterrupts(enabled);
    data_02150ee0.callback = param_4;
    data_02150ee0.arg = param_5;
    data_02150ee0.address = param_1;
    data_02150ee0.size = param_2;
    data_02150ee0.buf = param_3;
    data_02150ee0.unk_40 = 0;
    DC_InvalidateRange(data_02150ec0, 0x20);
    enabled = OS_DisableInterrupts();
    size = 0x20;
    if (param_2 <= 0x20) {
        size = param_2;
    }
    if (func_020843b4(param_1, size, data_02150ec0) == TRUE) {
        OS_RestoreInterrupts(enabled);
        return 0;
    }
    data_02150ee0.state = 0;
    OS_RestoreInterrupts(enabled);
    OS_WakeupThread(&data_02150ee0.queue);
    return 3;
}

void func_02083fa0(u32 param_1, BOOL param_2) {
    u32 result = 7;
    u16 size;
    NvramCallback callback;
    u8 kind = param_1;

    size = (data_02150ee0.size > 0x20) ? 0x20 : data_02150ee0.size;
    if (param_2 == 0 && kind == 0) {
        if (!func_020842a0(data_02150ec0, data_02150ee0.buf, size)) {
            result = 6;
        } else {
            u32 next;
            data_02150ee0.address += size;
            data_02150ee0.size -= size;
            data_02150ee0.buf += size;
            if (data_02150ee0.size != 0) {
                DC_InvalidateRange(data_02150ec0, 0x20);
                next = 0x20;
                if (data_02150ee0.size <= 0x20) {
                    next = data_02150ee0.size;
                }
                if (func_020843b4(data_02150ee0.address, next, data_02150ec0) == TRUE) {
                    return;
                }
                result = 3;
            } else {
                result = 0;
            }
        }
    } else if (param_2 == 1) {
        result = 3;
    } else {
        switch (kind) {
        case 2:
            result = 2;
            break;
        case 3:
            result = 4;
            break;
        case 4:
            DC_InvalidateRange(data_02150ec0, 0x20);
            if (func_020843b4(data_02150ee0.address, size, data_02150ec0) == TRUE) {
                return;
            }
            result = 3;
            break;
        }
    }

    data_02150ee0.state = 0;
    data_02150ee0.unk_40 = 0;
    callback = data_02150ee0.callback;
    if (callback != NULL) {
        data_02150ee0.callback = NULL;
        callback(result, data_02150ee0.arg);
    }
    OS_WakeupThread(&data_02150ee0.queue);
}

u32 func_020840f8(u32 param_1, u32 param_2, const void *param_3) {
    OSMessageQueue queue;
    void *message;
    OSMessage msgArray[1];
    u32 enabled;

    if (OSi_ThreadInfo.current == NULL) {
        return 4;
    }
    OS_InitMessageQueue(&queue, msgArray, 1);
    for (;;) {
        enabled = OS_DisableInterrupts();
        message = (OSMessage)(uintptr_t)func_02083eb8(param_1, param_2, param_3, func_02084260, &queue);
        if ((uintptr_t)message == 1) {
            OS_SleepThread(&data_02150ee0.queue);
        } else if ((uintptr_t)message == 3) {
            OS_Sleep(1);
        } else {
            OS_RestoreInterrupts(enabled);
            break;
        }
        OS_RestoreInterrupts(enabled);
    }
    if (message == NULL) {
        if (!OS_ReceiveMessage(&queue, &message, 1)) {
            OS_Terminate();
        }
    }
    return (u32)(uintptr_t)message;
}

BOOL func_020841e8(void) {
    return data_02150ea0 == 2;
}

void func_02084204(PXIFifoTag tag, uPtr data, BOOL err) {
    switch (data_02150ee0.state) {
    case 1:
        func_020837b8(data, err);
        break;
    case 2:
        func_020839e8(data, err);
        break;
    case 3:
        func_02083fa0(data, err);
        break;
    }
}

void func_02084260(u32 result, void *arg) {
    BOOL BVar1;

    BVar1 = OS_SendMessage(arg, (OSMessage)(uintptr_t)result, 0);
    if (BVar1 != 0) {
        return;
    }
    OS_Terminate();
}

void func_02084288(void *arg) {
    func_02084204(4, (u32)(uintptr_t)arg, 0);
}

BOOL func_020842a0(const void *a, const void *b, u32 size) {
    u32 i;

    for (i = 0; i < size; i++) {
        if (((const u8 *)a)[i] != ((const u8 *)b)[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL func_020842d8(void) {
    BOOL result = FALSE;
    if (PXI_SendWordByFifo(4, 0x3002000, 0) >= 0) {
        result = TRUE;
    }
    return result;
}

BOOL func_02084304(void) {
    BOOL result = FALSE;
    if (PXI_SendWordByFifo(4, 0x3002100, 0) >= 0) {
        result = TRUE;
    }
    return result;
}

BOOL func_02084330(void *param_1) {
    int tag = 4;
    BOOL result = FALSE;
    if (PXI_SendWordByFifo(tag, 0x2002200 | (NVRAM_PXI_ADDRESS(param_1) >> 24), result) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(tag, ((NVRAM_PXI_ADDRESS(param_1) >> 8) & 0xffff) | 0x10000, result) < 0) {
        return result;
    }
    result = (PXI_SendWordByFifo(tag, 0x1000000 | (((NVRAM_PXI_ADDRESS(param_1) << 8) & 0xff00) | 0x20000), result) >= 0) ? TRUE : result;
    return result;
}

BOOL func_020843b4(u32 param_1, u32 param_2, const void *param_3) {
    BOOL result = FALSE;
    if (PXI_SendWordByFifo(4, (((param_1 >> 16) & 0xff) | 0x2002300), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, (((param_1 << 16) >> 16) | 0x10000), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, ((param_2 >> 16) | 0x20000), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, (((param_2 << 16) >> 16) | 0x30000), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, ((NVRAM_PXI_ADDRESS(param_3) >> 16) | 0x40000), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, (((NVRAM_PXI_ADDRESS(param_3) << 16) >> 16) | 0x1050000), 0) >= 0) {
        result = TRUE;
    }
    return result;
}

BOOL func_020844a8(u32 param_1, u32 param_2, const void *param_3) {
    BOOL result = FALSE;
    if (PXI_SendWordByFifo(4, (((param_1 >> 16) & 0xff) | 0x2002500), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, (((param_1 << 16) >> 16) | 0x10000), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, (param_2 | 0x20000), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, ((NVRAM_PXI_ADDRESS(param_3) >> 16) | 0x30000), 0) < 0) {
        return result;
    }
    if (PXI_SendWordByFifo(4, (((NVRAM_PXI_ADDRESS(param_3) << 16) >> 16) | 0x1040000), 0) >= 0) {
        result = TRUE;
    }
    return result;
}

BOOL func_02084570(void) {
    BOOL result = FALSE;
    if (PXI_SendWordByFifo(4, 0x3002d00, 0) >= 0) {
        result = TRUE;
    }
    return result;
}
