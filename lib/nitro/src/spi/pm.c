// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/spi/src/pm.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). TwlSDK 5 builds it as one object whose DSi part, in
// twl/ltdmain_begin.h blocks, goes to the LTD autoload's .ltdmain 0x027313ac-0x02731754 and .ltdmain.bss
// 0x02732fa4-0x02732fc0 (ltd_autoload_0's delinks.txt names it pm@ltd.c: tools/two_modules.py); that part reaches
// PMi_ExitFactor and PMi_AutoExitFlag from main's .bss and .data like any static of the object.
#include <nitro/spi/ARM9/pm.h>
#include <nitro/pxi.h>
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/spi/common/config.h>
#include <nitro/ctrdg.h>
#include <nitro/mb.h>
#include <nitro/os/common/systemWork.h>

/* Black's own symbols with no header yet; their files are not known. */
void MI_StopAllDma(void);
void MI_StopAllNDma(void);

typedef struct {
    BOOL lock;
    PMCallback callback;
    void * callbackArg;
    void * work;
} PMiWork;

#define PMi_LCD_POWER_WAIT_MSEC  150
#define PMi_LCD_POWER_WAIT_TICK  (OS_MilliSecondsToTicks(PMi_LCD_POWER_WAIT_MSEC) * (64 * 2))
#define PMi_LCD_SLEEP_WAIT_MSEC  110
#define PMi_LCD_SLEEP_WAIT_TICK  (OS_MilliSecondsToTicks(PMi_LCD_SLEEP_WAIT_MSEC) * (64 * 2))

#define PMi_STAT_BATTERY_MASK    1

#define PMi_SetCallback(callback, arg)     \
    do {                                   \
        PMi_Work.callback = (callback);    \
        PMi_Work.callbackArg = (arg);      \
    } while (0)

void func_0208b950(PXIFifoTag tag, uPtr data, BOOL err);
void func_0208c080(u32 data);
void func_0208c89c(void);

inline u32 PMi_MakeData1 (u32 bit, u32 seq, u32 command, u32 data)
{
    return (bit) | ((seq) << SPI_PXI_INDEX_SHIFT) | ((command) << 8) | ((data) & 0xff);
}

inline u32 PMi_MakeData2 (u32 bit, u32 seq, u32 data)
{
    return (bit) | ((seq) << SPI_PXI_INDEX_SHIFT) | ((data) & 0xffff);
}

// What PMi_WaitBusy checks to know it must take the PXI interrupt itself (PMi_WaitBusyMethod)
#define PMi_WAITBUSY_METHOD_CPUMODE (1 << 1)
#define PMi_WAITBUSY_METHOD_CPSR    (1 << 2)
#define PMi_WAITBUSY_METHOD_IME     (1 << 3)

void func_0208b840(u32 result, void * arg);
u32 func_0208be00(PMCallback callback, u32 *result);
void func_0208b7bc(void);
static u32 PMi_SetAmp(PMAmpSwitch status);

// How long the retries wait: TwlSDK's HW_CPU_CLOCK_ARM9 / 100, a hundredth of a second, with Black's OS_GetTick
#define PMi_WAIT_RETRY (HW_CPU_CLOCK_ARM7 / 100)
// How long the LCD waits before it is turned on or off, and between tries to turn it on (libntr's is 10)
#define PMi_LCD_WAIT_SYS_CYCLES 0x360000
#define PMi_PXI_WAIT_TICK 5
// How many frames the LCD stays off at least
#define PMi_WAIT_FRAME 7
// What PMi_TryToSendPxiDataTillSuccess's result is until the ARM7 answers
#define PMi_UNUSED_RESULT 0xffff0000

// TwlSDK's: the word the ARM7 reads to know whether a reset is allowed
#define HW_RESET_LOCK_FLAG_BUF 0x02fff684

