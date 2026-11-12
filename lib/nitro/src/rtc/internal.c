// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/rtc/src/internal.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/rtc/ARM9/api.h>
#include <nitro/pxi.h>

static BOOL RtcSendPxiCommand(u32 command);

BOOL RTCi_ReadRawDateTimeAsync (void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_DATETIME);
}

BOOL RTCi_ReadRawDateAsync (void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_DATE);
}

BOOL RTCi_ReadRawTimeAsync (void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_TIME);
}

BOOL RTCi_WriteRawStatus2Async (void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_STATUS2);
}

static BOOL RtcSendPxiCommand (u32 command)
{
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_RTC, ((command << RTC_PXI_COMMAND_SHIFT) & RTC_PXI_COMMAND_MASK), 0)) {
        return FALSE;
    }

    return TRUE;
}
