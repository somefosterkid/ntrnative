// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov029/dwc_rap_vchat.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// dwc_rap_vchat.c: the game's side of the Wi-Fi voice chat, between the connection (dwc_rap.c in overlay 11), the
// microphone and the speaker and Nintendo's voice chat library libVCT, which follows it in overlay 29. The ROM names
// no file, so the name is a guess. The names are ours

#include "gfl/dwc_vchat.h"
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/vct.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/spi/ARM9/mic.h"
#include "nitro/os.h"
#include "nnsys/snd.h"
#include "system/dsi.h"

#define VCT_SAMPLE_BYTES 0x880

typedef struct DWCRapVChatWork {
    // What the microphone samples into, a ring that the stream callback reads
    u8 micRing[VCT_SAMPLE_BYTES];
    // Memory for the library's packets
    void *packetMemory;
    // The speaker's stream buffer
    u8 streamBuffer[VCT_SAMPLE_BYTES];
    // What is sent in place of the microphone's samples when it is muted
    u8 silence[VCT_SAMPLE_BYTES];
    void (*onEnd)(void);
    VCTSession session;
    // Whether the machine of each net ID is being heard
    int heard[2];
    int callbackKind;
    // 0 before a request, 1 while it waits, 2 once the voice flows
    int phase;
    u16 stalled;
    u16 muted;
    u8 unk19b4[4];
    u8 stream[0x64];
    VCTSession *requestSession;
    MICAutoParam micParam;
    u64 lastTick;
    int tickBudget;
    u16 waitFrames;
    u8 micState;
    u8 micRunning;
} DWCRapVChatWork;

BOOL func_ov013_021b370c(VCTConfig *config);
void func_ov013_021aeb20(void);
void func_ov013_021aeb78(MICResult result, void *arg);
void func_ov013_021aeb7c(NNSSndStrmCallbackStatus status, int numChannels, void *buffer[], u32 len,
                           NNSSndStrmFormat format, void *arg);
BOOL func_ov013_021aec74(u8 aid);
BOOL func_ov013_021aecc4(u8 aid);
void func_ov013_021aed08(u8 aid, int event, VCTSession *session, void *userData);
void func_ov013_021aed30(u8 aid, int event, VCTSession *session, void *userData);
void func_ov013_021aedec(VCTSession *session);
void func_ov013_021af124(void);

// pokebw2's sVChatWork
static DWCRapVChatWork *data_ov013_021b3c80;

// pokebw2's InitHardware
void func_ov013_021aeb20(void) {
    MIC_Init();
    PM_Init();
    PM_SetAmp(PM_AMP_ON);
    PM_SetAmpGain(PM_AMPGAIN_160);
    func_ov013_021b0cd8(TRUE);
    NNS_SndInit();
    NNS_SndStrmInit((NNSSndStrm *)data_ov013_021b3c80->stream);
    MIi_CpuClearFast(0, data_ov013_021b3c80->silence, VCT_SAMPLE_BYTES);
    data_ov013_021b3c80->tickBudget = 0;
    func_ov013_021b0d08(TRUE);
}

// pokebw2's MicCallback
void func_ov013_021aeb78(MICResult result, void *arg) {
}

void func_ov013_021aeb7c(NNSSndStrmCallbackStatus status, int numChannels, void *buffer[], u32 len,
                           NNSSndStrmFormat format, void *arg) {
    u8 *samples = arg;
    u32 i;

    if (status == NNS_SND_STRM_CALLBACK_SETUP) {
        for (i = 0; i < numChannels; i++) {
            MI_CpuFill8(buffer[i], 0, len);
        }
        return;
    }
    if (!func_0202eb0c()) {
        if (data_ov013_021b3c80->micRunning) {
            MIC_StartAutoSamplingAsync(&data_ov013_021b3c80->micParam, func_ov013_021aeb78, 0);
            data_ov013_021b3c80->micRunning = 0;
        }
    } else {
        if (data_ov013_021b3c80->micRunning) {
            MIC_StartLimitedSamplingAsync(&data_ov013_021b3c80->micParam, func_ov013_021aeb78, 0);
            data_ov013_021b3c80->micRunning = 0;
        }
    }
    if ((u8 *)MIC_GetLastSamplingAddress() - samples < len) {
        samples += len;
    }
    if (PAD_DetectFold() || data_ov013_021b3c80->muted != 0) {
        samples = data_ov013_021b3c80->silence;
    }
    for (i = 0; i < numChannels; i++) {
        func_ov013_021b0b48(buffer[i], len, NULL);
    }
    if (data_ov013_021b3c80->stalled == 0) {
        func_ov013_021b07f8(samples, len);
    }
    data_ov013_021b3c80->micState = 2;
}