// The DSi part's (TwlSDK 5's twl/ltdmain_begin.h blocks, in the LTD autoload's .ltdmain): the ARM7's utility requests
// (the status reads, here the battery level, and the power-off and the restart)
#define PM_UTIL_GET_STATUS 0xf
#define PM_UTIL_FORCE_RESET_HARDWARE 0x13
#define PM_UTIL_FORCE_EXIT 0x14
#define PM_UTIL_PARAM_BATTERY_LEVEL 1
// TwlSDK's HW_PARAM_LAUNCH_PARAM: the launcher's parameter, whose magic code a forced exit clears
#define HW_PARAM_LAUNCH_PARAM 0x02000300
// TwlSDK's HW_INIT_LOCK_BUF of the DSi's shared main memory, and its sync value (OSi_SetSyncValue)
#define HW_INIT_LOCK_BUF_TWL 0x02fffff0
#define OSi_SYNCTYPE_SENDER 0
#define OSi_SYNCTYPE_RECVER 1
#define OSi_SYNCVAL_NOT_READY 0
#define OSi_SYNCVAL_READY 1
// TwlSDK 5's, which this file's SDK 4 headers lack: the handshake with the ARM7 (os_spinLock.c) and the ARM9's end
// (os_terminate_proc.c)
void OSi_SyncWithOtherProc(int type, void *syncBuf);
void OSi_TerminateCore(void);
// The waits of the exit: between two reads of the vertical blank count, and before a request is sent again
#define PM_VBLANK_WAIT_CYCLES 0x32
#define PM_RETRY_CYCLES 0x51d23
// TwlSDK 5's, which this file's SDK 4 headers lack: the exit callbacks, which are the sleep ones' type, where a
// callback goes in a list, and the new DMA's registers (twl/mi/common/dma.h), which PMi_StopNDma reads
#define PMExitCallbackInfo PMSleepCallbackInfo
#define PMBatteryLowCallbackInfo PMSleepCallbackInfo
#define PM_CALLBACK_PRIORITY_MAX 255
#define PM_INSERT_APPEND 0
#define REG_NDMA0SAD_ADDR 0x04004104
#define MI_NDMA_REG_CNT_WOFFSET 6
#define MI_NDMA_REG(ndmaNo, reg) (*((vu32 *)(REG_NDMA0SAD_ADDR + (ndmaNo) * 0x1c + (reg) * 4)))
#define MI_NDMA_CONTINUOUS_ON (1UL << 29)
#define MI_NDMA_IMM_MODE_ON (1UL << 28)
#define MI_NDMA_TIMING_MASK (0xfUL << 24)
#define MI_NDMA_TIMING_CARD (4UL << 24)
#define MI_NDMA_TIMING_DISP (8UL << 24)
#define MI_NDMA_TIMING_DISP_MMEM (9UL << 24)
#define MI_NDMA_TIMING_CAMERA (11UL << 24)
void MI_WaitNDma(u32 ndmaNo);
void MI_StopNDma(u32 ndmaNo);

// The .bss and .data (and the DSi part's .ltdmain.bss), in TwlSDK 5's order and with its initializers (libntr's):
// MWCC heapsorts all of the object's at once, the initialized ones first in their order, then the others last first,
// which gives Black's layout of all three sections
static PMiWork PMi_Work;
static volatile BOOL PMi_SleepEndFlag;
static u32 PMi_LCDCount;
static u32 PMi_DispOffCount;
static PMSleepCallbackInfo * PMi_PreSleepCallbackList = NULL;
static PMSleepCallbackInfo * PMi_PostSleepCallbackList = NULL;

#include <twl/ltdmain_begin.h>
// The DSi's exit callbacks, before and after the exit's own work, and the low battery callback
static PMExitCallbackInfo *PMi_PreExitCallbackList = NULL;
PMExitCallbackInfo *PMi_PostExitCallbackList = NULL;
PMBatteryLowCallbackInfo PMi_BatteryLowCallbackInfo = {NULL, NULL, 0, NULL};
#include <twl/ltdmain_end.h>

static PMAmpSwitch sAmpSwitch = PM_AMP_OFF;
// TwlSDK's PMi_AutoExitFlag: whether the exit goes on by itself once the pre-exit callbacks have run
static BOOL PMi_AutoExitFlag = TRUE;
static PMExitFactor PMi_ExitFactor = PM_EXIT_FACTOR_NONE;
// The DMA control words PMi_StopDma saved, one per channel
static u32 PMi_PreDmaCnt[4];
static u32 PMi_WaitBusyMethod = PMi_WAITBUSY_METHOD_CPUMODE;

// TwlSDK's PM_ReadyToExit, PMi_FinalizeDebugger, PMi_CallPostExitCallbackAndReset and PMi_TryLockForReset
void func_027314f0(void);
static void func_02731500(void);
static void PMi_ExecuteExit(BOOL isExit);
static BOOL PMi_TryLockExit(void);
u16 OS_GetBootType(void);
static void func_0208b87c(void);
u32 func_0208bbb8(u16 trigger, u16 keyIntrData);
BOOL func_0208c44c(PMLCDPower sw, PMLEDStatus led, BOOL skip, BOOL isSync);
u32 func_0208bcc4(PMLEDStatus status, PMCallback callback, void * arg);

extern void PXIi_HandlerRecvFifoNotEmpty(void);




void func_0208b7bc(void)
{
    volatile BOOL * p = &PMi_Work.lock;

    while (*p) {
        if (((PMi_WaitBusyMethod & PMi_WAITBUSY_METHOD_CPUMODE) && OS_GetProcMode() == OS_PROCMODE_IRQ)
            || ((PMi_WaitBusyMethod & PMi_WAITBUSY_METHOD_CPSR) && OS_GetCpsrIrq() == OS_INTRMODE_IRQ_DISABLE)
            || ((PMi_WaitBusyMethod & PMi_WAITBUSY_METHOD_IME) && reg_OS_IME == 0)) {
            PXIi_HandlerRecvFifoNotEmpty();
        }
    }
}

void func_0208b840(u32 result, void * arg)
{
    if (arg) {
        *(u32 *)arg = result;
    }
}

