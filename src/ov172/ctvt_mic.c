// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/ctvt_mic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/ctvt_mic.h"
#include "types.h"
#include "app/comm_tvt/ima_adpcm.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/spi/ARM9/mic.h"
#include "nitro/os.h"
#include "nnsys/snd.h"
#include "twl/snd/ARM9/sndex.h"


// The Xtransceiver's microphone: the voice is recorded into a buffer, sent packed by ctvt_talk.c, and the other
// side's voice played back on a wave channel

// The recording buffer, a little over three seconds at 8180 Hz
#define CTVT_MIC_BUFFER_SIZE 0xc800
#define CTVT_MIC_RATE 8180
#define CTVT_MIC_WAVE_CHANNEL 7
// The microphone is only used once this many frames have passed
#define CTVT_MIC_WARMUP_FRAMES 180

struct CtvtMic {
    u32 recordedSize;
    void *buffer;
    BOOL recording;
    BOOL playing;
    u8 frames;
    u32 playFrames;
    u32 playSize;
    int playSpeed;
    NNSSndWaveOutHandle channel;
};

static void func_ov172_021ba098(void *data);
static void func_ov172_021ba114(MICResult result, void *arg);
void func_ov172_021ba1b4(CtvtMic *work, HeapID heapId);
static void func_ov172_021ba1d8(CtvtMic *work);

static u16 data_ov172_021c720c[6] = { 0x7e46, 0x81ba, 0x7e46, 0x7e43, 0x836e, 0 };

// pokebw2's sCtvtMic
static CtvtMic *data_ov172_021c88ec;

// pokebw2's CtvtMic_Create
CtvtMic *func_ov172_021b9f9c(HeapID heapId) {
    CtvtMic *work = GFL_HeapAllocate(heapId, sizeof(CtvtMic), TRUE, "ctvt_mic.c", 87);

    MIC_Init();
    PM_Init();
    PM_SetAmp(PM_AMP_ON);
    PM_SetAmpGainLevel(80);
    if (OS_IsRunOnTwl() == TRUE) {
        func_027047c0(0, data_ov172_021c720c);
    }
    func_ov172_021ba1b4(work, heapId);
    work->buffer = allocConfigDSSoftwareFeature(heapId, CTVT_MIC_BUFFER_SIZE, "ctvt_mic.c", 134);
    work->recordedSize = 0;
    work->recording = FALSE;
    work->frames = 0;
    data_ov172_021c88ec = work;
    GFL_VBlankSetCallback(func_ov172_021ba098, NULL);
    return work;
}


// pokebw2's CtvtMic_Delete
void func_ov172_021ba01c(CtvtMic *work) {
    GFL_VBlankResetCallback();
    data_ov172_021c88ec = NULL;
    func_ov172_021ba120(work);
    func_ov172_021ba1d8(work);
    PM_SetAmp(PM_AMP_OFF);
    func_0203d7fc(work->buffer);
    GFL_HeapFree(work);
}

// pokebw2's CtvtMic_Update
void func_ov172_021ba050(CtvtMic *work) {
    if (work->recording == TRUE) {
        void *last = MIC_GetLastSamplingAddress();

        if (last == NULL) {
            work->recordedSize = 0;
        } else {
            work->recordedSize = (u32)((u8 *)last - (u8 *)work->buffer) + 4;
            if (work->recordedSize > CTVT_MIC_BUFFER_SIZE) {
                work->recordedSize = CTVT_MIC_BUFFER_SIZE;
            }
        }
    }
    if (work->playing == TRUE && !NNS_SndWaveOutIsPlaying(work->channel)) {
        work->playing = FALSE;
    }
    func_ov172_021ba1e4(work);
}

// pokebw2's CtvtMic_Draw
void func_ov172_021ba094(CtvtMic *work) {
}

// pokebw2's CtvtMic_VBlank
static void func_ov172_021ba098(void *data) {
    if (data_ov172_021c88ec->playing == TRUE) {
        data_ov172_021c88ec->playFrames++;
    }
    if (data_ov172_021c88ec->frames < CTVT_MIC_WARMUP_FRAMES) {
        data_ov172_021c88ec->frames++;
    }
}

