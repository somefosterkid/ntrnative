// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/spi/src/mic.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include    <nitro/spi.h>

typedef enum MICLock {
    MIC_LOCK_OFF = 0,
    MIC_LOCK_ON,
    MIC_LOCK_MAX
} MICLock;

typedef struct MICWork {
    MICLock lock;
    MICCallback callback;
    void * callbackArg;
    MICResult commonResult;
    MICCallback full;
    void * fullArg;
    void * dst_buf;
} MICWork;

// pokebw2's micInitialized
static u16 data_02152fe0;
// pokebw2's micWork
static MICWork data_02152fe4;

MICResult MIC_StopLimitedSampling(void);
void MicCommonCallback(PXIFifoTag tag, uPtr data, BOOL err);
static BOOL MicStartAutoSampling(void * buf, u32 size, u32 span, u8 flags);
static BOOL MicStopAutoSampling(void);

BOOL OSi_IsCodecTwlMode(void);
MICResult MICEXi_StartLimitedSampling(const MICAutoParam *param);
MICResult MICEXi_StartLimitedSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg);
MICResult MICEXi_StopLimitedSampling(void);
MICResult MIC_StartAutoSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg);
void MicGetResultCallback(MICResult result, void *arg);
void MicWaitBusy(void);
MICWork *MICi_GetSysWork(void);
MICResult MIC_StartAutoSampling(const MICAutoParam *param);
MICResult MIC_StopAutoSampling(void);



void MIC_Init(void)
{
    if (data_02152fe0 != 0) {
        return;
    }
    data_02152fe0 = 1;
    data_02152fe4.lock = MIC_LOCK_OFF;
    data_02152fe4.callback = NULL;

    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_MIC, PXI_PROC_ARM7)) {
    }
    OS_GetSystemWork()->mic_last_address = 0;
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_MIC, MicCommonCallback);
}

// From libntr (MIT, https://github.com/cybervisi0n/libntr), libraries/spi/src/mic.c: ported as it is, with credit (tools/ref_port.py)
MICResult MIC_StartAutoSamplingAsync (const MICAutoParam * param, MICCallback callback, void * arg)
{
    OSIntrMode enabled;
    u8 flags;


    {

        if ((u32)(param->buffer) & 0x01f) {
            return MIC_RESULT_ILLEGAL_PARAMETER;
        }

        if (param->size & 0x01f) {
            return MIC_RESULT_ILLEGAL_PARAMETER;
        }

        if (param->size <= 0) {
            return MIC_RESULT_ILLEGAL_PARAMETER;
        }

        if (param->rate < MIC_SAMPLING_RATE_LIMIT) {
            return MIC_RESULT_ILLEGAL_PARAMETER;
        }

        switch (param->type) {
        case MIC_SAMPLING_TYPE_8BIT:
            flags = SPI_MIC_SAMPLING_TYPE_8BIT;
            break;
        case MIC_SAMPLING_TYPE_12BIT:
            flags = SPI_MIC_SAMPLING_TYPE_12BIT;
            break;
        case MIC_SAMPLING_TYPE_SIGNED_8BIT:
            flags = SPI_MIC_SAMPLING_TYPE_S8BIT;
            break;
        case MIC_SAMPLING_TYPE_SIGNED_12BIT:
            flags = SPI_MIC_SAMPLING_TYPE_S12BIT;
            break;
        case MIC_SAMPLING_TYPE_12BIT_FILTER_OFF:
            flags = (SPI_MIC_SAMPLING_TYPE_12BIT | SPI_MIC_SAMPLING_TYPE_FILTER_OFF);
            break;
        case MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF:
            flags = (SPI_MIC_SAMPLING_TYPE_S12BIT | SPI_MIC_SAMPLING_TYPE_FILTER_OFF);
            break;
        default:
            return MIC_RESULT_ILLEGAL_PARAMETER;
        }

        if (param->loop_enable) {
            flags = (u8)(flags | SPI_MIC_SAMPLING_TYPE_LOOP_ON);
        } else {
            flags = (u8)(flags | SPI_MIC_SAMPLING_TYPE_LOOP_OFF);
        }

        flags = (u8)(flags | SPI_MIC_SAMPLING_TYPE_CORRECT_OFF);
    }

    enabled = OS_DisableInterrupts();
    if (data_02152fe4.lock != MIC_LOCK_OFF) {
        (void)OS_RestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }
    data_02152fe4.lock = MIC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    data_02152fe4.callback = callback;
    data_02152fe4.callbackArg = arg;
    data_02152fe4.full = param->full_callback;
    data_02152fe4.fullArg = param->full_arg;
    if (MicStartAutoSampling(param->buffer, param->size, param->rate, flags) == FALSE) {
        data_02152fe4.lock = MIC_LOCK_OFF;
        return MIC_RESULT_SEND_ERROR;
    }
    return MIC_RESULT_SUCCESS;
}


MICResult MIC_StartAutoSampling(const MICAutoParam *param)
{
    data_02152fe4.commonResult = MIC_StartAutoSamplingAsync(param, MicGetResultCallback, NULL);
    if (data_02152fe4.commonResult == MIC_RESULT_SUCCESS) {
        MicWaitBusy();
    }
    return data_02152fe4.commonResult;
}

