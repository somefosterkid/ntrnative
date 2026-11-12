// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/rtc/src/external.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>
#include <nitro/rtc.h>
#include "system/convert.h"
#include "system/internal.h"

typedef enum RTCLock {
    RTC_LOCK_OFF = 0,
    RTC_LOCK_ON,
    RTC_LOCK_MAX
} RTCLock;

typedef enum RTCSequence {
    RTC_SEQ_GET_DATE = 0,
    RTC_SEQ_GET_TIME,
    RTC_SEQ_GET_DATETIME,
    RTC_SEQ_SET_DATE,
    RTC_SEQ_SET_TIME,
    RTC_SEQ_SET_DATETIME,
    RTC_SEQ_GET_ALARM1_STATUS,
    RTC_SEQ_GET_ALARM2_STATUS,
    RTC_SEQ_GET_ALARM_PARAM,
    RTC_SEQ_SET_ALARM1_STATUS,
    RTC_SEQ_SET_ALARM2_STATUS,
    RTC_SEQ_SET_ALARM1_PARAM,
    RTC_SEQ_SET_ALARM2_PARAM,
    RTC_SEQ_SET_HOUR_FORMAT,
    RTC_SEQ_SET_REG_STATUS2,
    RTC_SEQ_SET_REG_ADJUST,
    RTC_SEQ_MAX
} RTCSequence;

typedef struct RTCWork {
    u32 lock;
    RTCCallback callback;
    void * buffer[2];
    void * callbackArg;
    u32 sequence;
    u32 index;
    RTCInterrupt interrupt;
    RTCResult commonResult;
} RTCWork;

static u16 rtcInitialized;
static RTCWork rtcWork;
static u16 rtcTickInitialized;
static OSTick rtcInitialTotalTicks;

static u32 RtcBCD2HEX(u32 bcd);
static void RtcGetResultCallback(RTCResult result, void * arg);
static void RtcWaitBusy(void);
void func_02088a04(PXIFifoTag fifoTag, u32 data, BOOL err);
RTCResult func_02700000(void);


void RTC_Init(void)
{
    if (rtcInitialized != 0) {
        return;
    }

    rtcInitialized = 1;
    rtcWork.lock = 0;
    rtcWork.callback = NULL;
    rtcWork.interrupt = 0;
    rtcWork.buffer[0] = NULL;
    rtcWork.buffer[1] = NULL;

    PXI_Init();
    while (!PXI_IsCallbackReady(5, 1)) {
    }
    PXI_SetFifoRecvCallback(5, func_02088a04);
}

RTCResult RTC_GetDateAsync (RTCDate * date, RTCCallback callback, void * arg)
{
    OSIntrMode enabled;

    SDK_ASSERT(date != NULL);
    SDK_ASSERT(callback != NULL);

    enabled = OS_DisableInterrupts();

    if (rtcWork.lock != RTC_LOCK_OFF) {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }

    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    rtcWork.sequence = RTC_SEQ_GET_DATE;
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)date;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;

    if (RTCi_ReadRawDateAsync()) {
        return RTC_RESULT_SUCCESS;
    }

    rtcWork.lock = RTC_LOCK_OFF;
    return RTC_RESULT_SEND_ERROR;
}

RTCResult RTC_GetDate (RTCDate * date)
{
    rtcWork.commonResult = RTC_GetDateAsync(date, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS) {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

RTCResult RTC_GetTimeAsync (RTCTime * time, RTCCallback callback, void * arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(time);
    SDK_NULL_ASSERT(callback);

    enabled = OS_DisableInterrupts();

    if (rtcWork.lock != RTC_LOCK_OFF) {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }

    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    rtcWork.sequence = RTC_SEQ_GET_TIME;
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)time;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;

    if (RTCi_ReadRawTimeAsync()) {
        return RTC_RESULT_SUCCESS;
    }

    rtcWork.lock = RTC_LOCK_OFF;
    return RTC_RESULT_SEND_ERROR;
}

RTCResult RTC_GetTime (RTCTime * time)
{
    rtcWork.commonResult = RTC_GetTimeAsync(time, RtcGetResultCallback, NULL);

    if (rtcWork.commonResult == RTC_RESULT_SUCCESS) {
        RtcWaitBusy();
    }

    return rtcWork.commonResult;
}

RTCResult RTC_GetDateTimeAsync (RTCDate * date, RTCTime * time, RTCCallback callback, void * arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(date);
    SDK_NULL_ASSERT(time);
    SDK_NULL_ASSERT(callback);

    enabled = OS_DisableInterrupts();

    if (rtcWork.lock != RTC_LOCK_OFF) {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }

    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    rtcWork.sequence = RTC_SEQ_GET_DATETIME;
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)date;
    rtcWork.buffer[1] = (void *)time;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;

    if (RTCi_ReadRawDateTimeAsync()) {
        return RTC_RESULT_SUCCESS;
    }

    rtcWork.lock = RTC_LOCK_OFF;
    return RTC_RESULT_SEND_ERROR;
}

