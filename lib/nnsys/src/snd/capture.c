// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nnsys/src/snd/snd_capture.c.
#include "nitro/mi.h"
#include "nitro/os.h"
#include "snd_internal.h"

// NitroSystem's capture (NNSi_SndCapture), which records the mixer's output into a buffer and plays it back on channels
// of its own, for reverb and effects. Only what the sound library's main loop and sleep callbacks call is left in the
// game; the functions that start a capture were stripped. The SDK's file is capture.c, and the names are NitroSystem's,
// except sCaptureReserved and sCaptureThreadFlag, which are ours

#define THREAD_MESSAGE_BUF_SIZE 8

static void NNSi_SndCaptureStop(void);

// MWCC lays these out in reverse order of declaration: the flag first and the capture last. sCaptureReserved and
// sMesgBuf were used only by the functions that start a capture, which the linker dropped
static CaptureParam sCapParam;
static OSMessage sMesgBuf[THREAD_MESSAGE_BUF_SIZE];
static OSMessageQueue sMesgQ;
static u32 sCaptureReserved;
static BOOL sCaptureThreadFlag;

void NNSi_SndCaptureInit(void) {
    sCaptureThreadFlag = FALSE;
    sCapParam.active = FALSE;
}

void NNSi_SndCaptureMain(void) {
    CaptureParam *info = &sCapParam;
    NNSSndFader *fader;
    int volume;

    if (!info->active) {
        return;
    }
    if (info->type != NNS_SND_CAPTURE_TYPE_REVERB) {
        return;
    }

    fader = &info->fader;
    NNSi_SndFaderUpdate(fader);
    if (info->fadeStopFlag && NNSi_SndFaderIsFinished(fader)) {
        NNSi_SndCaptureStop();
        return;
    }

    volume = NNSi_SndFaderGet(fader) >> 8;
    if (volume != info->volume) {
        SND_SetChannelVolume(info->chBitMask, volume, SND_CHANNEL_DATASHIFT_NONE);
        info->volume = volume;
    }
}

static void NNSi_SndCaptureStop(void) {
    CaptureParam *info = &sCapParam;
    BOOL alarmFlag;
    u32 tag;

    if (!info->active) {
        return;
    }

    alarmFlag = info->alarmNo >= 0 ? TRUE : FALSE;
    SND_StopTimer(info->chBitMask, info->capBitMask, alarmFlag ? 1 << info->alarmNo : 0, 0);
    if (alarmFlag) {
        tag = SND_GetCurrentCommandTag();
        SND_FlushCommand(SND_COMMAND_BLOCK);
        SND_WaitForCommandProc(tag);
        while (OS_ReceiveMessage(&sMesgQ, NULL, OS_MESSAGE_NOBLOCK)) {
        }
    }

    if (info->capBitMask) {
        NNS_SndUnlockCapture(info->capBitMask);
    }
    if (info->lockChBitFlag) {
        NNS_SndUnlockChannel(info->lockChBitFlag);
    }
    if (alarmFlag) {
        NNS_SndFreeAlarm(info->alarmNo);
    }
    if (info->type == NNS_SND_CAPTURE_TYPE_EFFECT) {
        SND_SetOutputSelector(0, 0, 0, 0);
    }
    info->active = FALSE;
}

void NNSi_SndCaptureBeginSleep(void) {
    CaptureParam *info = &sCapParam;
    u32 tag;

    if (!info->active) {
        return;
    }
    SND_StopTimer(info->chBitMask, info->capBitMask, info->alarmNo >= 0 ? 1 << info->alarmNo : 0, 0);

    tag = SND_GetCurrentCommandTag();
    SND_FlushCommand(SND_COMMAND_BLOCK);
    SND_WaitForCommandProc(tag);
}

void NNSi_SndCaptureEndSleep(void) {
    CaptureParam *info = &sCapParam;

    if (!info->active) {
        return;
    }
    info->bufPos = 0;
    MI_CpuClear32(info->bufL, info->bufSize);
    MI_CpuClear32(info->bufR, info->bufSize);
    DC_FlushRange(info->bufL, info->bufSize);
    DC_FlushRange(info->bufR, info->bufSize);

    SND_StartTimer(info->chBitMask, info->capBitMask, info->alarmNo >= 0 ? 1 << info->alarmNo : 0, 0);
}

#ifndef __MWERKS__
// The Gen 4 games' capture (NitroSystem 071126), which Black never calls: the functions that start and stop a capture,
// written from what they do, not in the DS build, whose capture.c is complete without them. Capture 0 records the
// mixer's left output and plays it back on channel 1, capture 1 the right on channel 3, from a buffer each, in a loop
// (the hardware's pairs). A reverb leaves the playback in the mixer, so its echo is captured again and fades with the
// channels' volume; an effect takes the channels out of the mixer and out to the speakers, and a thread runs the
// callback on each block as it is captured, before the playback comes round to it a buffer later