void func_0208b84c(u32 result)
{
    PMCallback callback = PMi_Work.callback;
    void * arg = PMi_Work.callbackArg;

    PMi_Work.lock = FALSE;
    if (callback) {
        PMi_Work.callback = NULL;
        callback(result, arg);
    }
}

void func_0208b87c(void)
{
    vu32 vcount = OS_GetVBlankCount();

    while (vcount == OS_GetVBlankCount()) {
    }
}

void PM_Init(void)
{
    static u16 PMi_IsInit = FALSE;

    if (PMi_IsInit) {
        return;
    }

    PMi_IsInit = TRUE;
    PMi_Work.lock = FALSE;
    PMi_Work.callback = NULL;
    *(u32 *)HW_RESET_LOCK_FLAG_BUF = PM_RESET_FLAG_NONE;

    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_PM, PXI_PROC_ARM7)) {
        SVC_WaitByLoop(100);
    }

    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_PM, func_0208b950);

    PMi_LCDCount = PMi_DispOffCount = OS_GetVBlankCount();
}

void func_0208b950(PXIFifoTag tag, uPtr data, BOOL err)
{
#pragma unused(tag)
    u16 command;
    u16 pxiResult;
    BOOL callCallback = TRUE;

    command = (u16)((data & SPI_PXI_RESULT_COMMAND_MASK) >> SPI_PXI_RESULT_COMMAND_SHIFT);
    pxiResult = (u16)((data & SPI_PXI_RESULT_DATA_MASK) >> SPI_PXI_RESULT_DATA_SHIFT);

    if (err) {
        switch (command) {
        case SPI_PXI_COMMAND_PM_SLEEP_START:
        case SPI_PXI_COMMAND_PM_UTILITY:
            pxiResult = PM_RESULT_BUSY;
            break;
        default:
            pxiResult = PM_RESULT_ERROR;
        }
        func_0208b84c(pxiResult);
        return;
    }

    switch (command) {
    case SPI_PXI_COMMAND_PM_SLEEP_START:
        break;
    case SPI_PXI_COMMAND_PM_UTILITY:
        if (PMi_Work.work) {
            *(u16 *)PMi_Work.work = pxiResult;
        }
        pxiResult = PM_RESULT_SUCCESS;
        break;
    case SPI_PXI_COMMAND_PM_SYNC:
        pxiResult = PM_RESULT_SUCCESS;
        break;
    case SPI_PXI_COMMAND_PM_SLEEP_END:
        PMi_SleepEndFlag = TRUE;
        break;
    case SPI_PXI_COMMAND_PM_NOTIFY:
        switch (pxiResult) {
        case PM_NOTIFY_POWER_SWITCH:
            PMi_ProcessExitRequest(PM_EXIT_FACTOR_PWSW);
            *(u32 *)HW_RESET_LOCK_FLAG_BUF = PM_RESET_FLAG_FORCED;
            break;
        case PM_NOTIFY_SHUTDOWN:
            break;
        case PM_NOTIFY_RESET_HARDWARE:
            break;
        case PM_NOTIFY_BATTERY_LOW:
            if (PMi_BatteryLowCallbackInfo.callback) {
                PMi_BatteryLowCallbackInfo.callback(PMi_BatteryLowCallbackInfo.arg);
            }
            break;
        case PM_NOTIFY_BATTERY_EMPTY:
            PMi_ProcessExitRequest(PM_EXIT_FACTOR_BATTERY);
            *(u32 *)HW_RESET_LOCK_FLAG_BUF = PM_RESET_FLAG_FORCED;
            break;
        default:
            break;
        }
        callCallback = FALSE;
        break;
    }

    if (callCallback) {
        func_0208b84c(pxiResult);
    }
}

u32 func_0208ba70(u32 * sendData, int num, u16 * retValue, PMCallback callback, void * arg)
{
    int n;
    OSIntrMode enabled = OS_DisableInterrupts();

    if (PMi_Work.lock) {
        (void)OS_RestoreInterrupts(enabled);
        return PM_BUSY;
    }
    PMi_Work.lock = TRUE;
    PMi_Work.work = (void *)retValue;
    PMi_Work.callback = callback;
    PMi_Work.callbackArg = arg;

    for (n = 0; n < num; n++) {
        func_0208c080(sendData[n]);
    }

    (void)OS_RestoreInterrupts(enabled);
    return PM_SUCCESS;
}

void func_0208baf4(u32 * sendData, int num)
{
    volatile u32 result;

    while (1) {
        result = PMi_UNUSED_RESULT;
        while (func_0208ba70(sendData, num, NULL, func_0208b840, (void *)&result) != PM_SUCCESS) {
            OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
        }
        while (result == PMi_UNUSED_RESULT) {
            OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
        }
        if (result == SPI_PXI_RESULT_SUCCESS) {
            break;
        }
        OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
    }
}

