// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/snd/src/snd_command.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/snd/common/command.h>

#include <nitro/misc.h>
#include <nitro/os.h>
#include <nitro/pxi.h>
#include <nitro/mi.h>

#include <nitro/snd/common/seq.h>
#include <nitro/snd/common/capture.h>
#include <nitro/snd/common/work.h>
#include <nitro/snd/common/global.h>
#include <nitro/snd/common/util.h>
#include <nitro/snd/common/alarm.h>
#include <nitro/snd/common/main.h>

#define SND_COMMAND_NUM 256
#define SND_PXI_FIFO_MESSAGE_BUFSIZE   8
#define SND_MSG_REQUEST_COMMAND_PROC   0

#define UNPACK_COMMAND(arg, shift, bit) (((arg) >> (shift)) & ((1 << (bit)) - 1))

    static SNDCommand sCommandArray[SND_COMMAND_NUM] ATTRIBUTE_ALIGN(32);
    static SNDCommand * sFreeList;
    static SNDCommand * sFreeListEnd;

    static SNDCommand * sReserveList;
    static SNDCommand * sReserveListEnd;

    static SNDCommand * sWaitingCommandListQueue[SND_PXI_FIFO_MESSAGE_BUFSIZE + 1];
    static int sWaitingCommandListQueueRead;
    static int sWaitingCommandListQueueWrite;

    static int sWaitingCommandListCount;

    static u32 sCurrentTag;
    static u32 sFinishedTag;

    static SNDSharedWork sSharedWork ATTRIBUTE_ALIGN(32);

static void PxiFifoCallback(PXIFifoTag tag, uPtr data, BOOL err);
static void InitPXI(void);

    static void RequestCommandProc(void);
    static SNDCommand * AllocCommand(void);
    static BOOL IsCommandAvailable(void);

void SND_CommandInit (void)
{
    SNDCommand * command;
    int i;

    InitPXI();

    sFreeList = &sCommandArray[0];
    for (i = 0; i < SND_COMMAND_NUM - 1; i++) {
        sCommandArray[i].next = &sCommandArray[i + 1];
    }
    sCommandArray[SND_COMMAND_NUM - 1].next = NULL;
    sFreeListEnd = &sCommandArray[SND_COMMAND_NUM - 1];

    sReserveList = NULL;
    sReserveListEnd = NULL;

    sWaitingCommandListCount = 0;

    sWaitingCommandListQueueRead = 0;
    sWaitingCommandListQueueWrite = 0;

    sCurrentTag = 1;
    sFinishedTag = 0;

    SNDi_SharedWork = &sSharedWork;
    SNDi_InitSharedWork(SNDi_SharedWork);

    command = SND_AllocCommand(SND_COMMAND_BLOCK);
    if (command != NULL) {
        command->id = SND_COMMAND_SHARED_WORK;
        command->arg[0] = (u32)SNDi_SharedWork;

        SND_PushCommand(command);
        (void)SND_FlushCommand(SND_COMMAND_BLOCK);
    }

}

const SNDCommand * SND_RecvCommandReply (u32 flags)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    SNDCommand * commandList;
    SNDCommand * commandListEnd;

    if (flags & SND_COMMAND_BLOCK) {
        while (sFinishedTag == SNDi_GetFinishedCommandTag()) {
            (void)OS_RestoreInterrupts(bak_psr);
            OS_SpinWaitSysCycles(50);
            bak_psr = OS_DisableInterrupts();
        }
    } else {
        if (sFinishedTag == SNDi_GetFinishedCommandTag()) {
            (void)OS_RestoreInterrupts(bak_psr);
            return NULL;
        }
    }

    commandList = sWaitingCommandListQueue[sWaitingCommandListQueueRead];
    sWaitingCommandListQueueRead++;
    if (sWaitingCommandListQueueRead > SND_PXI_FIFO_MESSAGE_BUFSIZE)
        sWaitingCommandListQueueRead = 0;

    commandListEnd = commandList;
    while (commandListEnd->next != NULL) {
        commandListEnd = commandListEnd->next;
    }

    if (sFreeListEnd != NULL) {
        sFreeListEnd->next = commandList;
    } else {
        sFreeList = commandList;
    }

    sFreeListEnd = commandListEnd;

    sWaitingCommandListCount--;
    sFinishedTag++;

    (void)OS_RestoreInterrupts(bak_psr);
    return commandList;
}