#define CAPTURE_THREAD_STACK_SIZE 0x1000
#define CAPTURE_THREAD_PRIORITY 4

#define CAPTURE_CH_BIT_MASK ((1 << SND_CAPTURE_0_OUT_CHANNEL) | (1 << SND_CAPTURE_1_OUT_CHANNEL))
#define CAPTURE_BIT_MASK ((1 << SND_CAPTURE_0) | (1 << SND_CAPTURE_1))

static OSThread sCaptureThread;
static u64 sCaptureThreadStack[CAPTURE_THREAD_STACK_SIZE / sizeof(u64)];

static void CaptureThread(void *arg);
static void CaptureAlarmCallback(void *arg);
static BOOL LockCapture(CaptureParam *info, BOOL needAlarm);
static void SetupCapture(CaptureParam *info, void *buffer, u32 bufLen, NNSSndCaptureFormat format, int sampleRate,
                         int volume);

BOOL NNS_SndCaptureStartReverb(void *buffer, u32 bufSize, NNSSndCaptureFormat format, int sampleRate, int volume) {
    CaptureParam *info = &sCapParam;

    if (!LockCapture(info, FALSE)) {
        return FALSE;
    }
    info->type = NNS_SND_CAPTURE_TYPE_REVERB;
    info->callback = NULL;
    info->arg = NULL;
    info->interval = 0;
    SetupCapture(info, buffer, bufSize / 2, format, sampleRate, volume);

    NNSi_SndFaderInit(&info->fader);
    NNSi_SndFaderSet(&info->fader, volume << 8, 0);
    info->fadeStopFlag = FALSE;
    info->volume = volume;
    info->active = TRUE;

    SND_StartTimer(info->chBitMask, info->capBitMask, 0, 0);
    return TRUE;
}

void NNS_SndCaptureStopReverb(int frames) {
    CaptureParam *info = &sCapParam;

    if (!info->active || info->type != NNS_SND_CAPTURE_TYPE_REVERB) {
        return;
    }
    if (frames == 0) {
        NNSi_SndCaptureStop();
        return;
    }
    NNSi_SndFaderSet(&info->fader, 0, frames);
    info->fadeStopFlag = TRUE;
}

void NNS_SndCaptureSetReverbVolume(int volume, int frames) {
    CaptureParam *info = &sCapParam;

    if (!info->active || info->type != NNS_SND_CAPTURE_TYPE_REVERB) {
        return;
    }
    NNSi_SndFaderSet(&info->fader, volume << 8, frames);
    info->fadeStopFlag = FALSE;
}

// The buffer is split in two for the channels, and each half in interval blocks, a whole number of 32-byte units
// each. The alarm goes off once per block
BOOL NNS_SndCaptureStartEffect(void *buffer, u32 bufSize, NNSSndCaptureFormat format, int sampleRate, int interval,
                               NNSSndCaptureEffectCallback callback, void *arg) {
    CaptureParam *info = &sCapParam;
    u32 bufLen = bufSize / (interval * 32 * 2) * interval * 32;
    u32 samples = format == NNS_SND_CAPTURE_FORMAT_PCM16 ? bufLen / 2 : bufLen;
    u32 alarmPeriod = (u64)(SND_TIMER_CLOCK / sampleRate) * samples / 32 / interval;

    if (bufLen == 0 || !LockCapture(info, TRUE)) {
        return FALSE;
    }
    info->type = NNS_SND_CAPTURE_TYPE_EFFECT;
    info->callback = callback;
    info->arg = arg;
    info->interval = interval;
    SetupCapture(info, buffer, bufLen, format, sampleRate, SND_CHANNEL_VOLUME_MAX);
    info->volume = SND_CHANNEL_VOLUME_MAX;

    if (!sCaptureThreadFlag) {
        OS_InitMessageQueue(&sMesgQ, sMesgBuf, THREAD_MESSAGE_BUF_SIZE);
        OS_CreateThread(&sCaptureThread, CaptureThread, NULL, sCaptureThreadStack + CAPTURE_THREAD_STACK_SIZE / sizeof(u64),
                        CAPTURE_THREAD_STACK_SIZE, CAPTURE_THREAD_PRIORITY);
        OS_WakeupThreadDirect(&sCaptureThread);
        sCaptureThreadFlag = TRUE;
    }

    SND_SetupAlarm(info->alarmNo, alarmPeriod, alarmPeriod, CaptureAlarmCallback, NULL);
    SND_SetOutputSelector(SND_OUTPUT_CHANNEL1, SND_OUTPUT_CHANNEL3, SND_CHANNEL_OUT_BYPASS, SND_CHANNEL_OUT_BYPASS);
    info->active = TRUE;

    SND_StartTimer(info->chBitMask, info->capBitMask, 1 << info->alarmNo, 0);
    return TRUE;
}

void NNS_SndCaptureStopEffect(void) {
    CaptureParam *info = &sCapParam;

    if (!info->active || info->type != NNS_SND_CAPTURE_TYPE_EFFECT) {
        return;
    }
    NNSi_SndCaptureStop();
}