MICResult MIC_StopAutoSamplingAsync(MICCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(callback);

    enabled = OS_DisableInterrupts();
    if (data_02152fe4.lock != MIC_LOCK_OFF) {
        (void)OS_RestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }
    data_02152fe4.lock = MIC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    data_02152fe4.callback = callback;
    data_02152fe4.callbackArg = arg;
    if (MicStopAutoSampling() == FALSE) {
        data_02152fe4.lock = MIC_LOCK_OFF;
        return MIC_RESULT_SEND_ERROR;
    }
    return MIC_RESULT_SUCCESS;
}

MICResult MIC_StopAutoSampling(void)
{
    data_02152fe4.commonResult = MIC_StopAutoSamplingAsync(MicGetResultCallback, NULL);
    if (data_02152fe4.commonResult == MIC_RESULT_SUCCESS) {
        MicWaitBusy();
    }
    return data_02152fe4.commonResult;
}


void *MIC_GetLastSamplingAddress(void)
{
    return (void *)(OS_GetSystemWork()->mic_last_address);
}

MICResult MIC_StartLimitedSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg)
{
    if (OSi_IsCodecTwlMode() == 1) {
        return MICEXi_StartLimitedSamplingAsync(param, callback, arg);
    }
    return MIC_StartAutoSamplingAsync(param, callback, arg);
}


MICResult MIC_StartLimitedSampling(const MICAutoParam *param)
{
    if (OSi_IsCodecTwlMode() == 1) {
        return MICEXi_StartLimitedSampling(param);
    }
    return MIC_StartAutoSampling(param);
}

MICResult MIC_StopLimitedSampling(void)
{
    if (OSi_IsCodecTwlMode() == 1) {
        return MICEXi_StopLimitedSampling();
    }
    return MIC_StopAutoSampling();
}

void MicCommonCallback(PXIFifoTag tag, uPtr data, BOOL err)
{
    u16 command;
    u16 pxiresult;
    MICResult result;
    MICCallback cb;

    if (err) {
        if (data_02152fe4.lock != MIC_LOCK_OFF) {
            data_02152fe4.lock = MIC_LOCK_OFF;
        }
        if (data_02152fe4.callback) {
            cb = data_02152fe4.callback;
            data_02152fe4.callback = NULL;
            cb(MIC_RESULT_FATAL_ERROR, data_02152fe4.callbackArg);
        }
    }

    command = (u16)((data & 0x7f00) >> 8);
    pxiresult = (u16)(data & 0xff);

    switch (pxiresult) {
    case 0:
        result = MIC_RESULT_SUCCESS;
        break;
    case 1:
        result = MIC_RESULT_INVALID_COMMAND;
        break;
    case 2:
        result = MIC_RESULT_ILLEGAL_PARAMETER;
        break;
    case 3:
        result = MIC_RESULT_ILLEGAL_STATUS;
        break;
    case 4:
        result = MIC_RESULT_BUSY;
        break;
    default:
        result = MIC_RESULT_FATAL_ERROR;
    }

    if (command == 0x51) {
        if (data_02152fe4.full) {
            data_02152fe4.full(result, data_02152fe4.fullArg);
        }
    } else {
        if (command == 0x40) {
            if (data_02152fe4.dst_buf) {
                *(u16 *)(data_02152fe4.dst_buf) = OS_GetSystemWork()->mic_sampling_data;
            }
        }

        if (data_02152fe4.lock != MIC_LOCK_OFF) {
            data_02152fe4.lock = MIC_LOCK_OFF;
        }

        if (data_02152fe4.callback) {
            cb = data_02152fe4.callback;
            data_02152fe4.callback = NULL;
            cb(result, data_02152fe4.callbackArg);
        }
    }
}

static BOOL MicStartAutoSampling (void * buf, u32 size, u32 span, u8 flags)
{
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_MIC_AUTO_ON << 8) | (u32)flags, 0)) {
        return FALSE;
    }

    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (1 << SPI_PXI_INDEX_SHIFT) | ((u32)buf >> 16), 0)) {
        return FALSE;
    }

    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               (2 << SPI_PXI_INDEX_SHIFT) | ((u32)buf & 0x0000ffff), 0)) {
        return FALSE;
    }

    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (3 << SPI_PXI_INDEX_SHIFT) | (size >> 16), 0)) {
        return FALSE;
    }

    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               (4 << SPI_PXI_INDEX_SHIFT) | (size & 0x0000ffff), 0)) {
        return FALSE;
    }

    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (5 << SPI_PXI_INDEX_SHIFT) | (span >> 16), 0)) {
        return FALSE;
    }

    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_END_BIT |
                               (6 << SPI_PXI_INDEX_SHIFT) | (span & 0x0000ffff), 0)) {
        return FALSE;
    }

    return TRUE;
}

static BOOL MicStopAutoSampling (void)
{
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_MIC_AUTO_OFF << 8), 0)) {
        return FALSE;
    }

    return TRUE;
}

void MicGetResultCallback(MICResult result, void *arg)
{
    data_02152fe4.commonResult = result;
}

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): until the ARM7 ends the command
void MicWaitBusy(void)
{
    while (*(volatile MICLock *)&data_02152fe4.lock == MIC_LOCK_ON) {
    }
}
#else
asm void MicWaitBusy(void)
{
    ldr r12, = data_02152fe4.lock
@loop:
    ldr r0, [r12, #0]
    cmp r0, #MIC_LOCK_ON
    beq @loop
    bx lr
}
#endif

#include <nitro/codereset.h>

MICWork *MICi_GetSysWork(void)
{
    return &data_02152fe4;
}