u32 func_0208bbb8(u16 trigger, u16 keyIntrData)
{
    u32 sendData[2];

    sendData[0] = PMi_MakeData1(SPI_PXI_START_BIT | SPI_PXI_END_BIT, 0, SPI_PXI_COMMAND_PM_SYNC, 0);
    func_0208baf4(sendData, 1);

    while (func_0208c44c(PM_LCD_POWER_OFF, PM_LED_BLINK_LOW, FALSE, TRUE) != TRUE) {
    }

    sendData[0] = PMi_MakeData1(SPI_PXI_START_BIT, 0, SPI_PXI_COMMAND_PM_SLEEP_START, trigger);
    sendData[1] = PMi_MakeData2(SPI_PXI_END_BIT, 1, keyIntrData);
    func_0208baf4(sendData, 2);

    return PM_SUCCESS;
}

u32 func_0208bc44(u32 number, u16 parameter, u16 * retValue, PMCallback callback, void * arg)
{
    u32 sendData[2];

    sendData[0] = PMi_MakeData1(SPI_PXI_START_BIT, 0, SPI_PXI_COMMAND_PM_UTILITY, number);
    sendData[1] = PMi_MakeData2(SPI_PXI_END_BIT, 1, parameter);
    return func_0208ba70(sendData, 2, retValue, callback, arg);
}

u32 PMi_ReadRegister(u32 number, u16 parameter, u16 * retValue)
{
    u32 commandResult;
    u32 sendResult = func_0208bc44(number, parameter, retValue, func_0208b840, &commandResult);

    if (sendResult == PM_SUCCESS) {
        func_0208b7bc();
        return commandResult;
    }
    return sendResult;
}

u32 func_0208bcc4(PMLEDStatus status, PMCallback callback, void * arg)
{
    u32 command;

    switch (status) {
    case PM_LED_ON:
        command = PM_UTIL_LED_ON;
        break;
    case PM_LED_BLINK_HIGH:
        command = PM_UTIL_LED_BLINK_HIGH_SPEED;
        break;
    case PM_LED_BLINK_LOW:
        command = PM_UTIL_LED_BLINK_LOW_SPEED;
        break;
    default:
        command = 0;
    }

    return (command) ? func_0208bc44(command, 0, NULL, callback, arg) : PM_INVALID_COMMAND;
}











u32 PM_SendUtilityCommand (u32 number)
{
    u32 done;
    u32 result = func_0208bcc4(number, func_0208b840, &done);

    if (result != 0) {
        return result;
    }
    func_0208b7bc();
    return done;
}

u32 PM_SetBackLightAsync (PMLCDTarget target, PMBackLightSwitch sw, PMCallback callback, void * arg)
{
    u32 command = 0;

    if (target == PM_LCD_TOP) {
        if (sw == PM_BACKLIGHT_ON) {
            command = PM_UTIL_LCD2_BACKLIGHT_ON;
        }
        if (sw == PM_BACKLIGHT_OFF) {
            command = PM_UTIL_LCD2_BACKLIGHT_OFF;
        }
    } else if (target == PM_LCD_BOTTOM) {
        if (sw == PM_BACKLIGHT_ON) {
            command = PM_UTIL_LCD1_BACKLIGHT_ON;
        }
        if (sw == PM_BACKLIGHT_OFF) {
            command = PM_UTIL_LCD1_BACKLIGHT_OFF;
        }
    } else if (target == PM_LCD_ALL) {
        if (sw == PM_BACKLIGHT_ON) {
            command = PM_UTIL_LCD12_BACKLIGHT_ON;
        }
        if (sw == PM_BACKLIGHT_OFF) {
            command = PM_UTIL_LCD12_BACKLIGHT_OFF;
        }
    }

    if (command == 0) {
        return 0xffff;
    }
    return func_0208bc44(command, 0, 0, callback, arg);
}

u32 PM_SetBackLight(PMLCDTarget target, PMBackLightSwitch sw) {
    u32 result;
    u32 value = PM_SetBackLightAsync(target, sw, func_0208b840, &result);

    if (value != 0) {
        return value;
    }
    func_0208b7bc();
    return result;
}

u32 func_0208be00(PMCallback callback, u32 *result)
{
    PMi_ExitFactor = PM_EXIT_FACTOR_USER;
    func_0208c89c();
    if (OS_IsRunOnTwl()) {
        PMi_ExecuteList(PMi_PostExitCallbackList);
    }
    return func_0208bc44(0xe, 0, 0, callback, result);
}

u32 func_0208be58(void)
{
    u32 commandResult;
    u32 sendResult = func_0208be00(func_0208b840, &commandResult);

    if (sendResult == PM_SUCCESS) {
        PMi_WaitBusyMethod = PMi_WAITBUSY_METHOD_CPSR | PMi_WAITBUSY_METHOD_IME;
        func_0208b7bc();
        PMi_WaitBusyMethod = PMi_WAITBUSY_METHOD_CPUMODE;
        return commandResult;
    }

    return sendResult;
}

void PM_ForceToPowerOff(void)
{
    if (func_0208be58() != 0) {
        do {
            OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
        } while (func_0208be58() != 0);
    }
    OS_DisableInterrupts();
    MI_StopAllDma();
    if (OS_IsRunOnTwl()) {
        MI_StopAllNDma();
    }
    while (TRUE) {
        OS_Halt();
    }
}