SNDCommand * SND_AllocCommand (u32 flags)
{
    SNDCommand * command;

    if (!IsCommandAvailable())
        return NULL;

    command = AllocCommand();
    if (command != NULL)
        return command;

    if ((flags & SND_COMMAND_BLOCK) == 0)
        return NULL;

    if (SND_CountWaitingCommand() > 0) {
        while (SND_RecvCommandReply(SND_COMMAND_NOBLOCK) != NULL) {
        }

        command = AllocCommand();
        if (command != NULL)
            return command;
    } else {
        (void)SND_FlushCommand(SND_COMMAND_BLOCK);
    }

    RequestCommandProc();

    do {
        (void)SND_RecvCommandReply(SND_COMMAND_BLOCK);
        command = AllocCommand();
    } while (command == NULL);

    return command;
}

void SND_PushCommand (struct SNDCommand * command)
{
    OSIntrMode bak_psr;

    SDK_NULL_ASSERT(command);

    bak_psr = OS_DisableInterrupts();

    if (sReserveListEnd == NULL) {
        sReserveList = command;
        sReserveListEnd = command;
    } else {
        sReserveListEnd->next = command;
        sReserveListEnd = command;
    }

    command->next = NULL;

    (void)OS_RestoreInterrupts(bak_psr);
}

BOOL SND_FlushCommand (u32 flags)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();

    if (!sReserveList) {
        (void)OS_RestoreInterrupts(bak_psr);
        return TRUE;
    }

    if (sWaitingCommandListCount >= SND_PXI_FIFO_MESSAGE_BUFSIZE) {
        if ((flags & SND_COMMAND_BLOCK) == 0) {
            (void)OS_RestoreInterrupts(bak_psr);
            return FALSE;
        }

        do {
            (void)SND_RecvCommandReply(SND_COMMAND_BLOCK);
        } while (sWaitingCommandListCount >= SND_PXI_FIFO_MESSAGE_BUFSIZE);

        if (sReserveList == NULL) {
            (void)OS_RestoreInterrupts(bak_psr);
            return TRUE;
        }
    }

    DC_FlushRange(sCommandArray, sizeof(sCommandArray));
    if (PXI_SendWordByFifo(PXI_FIFO_TAG_SOUND, (u32)sReserveList, FALSE) < 0) {
        if ((flags & SND_COMMAND_BLOCK) == 0) {
            (void)OS_RestoreInterrupts(bak_psr);
            return FALSE;
        }

        while (sWaitingCommandListCount >= SND_PXI_FIFO_MESSAGE_BUFSIZE ||
               PXI_SendWordByFifo(PXI_FIFO_TAG_SOUND, (u32)sReserveList, FALSE) < 0) {

            (void)OS_RestoreInterrupts(bak_psr);
            (void)SND_RecvCommandReply(SND_COMMAND_NOBLOCK);
            bak_psr = OS_DisableInterrupts();

            DC_FlushRange(sCommandArray, sizeof(sCommandArray));

            if (!sReserveList) {

                (void)OS_RestoreInterrupts(bak_psr);
                return TRUE;
            }
        }
    }

    sWaitingCommandListQueue[sWaitingCommandListQueueWrite] = sReserveList;
    sWaitingCommandListQueueWrite++;
    if (sWaitingCommandListQueueWrite > SND_PXI_FIFO_MESSAGE_BUFSIZE)
        sWaitingCommandListQueueWrite = 0;

    sReserveList = NULL;
    sReserveListEnd = NULL;

    sWaitingCommandListCount++;
    sCurrentTag++;

    (void)OS_RestoreInterrupts(bak_psr);

    if (flags & SND_COMMAND_IMMEDIATE) {
        RequestCommandProc();
    }

    return TRUE;
}