// pokebw2's CtvtMic_StartRecording
BOOL func_ov172_021ba0bc(CtvtMic *work) {
    MICAutoParam param;

    param.type = MIC_SAMPLING_TYPE_SIGNED_12BIT;
    param.size = CTVT_MIC_BUFFER_SIZE;
    param.buffer = work->buffer;
    MIi_CpuClear32(0, param.buffer, param.size);
    if (param.size & 0x1f) {
        param.size &= ~0x1f;
    }
    param.full_callback = func_ov172_021ba114;
    param.loop_enable = FALSE;
    param.rate = MIC_SAMPLING_RATE_8180;
    param.full_arg = work;
    if (MIC_StartLimitedSampling(&param) == MIC_RESULT_SUCCESS) {
        work->recording = TRUE;
        work->recordedSize = 0;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtMic_OnBufferFull
static void func_ov172_021ba114(MICResult result, void *arg) {
    CtvtMic *work = arg;

    work->recording = FALSE;
    work->recordedSize = CTVT_MIC_BUFFER_SIZE;
}

// pokebw2's CtvtMic_StopRecording
BOOL func_ov172_021ba120(CtvtMic *work) {
    if (work->recording == FALSE) {
        return TRUE;
    }
    if (MIC_StopLimitedSampling() == MIC_RESULT_SUCCESS) {
        work->recordedSize = (u32)((u8 *)MIC_GetLastSamplingAddress() - (u8 *)work->buffer) + 4;
        if (work->recordedSize > CTVT_MIC_BUFFER_SIZE) {
            work->recordedSize = CTVT_MIC_BUFFER_SIZE;
        }
        work->recording = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtMic_IsRecording
BOOL func_ov172_021ba158(CtvtMic *work) {
    return work->recording;
}

// pokebw2's CtvtMic_GetRecordedSize
u32 func_ov172_021ba15c(CtvtMic *work) {
    if (work->recordedSize > CTVT_MIC_SKIPPED_SIZE) {
        return work->recordedSize - CTVT_MIC_SKIPPED_SIZE;
    }
    return 0;
}

// pokebw2's CtvtMic_GetBuffer
void *func_ov172_021ba170(CtvtMic *work) {
    return work->buffer;
}

// pokebw2's CtvtMic_Encode
u32 func_ov172_021ba174(CtvtMic *work, const s16 *src, u8 *dst, u32 size) {
    MI_CpuFill8(dst, 0, size);
    return func_ov172_021b6a00(src, size, dst);
}

// pokebw2's CtvtMic_Decode
u32 func_ov172_021ba194(CtvtMic *work, const s8 *src, s16 *dst, u32 size) {
    MIi_CpuClear32(0, dst, size * 4);
    return func_ov172_021b6a18(src, size, dst);
}

void func_ov172_021ba1b4(CtvtMic *work, HeapID heapId) {
    work->channel = NNS_SndWaveOutAllocChannel(CTVT_MIC_WAVE_CHANNEL);
    GFL_ASSERT(work->channel != NULL);
    work->playing = FALSE;
}


// pokebw2's CtvtMic_FreeChannel
static void func_ov172_021ba1d8(CtvtMic *work) {
    NNS_SndWaveOutFreeChannel(work->channel);
}

// pokebw2's CtvtMic_Debug
void func_ov172_021ba1e4(CtvtMic *work) {
}

// pokebw2's CtvtMic_Play
BOOL func_ov172_021ba1e8(CtvtMic *work, const void *data, u32 size, int volume, int speed) {
    if (size > 32) {
        work->playing = NNS_SndWaveOutStart(work->channel, NNS_SND_WAVE_FORMAT_PCM16, (const u8 *)data + CTVT_MIC_SKIPPED_SIZE, FALSE,
                                            0, size / 2, CTVT_MIC_RATE, volume, speed, 64);
        work->playFrames = 0;
        work->playSize = size;
        work->playSpeed = speed;
        return work->playing;
    }
    return FALSE;
}

// pokebw2's CtvtMic_StopPlaying
void func_ov172_021ba234(CtvtMic *work) {
    work->playing = FALSE;
    NNS_SndWaveOutStop(work->channel);
}

// pokebw2's CtvtMic_IsPlaying
BOOL func_ov172_021ba244(CtvtMic *work) {
    return work->playing;
}

// pokebw2's CtvtMic_GetPlaySize
u32 func_ov172_021ba248(CtvtMic *work) {
    return work->playSize;
}

// pokebw2's CtvtMic_GetPlayFrames
u16 func_ov172_021ba24c(CtvtMic *work) {
    return work->playFrames;
}

// The length of the playback in frames: the samples at 8180 Hz, at the playback's speed
// pokebw2's CtvtMic_GetPlayLength
u16 func_ov172_021ba254(CtvtMic *work) {
    return (work->playSize << 15) / (270 * work->playSpeed);
}

// pokebw2's CtvtMic_IsReady
BOOL func_ov172_021ba270(CtvtMic *work) {
    if (work->frames >= CTVT_MIC_WARMUP_FRAMES) {
        return TRUE;
    }
    return FALSE;
}