u32 PM_SetAmp (PMAmpSwitch status)
{
    sAmpSwitch = status;
    return PMi_SetAmp(status);
}

static u32 PMi_SetAmp (PMAmpSwitch status)
{
    if (PM_GetLCDPower()) {
        return PMi_ReadRegister(0x10, (u16)status, 0);
    } else {
        return PM_RESULT_SUCCESS;
    }
}

u32 PM_SetAmpGain(PMAmpGain gain) {
    return PMi_ReadRegister(0x11, (u16)gain, 0);
}


u32 PM_SetAmpGainLevel (u8 level)
{
    return PMi_ReadRegister(0x16, level, NULL);
}

u32 PM_GetBattery(PMBattery * batteryBuf) {
    u16 data;
    u32 result;
    u32 charge = 0;

    result = PMi_ReadRegister(0xf, 0, &data);
    if (result != 0) {
        return result;
    }
    if (batteryBuf == NULL) {
        return result;
    }
    if (data != 0) {
        charge = 1;
    }
    *batteryBuf = charge;
    return result;
}


#include <twl/ltdmain_begin.h>
u32 PMi_GetBatteryLevelCore(u16 *level) {
    u16 status;
    u32 result = PMi_ReadRegister(PM_UTIL_GET_STATUS, PM_UTIL_PARAM_BATTERY_LEVEL, &status);

    if (result == PM_RESULT_SUCCESS) {
        if (level) {
            *level = status;
        }
    }
    return result;
}
#include <twl/ltdmain_end.h>

u32 PM_GetBatteryLevel(u16 *level) {
    if (OS_IsRunOnTwl() == FALSE) {
        return 2;
    }
    return PMi_GetBatteryLevelCore(level);
}

u32 PM_GetBackLight(PMBackLightSwitch * target, PMBackLightSwitch * backlight) {
    u16 status;
    u32 result;

    result = PMi_ReadRegister(0xf, 3, &status);
    if (result != 0) {
        return result;
    }
    if (target != NULL) {
        *target = (status & 8) ? 1 : 0;
    }
    if (backlight != NULL) {
        *backlight = (status & 4) ? 1 : 0;
    }
    return result;
}


u32 PM_GetAmp(PMAmpSwitch *swBuf) {
    u16 data;
    u32 result;

    result = PMi_ReadRegister(0xf, 6, &data);
    if (result != 0) {
        return result;
    }
    if (swBuf == NULL) {
        return result;
    }
    *swBuf = (data != 0) ? PM_AMP_ON : PM_AMP_OFF;
    return result;
}

void func_0208c080(u32 arg) {
    while (PXI_SendWordByFifo(8, arg, 0) != 0) {
    }
}

#include <twl/ltdmain_begin.h>
void PMi_StopNDma(u32 ndmaNo) {
    if (MI_NDMA_REG(ndmaNo, MI_NDMA_REG_CNT_WOFFSET) & MI_NDMA_IMM_MODE_ON) {
        MI_WaitNDma(ndmaNo);
        return;
    }
    switch (MI_NDMA_REG(ndmaNo, MI_NDMA_REG_CNT_WOFFSET) & MI_NDMA_TIMING_MASK) {
    case MI_NDMA_TIMING_CARD:
        while (reg_MI_MCCNT1 & REG_MI_MCCNT1_START_MASK) {
        }
        break;
    case MI_NDMA_TIMING_DISP:
    case MI_NDMA_TIMING_DISP_MMEM:
        MI_WaitNDma(ndmaNo);
    case MI_NDMA_TIMING_CAMERA:
        MI_StopNDma(ndmaNo);
        break;
    default:
        if (!(MI_NDMA_REG(ndmaNo, MI_NDMA_REG_CNT_WOFFSET) & MI_NDMA_CONTINUOUS_ON)) {
            MI_WaitNDma(ndmaNo);
        } else {
            MI_StopNDma(ndmaNo);
        }
        break;
    }
}
#include <twl/ltdmain_end.h>

void PMi_StopDma(void)
{
    u32 i;

    for (i = 0; i <= MI_DMA_MAX_NUM; i++) {
        vu32 * cnt = (vu32 *)(REG_DMA0CNT_ADDR + i * 12);
        u32 mode;

        PMi_PreDmaCnt[i] = *cnt;
        mode = *cnt & MI_DMA_TIMING_MASK;
        switch (mode) {
        case MI_DMA_TIMING_CARD:
            while (reg_MI_MCCNT1 & REG_MI_MCCNT1_START_MASK) {
            }
            break;
        case MI_DMA_TIMING_DISP:
        case MI_DMA_TIMING_DISP_MMEM:
            MI_WaitDma(i);
            break;
        default:
            if (!(*cnt & MI_DMA_CONTINUOUS_ON)) {
                MI_WaitDma(i);
            } else {
                MI_StopDma(i);
            }
            break;
        }
        if (OS_IsRunOnTwl()) {
            PMi_StopNDma(i);
        }
    }
}