BOOL NNS_SndCaptureIsActive(void) {
    return sCapParam.active;
}

// Takes the channels, the captures and, for an effect, an alarm
static BOOL LockCapture(CaptureParam *info, BOOL needAlarm) {
    if (info->active) {
        return FALSE;
    }
    if (!NNS_SndLockChannel(CAPTURE_CH_BIT_MASK)) {
        return FALSE;
    }
    if (!NNS_SndLockCapture(CAPTURE_BIT_MASK)) {
        NNS_SndUnlockChannel(CAPTURE_CH_BIT_MASK);
        return FALSE;
    }
    info->alarmNo = -1;
    if (needAlarm) {
        info->alarmNo = NNS_SndAllocAlarm();
        if (info->alarmNo < 0) {
            NNS_SndUnlockCapture(CAPTURE_BIT_MASK);
            NNS_SndUnlockChannel(CAPTURE_CH_BIT_MASK);
            return FALSE;
        }
    }
    info->lockChBitFlag = CAPTURE_CH_BIT_MASK;
    info->chBitMask = CAPTURE_CH_BIT_MASK;
    info->capBitMask = CAPTURE_BIT_MASK;
    return TRUE;
}

// Clears the two buffers of bufLen bytes from buffer and sets the captures and their channels up on them
static void SetupCapture(CaptureParam *info, void *buffer, u32 bufLen, NNSSndCaptureFormat format, int sampleRate,
                         int volume) {
    SNDWaveFormat waveFormat = format == NNS_SND_CAPTURE_FORMAT_PCM8 ? SND_WAVE_FORMAT_PCM8 : SND_WAVE_FORMAT_PCM16;
    SNDCaptureFormat capFormat = format == NNS_SND_CAPTURE_FORMAT_PCM8 ? SND_CAPTURE_FORMAT_PCM8 : SND_CAPTURE_FORMAT_PCM16;
    int timer = SND_TIMER_CLOCK / sampleRate;

    info->format = format;
    info->sampleRate = sampleRate;
    info->bufL = buffer;
    info->bufR = (u8 *)buffer + bufLen;
    info->bufSize = bufLen;
    info->bufPos = 0;
    MI_CpuClear32(info->bufL, bufLen);
    MI_CpuClear32(info->bufR, bufLen);
    DC_FlushRange(info->bufL, bufLen);
    DC_FlushRange(info->bufR, bufLen);

    SND_SetupChannelPcm(SND_CAPTURE_0_OUT_CHANNEL, waveFormat, info->bufL, SND_CHANNEL_LOOP_REPEAT, 0, bufLen / sizeof(u32),
                        volume, SND_CHANNEL_DATASHIFT_NONE, timer, SND_CHANNEL_PAN_MIN);
    SND_SetupChannelPcm(SND_CAPTURE_1_OUT_CHANNEL, waveFormat, info->bufR, SND_CHANNEL_LOOP_REPEAT, 0, bufLen / sizeof(u32),
                        volume, SND_CHANNEL_DATASHIFT_NONE, timer, SND_CHANNEL_PAN_MAX);
    SND_SetupCapture(SND_CAPTURE_0, capFormat, info->bufL, bufLen / sizeof(u32), TRUE, SND_CAPTURE_IN_MIXER_L,
                     SND_CAPTURE_OUT_NORMAL);
    SND_SetupCapture(SND_CAPTURE_1, capFormat, info->bufR, bufLen / sizeof(u32), TRUE, SND_CAPTURE_IN_MIXER_R,
                     SND_CAPTURE_OUT_NORMAL);
}

static void CaptureAlarmCallback(void *arg) {
    OS_SendMessage(&sMesgQ, NULL, OS_MESSAGE_NOBLOCK);
}

// Runs the effect's callback on the block the capture has just filled; the playback, a buffer behind, gets there later
static void CaptureThread(void *arg) {
    CaptureParam *info = &sCapParam;
    u32 len;

    while (TRUE) {
        OS_ReceiveMessage(&sMesgQ, NULL, OS_MESSAGE_BLOCK);
        if (!info->active || info->type != NNS_SND_CAPTURE_TYPE_EFFECT) {
            continue;
        }

        len = info->bufSize / info->interval;
        DC_InvalidateRange((u8 *)info->bufL + info->bufPos, len);
        DC_InvalidateRange((u8 *)info->bufR + info->bufPos, len);
        info->callback((u8 *)info->bufL + info->bufPos, (u8 *)info->bufR + info->bufPos, len, info->format, info->arg);

        info->bufPos += len;
        if (info->bufPos >= info->bufSize) {
            info->bufPos = 0;
        }
    }
}

// The kind of the capture that runs (or ran last): HeartGold's (declared there implicitly)
NNSSndCaptureType NNS_SndCaptureGetCaptureType(void);

NNSSndCaptureType NNS_SndCaptureGetCaptureType(void) {
    return sCapParam.type;
}
#endif