// pokebw2's RequestVoice
BOOL func_ov013_021aec74(u8 aid) {
    VCTSession *session = data_ov013_021b3c80->requestSession;
    int result;

    if (session == NULL) {
        session = func_ov013_021b1f2c(aid);
        if (session == NULL) {
            return FALSE;
        }
        result = func_ov013_021b20b0(session, 0);
    } else {
        result = func_ov013_021b20b0(session, 0);
    }
    if (result) {
        func_ov013_021b1f54(session);
        data_ov013_021b3c80->requestSession = NULL;
        return FALSE;
    }
    data_ov013_021b3c80->requestSession = session;
    return TRUE;
}

// pokebw2's AcceptVoice
BOOL func_ov013_021aecc4(u8 aid) {
    VCTSession *session = data_ov013_021b3c80->requestSession;

    if (session != NULL && session->state == 4) {
        if (func_ov013_021b2180(session, 0)) {
            return FALSE;
        }
        if (func_ov013_021b0614(data_ov013_021b3c80->requestSession)) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// pokebw2's ManualCallback
void func_ov013_021aed08(u8 aid, int event, VCTSession *session, void *userData) {
    switch (event) {
    case 9:
        func_ov013_021aedec(session);
        break;
    case 7:
        func_ov013_021b0614(session);
        break;
    case 12:
        func_ov013_021aedec(session);
        break;
    }
}

// pokebw2's SessionCallback
void func_ov013_021aed30(u8 aid, int event, VCTSession *session, void *userData) {
    switch (event) {
    case 1:
        if (data_ov013_021b3c80->requestSession != NULL) {
            func_ov013_021b2180(session, 3);
            func_ov013_021b1f54(session);
        } else {
            data_ov013_021b3c80->requestSession = session;
        }
        break;
    case 8:
        func_ov013_021b2180(session, 0);
        func_ov013_021aedec(session);
        break;
    case 9:
        func_ov013_021aedec(session);
        break;
    case 4:
        func_ov013_021b2180(session, 4);
        func_ov013_021aedec(session);
        break;
    case 7:
        if (session->active != data_ov013_021b3c80->callbackKind) {
            func_ov013_021aedec(session);
        } else if (func_ov013_021b0614(session)) {
            data_ov013_021b3c80->phase = 2;
        }
        break;
    case 2:
        func_ov013_021aedec(session);
        break;
    case 3:
    case 11:
    case 12:
        func_ov013_021aedec(session);
        break;
    }
}

// pokebw2's DropSession
void func_ov013_021aedec(VCTSession *session) {
    func_ov013_021b074c(session);
    func_ov013_021b1f54(session);
    data_ov013_021b3c80->requestSession = NULL;
    func_ov009_02156400();
}

void func_ov013_021aee14(BOOL stalled) {
    u64 now;
    u32 elapsed;

    data_ov013_021b3c80->stalled = stalled;
    now = OS_GetTick();
    elapsed = (u32)now - (u32)data_ov013_021b3c80->lastTick;
    data_ov013_021b3c80->tickBudget += elapsed * 0xfa00 / 0x82ea - 0x411a;
    if (data_ov013_021b3c80->tickBudget < -10000) {
        data_ov013_021b3c80->tickBudget = 0;
    }
    data_ov013_021b3c80->lastTick = now;
    func_ov013_021b386c();
    if (data_ov013_021b3c80->tickBudget >= 0x411a) {
        do {
            func_ov013_021b386c();
            data_ov013_021b3c80->tickBudget -= 0x411a;
        } while (data_ov013_021b3c80->tickBudget >= 0x411a);
    }
    if (data_ov013_021b3c80->callbackKind == 3) {
        return;
    }
    switch (data_ov013_021b3c80->phase) {
    case 0:
        if (DWC_GetMyAID() == 0) {
            if (func_ov013_021aec74(1)) {
                data_ov013_021b3c80->phase = 1;
                data_ov013_021b3c80->waitFrames = 60;
            }
        } else if (DWC_GetMyAID() == 1) {
            if (func_ov013_021aecc4(0)) {
                data_ov013_021b3c80->phase = 2;
            }
        }
        break;
    case 1:
        data_ov013_021b3c80->waitFrames--;
        if (data_ov013_021b3c80->waitFrames == 0) {
            data_ov013_021b3c80->phase = 0;
        }
        break;
    case 2:
        break;
    }
}


// pokebw2's DWCRapVChat_ReceivePacket
BOOL func_ov013_021aef44(int aid, u8 *data, int size) {
    if (data_ov013_021b3c80 == NULL) {
        return FALSE;
    }
    if (func_ov013_021b38b0(aid, data, size)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov013_021aef6c(HeapID heapId, int codec, int count) {
    VCTConfig config;
    u8 channels[3] = { 0, 0, 0 };
    BOOL ret;

    if (data_ov013_021b3c80 == NULL) {
        data_ov013_021b3c80 = func_ov009_02156f5c(heapId, sizeof(DWCRapVChatWork), 32);
        MI_CpuFill8(data_ov013_021b3c80, 0, sizeof(DWCRapVChatWork));
        data_ov013_021b3c80->packetMemory = func_ov009_02156f5c(heapId, 0x23c0 * count + 0x20, 32);
        data_ov013_021b3c80->onEnd = NULL;
        func_ov013_021aeb20();
    }
    data_ov013_021b3c80->micParam.type = MIC_SAMPLING_TYPE_SIGNED_12BIT;
    data_ov013_021b3c80->micParam.buffer = data_ov013_021b3c80->micRing;
    data_ov013_021b3c80->micParam.size = VCT_SAMPLE_BYTES;
    if (func_0202eb0c() == 0) {
        data_ov013_021b3c80->micParam.rate = 0x1040;
    } else {
        data_ov013_021b3c80->micParam.rate = 0x1001;
    }
    data_ov013_021b3c80->micParam.loop_enable = TRUE;
    data_ov013_021b3c80->micParam.full_callback = NULL;
    data_ov013_021b3c80->micParam.full_arg = NULL;
    data_ov013_021b3c80->micRunning = TRUE;
    NNS_SndStrmAllocChannel((NNSSndStrm *)data_ov013_021b3c80->stream, 1, channels);
    NNS_SndStrmSetVolume((NNSSndStrm *)data_ov013_021b3c80->stream, 0);
    ret = NNS_SndStrmSetup((NNSSndStrm *)data_ov013_021b3c80->stream, 1, data_ov013_021b3c80->streamBuffer,
                        VCT_SAMPLE_BYTES, func_0202eb0c() ? 0x40 : 0x41, 2, func_ov013_021aeb7c, data_ov013_021b3c80);
    GFL_ASSERT(ret);
    data_ov013_021b3c80->phase = 0;
    data_ov013_021b3c80->requestSession = NULL;
    config.mode = 1;
    data_ov013_021b3c80->callbackKind = 1;
    config.sessions = &data_ov013_021b3c80->session;
    config.sessionCount = count;
    config.aid = DWC_GetMyAID();
    GFL_ASSERT(config.aid != -1);
    if (data_ov013_021b3c80->callbackKind == 3) {
        config.callback = func_ov013_021aed08;
    } else {
        config.callback = func_ov013_021aed30;
    }
    config.userData = NULL;
    config.buffer = data_ov013_021b3c80->packetMemory;
    config.bufferSize = 0x23c0 * count + 0x20;
    func_ov013_021b370c(&config);
    data_ov013_021b3c80->stalled = 0;
    func_ov013_021b0c64(codec);
    func_ov013_021af124();
    func_ov013_021b0d08(TRUE);
}


// pokebw2's StartStream
void func_ov013_021af124(void) {
    NNS_SndStrmStart((NNSSndStrm *)data_ov013_021b3c80->stream);
}

// Ends the voice chat of the machine that asked
// pokebw2's DWCRapVChat_RequestEnd
void func_ov013_021af13c(void) {
    if (data_ov013_021b3c80->requestSession == NULL || data_ov013_021b3c80->phase == 0) {
        func_ov009_02156400();
        return;
    }
    if (data_ov013_021b3c80->phase == 1) {
        if (func_ov013_021b20b0(data_ov013_021b3c80->requestSession, 2)) {
            func_ov009_02156400();
            return;
        }
    }
    if (func_ov013_021b20b0(data_ov013_021b3c80->requestSession, 1)) {
        func_ov009_02156400();
    }
}

// pokebw2's DWCRapVChat_SetEndCallback
void func_ov013_021af18c(void (*callback)(void)) {
    data_ov013_021b3c80->onEnd = callback;
}

// pokebw2's DWCRapVChat_End
void func_ov013_021af1a0(void) {
    void (*onEnd)(void);

    if (data_ov013_021b3c80 != NULL) {
        onEnd = data_ov013_021b3c80->onEnd;
        if (!func_0202eb0c()) {
            MIC_StopAutoSampling();
        } else {
            MIC_StopLimitedSampling();
        }
        NNS_SndStrmStop((NNSSndStrm *)data_ov013_021b3c80->stream);
        NNS_SndStrmFreeChannel((NNSSndStrm *)data_ov013_021b3c80->stream);
        func_ov013_021b3844();
        PM_SetAmp(PM_AMP_OFF);
        func_ov009_02156fdc(0, data_ov013_021b3c80->packetMemory, 0);
        func_ov009_02156fdc(0, data_ov013_021b3c80, 0);
        data_ov013_021b3c80 = NULL;
        if (onEnd != NULL) {
            onEnd();
        }
    }
}

// The machine that is speaking, if the voice is loud enough
// pokebw2's DWCRapVChat_GetSpeaking
BOOL func_ov013_021af210(void) {
    s32 levels[4];

    if (data_ov013_021b3c80 != NULL) {
        func_ov013_021b3064(levels);
        if (levels[1] > 2) {
            return levels[0];
        }
    }
    return FALSE;
}

// pokebw2's DWCRapVChat_AddHeard
BOOL func_ov013_021af238(u32 netIds, int myNetId) {
    int i;

    if (data_ov013_021b3c80 == NULL || data_ov013_021b3c80->callbackKind != 3) {
        return FALSE;
    }
    for (i = 0; i < 2; i++) {
        if (i != myNetId && (netIds & (1 << i)) && data_ov013_021b3c80->heard[i] != 1) {
            if (func_ov013_021b21a4(i)) {
                return FALSE;
            }
            data_ov013_021b3c80->heard[i] = 1;
        }
    }
    return TRUE;
}

// pokebw2's DWCRapVChat_RemoveHeard
BOOL func_ov013_021af2ac(int myNetId) {
    int i;

    if (data_ov013_021b3c80 == NULL || data_ov013_021b3c80->callbackKind != 3) {
        return FALSE;
    }
    for (i = 0; i < 2; i++) {
        if (i != myNetId && data_ov013_021b3c80->heard[i] == 1) {
            if (func_ov013_021b2254(i)) {
                return FALSE;
            }
            data_ov013_021b3c80->heard[i] = 0;
        }
    }
    return TRUE;
}

// pokebw2's DWCRapVChat_SetMuted
void func_ov013_021af30c(BOOL muted) {
    if (data_ov013_021b3c80 != NULL) {
        data_ov013_021b3c80->muted = muted;
        if (data_ov013_021b3c80->requestSession != NULL) {
            if (muted) {
                func_ov013_021b074c(data_ov013_021b3c80->requestSession);
            } else {
                func_ov013_021b0614(data_ov013_021b3c80->requestSession);
            }
        }
    }
}