void PM_GoSleepMode(PMWakeUpTrigger trigger, PMLogic logic, u16 keyPattern)
{
    BOOL prepIrq;
    OSIntrMode prepIntrMode;
    OSIrqMask prepIntrMask;
    BOOL powerOffFlag = FALSE;
    PMBackLightSwitch preTop;
    PMBackLightSwitch preBottom;
    u32 preGX;
    u32 preGXS;
    PMLCDPower preLCDPower;
    u32 startTrigger;

    PMi_ExecuteList(PMi_PreSleepCallbackList);

    prepIrq = OS_DisableIrq();
    prepIntrMode = OS_DisableInterrupts();
    prepIntrMask = OS_DisableIrqMask((1 << OS_IRQ_TABLE_MAX) - 1);
    {
        OSIntrMode intr = OS_IE_FIFO_RECV | (OS_IsTickAvailable() ? OS_IE_TIMER0 : 0);
        (void)OS_SetIrqMask(intr);
    }
    (void)OS_RestoreInterrupts(prepIntrMode);
    (void)OS_EnableIrq();

    if (trigger & PM_TRIGGER_CARD) {
        OSBootType bootType = OS_GetBootType();

        if (bootType == OS_BOOTTYPE_DOWNLOAD_MB || bootType == OS_BOOTTYPE_NAND) {
            trigger &= ~PM_TRIGGER_CARD;
        }
    }
    if (trigger & PM_TRIGGER_CARTRIDGE) {
        if (OS_IsRunOnTwl() || !CTRDG_IsExisting()) {
            trigger &= ~PM_TRIGGER_CARTRIDGE;
        }
    }

    preGX = reg_GX_DISPCNT;
    preGXS = reg_GXS_DB_DISPCNT;
    preLCDPower = PM_GetLCDPower();
    while (PM_GetBackLight(&preTop, &preBottom) != PM_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
    }
    while (PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF) != PM_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
    }

    func_0208b87c();
    reg_GX_DISPCNT = reg_GX_DISPCNT & ~REG_GX_DISPCNT_MODE_MASK;
    GXS_DispOff();
    func_0208b87c();
    func_0208b87c();

    PMi_StopDma();

    PMi_SleepEndFlag = FALSE;
    startTrigger = trigger | (preTop != PM_BACKLIGHT_OFF ? PM_BACKLIGHT_RECOVER_TOP_ON : PM_BACKLIGHT_RECOVER_TOP_OFF) | (preBottom != PM_BACKLIGHT_OFF ? PM_BACKLIGHT_RECOVER_BOTTOM_ON : PM_BACKLIGHT_RECOVER_BOTTOM_OFF);
    (void)OS_SetIrqMask(OS_IE_FIFO_RECV);
    (void)func_0208bbb8((u16)startTrigger, (u16)(logic | keyPattern));

    while (!PMi_SleepEndFlag) {
        OS_Halt();
    }

    {
        OSIntrMode intr = OS_IE_FIFO_RECV | (OS_IsTickAvailable() ? OS_IE_TIMER0 : 0);
        (void)OS_SetIrqMask(intr);
    }

    if ((trigger & PM_TRIGGER_CARD) && (OS_GetRequestIrqMask() & OS_IE_CARD_IREQ)) {
        powerOffFlag = TRUE;
    }

    if (!powerOffFlag) {
        if (preLCDPower == PM_LCD_POWER_ON) {
            while (func_0208c44c(PM_LCD_POWER_ON, PM_LED_ON, TRUE, TRUE) != TRUE) {
            }
        } else {
            while (PM_SendUtilityCommand(PM_LED_ON) != PM_RESULT_SUCCESS) {
                OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
            }
        }
        reg_GX_DISPCNT = preGX;
        reg_GXS_DB_DISPCNT = preGXS;
    }

    OS_SpinWaitSysCycles(PMi_LCD_WAIT_SYS_CYCLES);

    (void)OS_DisableIrq();
    (void)OS_SetIrqMask(prepIntrMask);
    (void)OS_RestoreInterrupts(prepIntrMode);
    (void)OS_RestoreIrq(prepIrq);

    if (powerOffFlag) {
        PM_ForceToPowerOff();
    }

    PMi_ExecuteList(PMi_PostSleepCallbackList);
}