void SND_WaitForCommandProc (u32 tag)
{
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }

    while (SND_RecvCommandReply(SND_COMMAND_NOBLOCK) != NULL) {
    }

    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }

    RequestCommandProc();

    while (!SND_IsFinishedCommandTag(tag)) {
        (void)SND_RecvCommandReply(SND_COMMAND_BLOCK);
    }
}

u32 SND_GetCurrentCommandTag (void)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    u32 tag;

    if (sReserveList == NULL) {
        tag = sFinishedTag;
    } else {
        tag = sCurrentTag;
    }

    (void)OS_RestoreInterrupts(bak_psr);
    return tag;
}

BOOL SND_IsFinishedCommandTag (u32 tag)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    BOOL result;

    if (tag > sFinishedTag) {
        if (tag - sFinishedTag < 0x80000000) {
            result = FALSE;
        } else {
            result = TRUE;
        }
    } else {
        if (sFinishedTag - tag < 0x80000000) {
            result = TRUE;
        } else {
            result = FALSE;
        }
    }

    (void)OS_RestoreInterrupts(bak_psr);
    return result;
}

int SND_CountFreeCommand (void)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    SNDCommand * command;
    int count = 0;

    command = sFreeList;
    while (command != NULL) {
        ++count;
        command = command->next;
    }

    (void)OS_RestoreInterrupts(bak_psr);
    return count;
}

int SND_CountReservedCommand (void)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    SNDCommand * command;
    int count = 0;

    command = sReserveList;
    while (command != NULL) {
        ++count;
        command = command->next;
    }

    (void)OS_RestoreInterrupts(bak_psr);
    return count;
}

int SND_CountWaitingCommand (void)
{
    int freeCount = SND_CountFreeCommand();
    int reservedCount = SND_CountReservedCommand();

    return SND_COMMAND_NUM - freeCount - reservedCount;
}

static void PxiFifoCallback (PXIFifoTag tag, u32 data, BOOL)
{
    OSIntrMode enabled;

#pragma unused(tag)
    enabled = OS_DisableInterrupts();
#pragma unused(result)
    SNDi_CallAlarmHandler((int)data);
    (void)OS_RestoreInterrupts(enabled);
}

static void InitPXI (void)
{
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_SOUND, PxiFifoCallback);

    if (IsCommandAvailable()) {
        while (!PXI_IsCallbackReady(PXI_FIFO_TAG_SOUND, PXI_PROC_ARM7)) {
            OS_SpinWaitSysCycles(50);
        }
    }
}

    static void RequestCommandProc (void)
    {
        while (PXI_SendWordByFifo(PXI_FIFO_TAG_SOUND, SND_MSG_REQUEST_COMMAND_PROC, FALSE) < 0) {
        }
    }

    static SNDCommand * AllocCommand (void)
    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        SNDCommand * command;

        if (sFreeList == NULL) {
            (void)OS_RestoreInterrupts(bak_psr);
            return NULL;
        }

        command = sFreeList;
        sFreeList = sFreeList->next;

        if (sFreeList == NULL)
            sFreeListEnd = NULL;

        (void)OS_RestoreInterrupts(bak_psr);
        return command;
    }

    static BOOL IsCommandAvailable (void)
    {
        OSIntrMode prev_mode;
        u32 res;

        if (!OS_IsRunOnEmulator())
            return TRUE;

        prev_mode = OS_DisableInterrupts();

        *(vu32 *)0x4fff200 = 0x10;
        res = *(vu32 *)0x4fff200;

        (void)OS_RestoreInterrupts(prev_mode);

        return res ? TRUE : FALSE;
    }