RTCResult RTC_GetDateTime (RTCDate * date, RTCTime * time)
{
    rtcWork.commonResult = RTC_GetDateTimeAsync(date, time, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS) {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

RTCWork *RTCi_GetSysWork(void)
{
    return &rtcWork;
}

static void func_02088a04(PXIFifoTag tag, u32 data, BOOL err) {
    RTCResult result;
    RTCPxiResult pxiresult;
    u8 command;
    RTCCallback cb;

    if (err) {
        if (rtcWork.index) {
            rtcWork.index = 0;
        }
        if (rtcWork.lock != RTC_LOCK_OFF) {
            rtcWork.lock = RTC_LOCK_OFF;
        }
        if (rtcWork.callback) {
            cb = rtcWork.callback;
            rtcWork.callback = NULL;
            cb(RTC_RESULT_FATAL_ERROR, rtcWork.callbackArg);
        }
        return;
    }
    command = (u8)((data & RTC_PXI_COMMAND_MASK) >> RTC_PXI_COMMAND_SHIFT);
    pxiresult = (RTCPxiResult)((data & RTC_PXI_RESULT_MASK) >> RTC_PXI_RESULT_SHIFT);
    if (command == RTC_PXI_COMMAND_INTERRUPT) {
        if (rtcWork.interrupt) {
            rtcWork.interrupt();
        }
        return;
    }
    if (pxiresult == RTC_PXI_RESULT_SUCCESS) {
        result = RTC_RESULT_SUCCESS;
        switch (rtcWork.sequence) {
        case RTC_SEQ_GET_DATE:
            {
                RTCDate *pDst = (RTCDate *)(rtcWork.buffer[0]);
                RTCRawDate *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.date);
                pDst->year = RtcBCD2HEX(pSrc->year);
                pDst->month = RtcBCD2HEX(pSrc->month);
                pDst->day = RtcBCD2HEX(pSrc->day);
                pDst->week = RTC_GetDayOfWeek(pDst);
            }
            break;
        case RTC_SEQ_GET_TIME:
            {
                RTCTime *pDst = (RTCTime *)(rtcWork.buffer[0]);
                RTCRawTime *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.time);
                pDst->hour = RtcBCD2HEX(pSrc->hour);
                pDst->minute = RtcBCD2HEX(pSrc->minute);
                pDst->second = RtcBCD2HEX(pSrc->second);
            }
            break;
        case RTC_SEQ_GET_DATETIME:
            {
                RTCDate *pDst = (RTCDate *)(rtcWork.buffer[0]);
                RTCRawDate *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.date);
                pDst->year = RtcBCD2HEX(*(u32 *)pSrc & 0x000000ff);
                pDst->month = RtcBCD2HEX(pSrc->month);
                pDst->day = RtcBCD2HEX(pSrc->day);
                pDst->week = RTC_GetDayOfWeek(pDst);
            }
            {
                RTCTime *pDst = (RTCTime *)(rtcWork.buffer[1]);
                RTCRawTime *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.time);
                pDst->hour = RtcBCD2HEX(pSrc->hour);
                pDst->minute = RtcBCD2HEX(pSrc->minute);
                pDst->second = RtcBCD2HEX(pSrc->second);
            }
            break;
        case RTC_SEQ_SET_DATE:
        case RTC_SEQ_SET_TIME:
        case RTC_SEQ_SET_DATETIME:
            break;
        case RTC_SEQ_GET_ALARM1_STATUS:
            {
                RTCAlarmStatus *pDst = (RTCAlarmStatus *)(rtcWork.buffer[0]);
                RTCRawStatus2 *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);
                switch (pSrc->intr_mode) {
                case RTC_INTERRUPT_MODE_ALARM:
                    *pDst = RTC_ALARM_STATUS_ON;
                    break;
                default:
                    *pDst = RTC_ALARM_STATUS_OFF;
                }
            }
            break;
        case RTC_SEQ_GET_ALARM2_STATUS:
            {
                RTCAlarmStatus *pDst = (RTCAlarmStatus *)(rtcWork.buffer[0]);
                RTCRawStatus2 *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);
                if (pSrc->intr2_mode) {
                    *pDst = RTC_ALARM_STATUS_ON;
                } else {
                    *pDst = RTC_ALARM_STATUS_OFF;
                }
            }
            break;
        case RTC_SEQ_GET_ALARM_PARAM:
            {
                RTCAlarmParam *pDst = (RTCAlarmParam *)(rtcWork.buffer[0]);
                RTCRawAlarm *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.alarm);
                pDst->week = (RTCWeek)(pSrc->week);
                pDst->hour = RtcBCD2HEX(pSrc->hour);
                pDst->minute = RtcBCD2HEX(pSrc->minute);
                pDst->enable = RTC_ALARM_ENABLE_NONE;
                if (pSrc->we) {
                    pDst->enable += RTC_ALARM_ENABLE_WEEK;
                }
                if (pSrc->he) {
                    pDst->enable += RTC_ALARM_ENABLE_HOUR;
                }
                if (pSrc->me) {
                    pDst->enable += RTC_ALARM_ENABLE_MINUTE;
                }
            }
            break;
        case RTC_SEQ_SET_ALARM1_STATUS:
            if (rtcWork.index == 0) {
                RTCRawStatus2 *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);
                if (*(RTCAlarmStatus *)(rtcWork.buffer[0]) == RTC_ALARM_STATUS_ON) {
                    if (pSrc->intr_mode != RTC_INTERRUPT_MODE_ALARM) {
                        rtcWork.index++;
                        pSrc->intr_mode = RTC_INTERRUPT_MODE_ALARM;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                } else {
                    if (pSrc->intr_mode != RTC_INTERRUPT_MODE_NONE) {
                        rtcWork.index++;
                        pSrc->intr_mode = RTC_INTERRUPT_MODE_NONE;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
            } else {
                rtcWork.index = 0;
            }
            break;
        case RTC_SEQ_SET_ALARM2_STATUS:
            if (rtcWork.index == 0) {
                RTCRawStatus2 *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);
                if (*(RTCAlarmStatus *)(rtcWork.buffer[0]) == RTC_ALARM_STATUS_ON) {
                    if (!pSrc->intr2_mode) {
                        rtcWork.index++;
                        pSrc->intr2_mode = 1;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                } else {
                    if (pSrc->intr2_mode) {
                        rtcWork.index++;
                        pSrc->intr2_mode = 0;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
            } else {
                rtcWork.index = 0;
            }
            break;
        case RTC_SEQ_SET_ALARM1_PARAM:
        case RTC_SEQ_SET_ALARM2_PARAM:
        case RTC_SEQ_SET_HOUR_FORMAT:
        case RTC_SEQ_SET_REG_STATUS2:
        case RTC_SEQ_SET_REG_ADJUST:
            break;
        default:
            if (OS_IsRunOnTwl() != TRUE) {
                rtcWork.index = 0;
                result = RTC_RESULT_INVALID_COMMAND;
            } else {
                result = func_02700000();
            }
        }
    } else {
        rtcWork.index = 0;
        switch (pxiresult) {
        case RTC_PXI_RESULT_INVALID_COMMAND:
            result = RTC_RESULT_INVALID_COMMAND;
            break;
        case RTC_PXI_RESULT_ILLEGAL_STATUS:
            result = RTC_RESULT_ILLEGAL_STATUS;
            break;
        case RTC_PXI_RESULT_BUSY:
            result = RTC_RESULT_BUSY;
            break;
        case RTC_PXI_RESULT_FATAL_ERROR:
        default:
            result = RTC_RESULT_FATAL_ERROR;
        }
    }
    if (rtcWork.index == 0) {
        if (rtcWork.lock != RTC_LOCK_OFF) {
            rtcWork.lock = RTC_LOCK_OFF;
        }
        if (rtcWork.callback) {
            cb = rtcWork.callback;
            rtcWork.callback = NULL;
            cb(result, rtcWork.callbackArg);
        }
    }
}



static u32 RtcBCD2HEX(u32 bcd)
{
    u32 hex = 0;
    s32 i;
    s32 w;

    for (i = 0; i < 8; i++) {
        if (((bcd >> (i * 4)) & 0x0000000f) >= 0x0a) {
            return hex;
        }
    }

    for (i = 0, w = 1; i < 8; i++, w *= 10) {
        hex += (((bcd >> (i * 4)) & 0x0000000f) * w);
    }

    return hex;
}

static void RtcGetResultCallback (RTCResult result, void * arg)
{
#pragma unused(arg)

    rtcWork.commonResult = result;
}

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): until the ARM7 ends the command
static void RtcWaitBusy(void)
{
    while (rtcWork.lock == RTC_LOCK_ON) {
    }
}
#else
static asm void RtcWaitBusy (void)
{
    ldr r12, = rtcWork.lock
loop:
    ldr r0, [r12, #0]
    cmp r0, #RTC_LOCK_ON
    beq loop
    bx lr
}
#endif

#include <nitro/codereset.h>