BOOL func_0208c44c(PMLCDPower sw, PMLEDStatus led, BOOL skip, BOOL isSync)
{
    switch (sw) {
    case PM_LCD_POWER_ON:
        if (!skip && OS_GetVBlankCount() - PMi_LCDCount <= PMi_WAIT_FRAME) {
            return FALSE;
        }

        if (led != PM_LED_NONE) {
            if (isSync) {
                while (PM_SendUtilityCommand(led) != PM_RESULT_SUCCESS) {
                    OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
                }
            } else {
                while (func_0208bcc4(led, NULL, NULL) != PM_RESULT_SUCCESS) {
                    OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
                }
            }
        }

        reg_GX_POWCNT |= REG_GX_POWCNT_LCD_MASK;
        while (PMi_SetAmp(sAmpSwitch) != PM_RESULT_SUCCESS) {
            OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
        }
        break;
    case PM_LCD_POWER_OFF:
        while (PMi_SetAmp(PM_AMP_OFF) != PM_RESULT_SUCCESS) {
            OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
        }

        if (OS_GetVBlankCount() - PMi_DispOffCount <= 2) {
            func_0208b87c();
            func_0208b87c();
        }
        reg_GX_POWCNT &= ~REG_GX_POWCNT_LCD_MASK;

        PMi_LCDCount = OS_GetVBlankCount();

        if (led != PM_LED_NONE) {
            if (isSync) {
                while (PM_SendUtilityCommand(led) != PM_RESULT_SUCCESS) {
                    OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
                }
            } else {
                while (func_0208bcc4(led, NULL, NULL) != PM_RESULT_SUCCESS) {
                    OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
                }
            }
        }
        break;
    }

    return TRUE;
}






#define PMi_WAIT_FRAME  7

BOOL PM_SetLCDPower (PMLCDPower sw)
{
    if (sw != PM_LCD_POWER_ON) {
        sw = PM_LCD_POWER_OFF;
        if (GX_IsDispOn()) {
            GX_DispOff();
        }
    }

    return func_0208c44c(sw, PM_LED_NONE, FALSE, TRUE);
}

PMLCDPower PM_GetLCDPower (void)
{
    return (reg_GX_POWCNT & REG_GX_POWCNT_LCD_MASK) ? PM_LCD_POWER_ON : PM_LCD_POWER_OFF;
}

u32 func_0208c6a4(u32 arg) {
    return PMi_ReadRegister(0x12, (u16)arg, 0);
}

u32 func_0208c6c0(u32 *out) {
    u16 data;
    u32 result;

    result = PMi_ReadRegister(0xf, 8, &data);
    if (result != 0) {
        return result;
    }
    if (out != NULL) {
        *out = data;
    }
    return result;
}


u32 PMi_InsertList (PMSleepCallbackInfo ** list, PMSleepCallbackInfo * info, s32 priority, u32 mode)
{
    PMSleepCallbackInfo * cur;
    PMSleepCallbackInfo * prev;
    OSIntrMode enabled;

    if (list != NULL) {
        info->priority = priority;
        enabled = OS_DisableInterrupts();
        cur = *list;
        prev = NULL;
        while (cur != NULL) {
            if (mode == 0 && cur->priority > priority) {
                break;
            }
            if (mode == 1 && cur->priority >= priority) {
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        if (cur != NULL) {
            info->next = cur;
        } else {
            info->next = NULL;
        }
        if (prev != NULL) {
            prev->next = info;
        } else {
            *list = info;
        }
        return OS_RestoreInterrupts(enabled);
    }
}

void PMi_DeleteList(PMSleepCallbackInfo **list, PMSleepCallbackInfo *info) {
    PMSleepCallbackInfo *prev;
    PMSleepCallbackInfo *cur;
    OSIntrMode enabled;

    if (list == NULL) {
        return;
    }

    enabled = OS_DisableInterrupts();
    cur = *list;
    prev = cur;
    while (cur != NULL) {
        if (cur == info) {
            if (cur == prev) {
                *list = cur->next;
            } else {
                prev->next = cur->next;
            }
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    OS_RestoreInterrupts(enabled);
}



void PMi_ExecuteList(PMSleepCallbackInfo * list) {
    while (list != NULL) {
        list->callback(list->arg);
        list = list->next;
    }
}

void PM_PrependPreSleepCallback(PMSleepCallbackInfo *info) {
    PMi_InsertList(&PMi_PreSleepCallbackList, info, -255, 1);
}

void PM_AppendPostSleepCallback(PMSleepCallbackInfo *info) {
    PMi_InsertList(&PMi_PostSleepCallbackList, info, 0xff, 0);
}

void PM_InsertPreSleepCallback(PMSleepCallbackInfo *info, int priority) {
    PMi_InsertList(&PMi_PreSleepCallbackList, info, priority, 0);
}






void PM_DeletePreSleepCallback (PMSleepCallbackInfo * info)
{
    PMi_DeleteList(&PMi_PreSleepCallbackList, info);
}

void PM_DeletePostSleepCallback(PMSleepCallbackInfo * info)
{
    PMi_DeleteList(&PMi_PostSleepCallbackList, info);
}

#include <twl/ltdmain_begin.h>

// TwlSDK's PMi_ProceedToExit
void PMi_ProcessExitRequest(PMExitFactor factor) {
    if (!PMi_TryLockExit()) {
        return;
    }
    PMi_ExitFactor = factor;
    PMi_ExecuteList(PMi_PreExitCallbackList);
    if (PMi_AutoExitFlag) {
        func_027314f0();
    }
}

// TwlSDK's PM_ReadyToExit
void func_027314f0(void) {
    PMi_ExecuteExit(TRUE);
}

// TwlSDK's PMi_FinalizeDebugger: tells the ARM7 that the ARM9 is about to stop
static void func_02731500(void) {
    *(vu8 *)(HW_INIT_LOCK_BUF_TWL + 4) = OSi_SYNCVAL_NOT_READY;
    OSi_SyncWithOtherProc(OSi_SYNCTYPE_SENDER, (void *)HW_INIT_LOCK_BUF_TWL);
    OSi_SyncWithOtherProc(OSi_SYNCTYPE_RECVER, (void *)HW_INIT_LOCK_BUF_TWL);
    (void)OS_DisableInterrupts();
    *(vu8 *)(HW_INIT_LOCK_BUF_TWL + 4) = OSi_SYNCVAL_READY;
}

// TwlSDK's PM_GetExitFactor
BOOL PMi_GetExitType(void) {
    return PMi_ExitFactor;
}

// TwlSDK's PM_AppendPreExitCallback
void func_0273154c(PMExitCallbackInfo *info) {
    PMi_InsertList(&PMi_PreExitCallbackList, info, PM_CALLBACK_PRIORITY_MAX, PM_INSERT_APPEND);
}

// TwlSDK's PM_InsertPostExitCallback
void PM_InsertPreExitCallback(PMExitCallbackInfo *info, int priority) {
    PMi_InsertList(&PMi_PostExitCallbackList, info, priority, PM_INSERT_APPEND);
}

// TwlSDK's PM_DeletePostExitCallback
void PM_DeletePreExitCallback(PMExitCallbackInfo *info) {
    PMi_DeleteList(&PMi_PostExitCallbackList, info);
}

void PMi_ExecutePostExitCallbackList(void) {
    PMi_ExecuteList(PMi_PostExitCallbackList);
}

// TwlSDK's PM_SetAutoExit
void func_027315c0(BOOL sw) {
    PMi_AutoExitFlag = sw;
}

// TwlSDK's PM_GetAutoExit
BOOL func_027315d0(void) {
    return PMi_AutoExitFlag;
}

// TwlSDK's PMi_CallPostExitCallbackAndReset: runs the post-exit callbacks, turns the displays off, waits for three
// vertical blanks, has the ARM7 shut the system down or restart it until it answers, and stops the program
static void PMi_ExecuteExit(BOOL isExit) {
    PMi_ExecuteList(PMi_PostExitCallbackList);
    GX_DispOff();
    GXS_DispOff();
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    {
        int n;
        for (n = 0; n < 3; n++) {
            u32 count = OS_GetVBlankCount();
            while (count == OS_GetVBlankCount()) {
                OS_SpinWaitSysCycles(PM_VBLANK_WAIT_CYCLES);
            }
        }
    }

    while (1) {
        u16 result;
        u32 command = isExit ? PM_UTIL_FORCE_EXIT : PM_UTIL_FORCE_RESET_HARDWARE;

        if (*(u32 *)HW_RESET_LOCK_FLAG_BUF == PM_RESET_FLAG_FORCED) {
            command = PM_UTIL_FORCE_EXIT;
            *(u32 *)HW_PARAM_LAUNCH_PARAM = 0;
        }
        while (PMi_ReadRegister(command, 0, &result) != PM_RESULT_SUCCESS) {
            OS_SpinWaitSysCycles(PM_RETRY_CYCLES);
        }
        if (result == PM_RESULT_SUCCESS) {
            break;
        }
        OS_SpinWaitSysCycles(PM_RETRY_CYCLES);
    }

    (void)OS_DisableInterrupts();
    MI_StopAllDma();
    MI_StopAllNDma();
    func_02731500();
    OSi_TerminateCore();
}

#include <twl/ltdmain_end.h>

void func_0208c89c(void)
{
    u32 preMethod;

    OS_SpinWaitSysCycles(PMi_LCD_WAIT_SYS_CYCLES);

    preMethod = PMi_WaitBusyMethod;
    PMi_WaitBusyMethod = PMi_WAITBUSY_METHOD_CPUMODE | PMi_WAITBUSY_METHOD_CPSR | PMi_WAITBUSY_METHOD_IME;
    if (PM_GetLCDPower() != PM_LCD_POWER_ON) {
        while (PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF) != PM_RESULT_SUCCESS) {
            OS_SpinWaitSysCycles(PMi_WAIT_RETRY);
        }

        while (!PM_SetLCDPower(PM_LCD_POWER_ON)) {
            OS_SpinWaitSysCycles(PMi_PXI_WAIT_TICK);
        }
    }
    PMi_WaitBusyMethod = preMethod;
}


void PMi_SetDispOffCount(void)
{
    PMi_DispOffCount = OS_GetVBlankCount();
}

#include <twl/ltdmain_begin.h>

static volatile BOOL isLockedReset = FALSE;

// TwlSDK's PMi_TryLockForReset
static BOOL PMi_TryLockExit(void) {
    OSIntrMode e = OS_DisableInterrupts();

    if (isLockedReset) {
        (void)OS_RestoreInterrupts(e);
        return FALSE;
    }
    isLockedReset = TRUE;
    (void)OS_RestoreInterrupts(e);
    return TRUE;
}

#include <twl/ltdmain_end.h>
