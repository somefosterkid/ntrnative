// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/snd_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "gfl/sound.h"
#include "nitro/os.h"
#include "nnsys/snd.h"
#include "work_area.h"

// The game's sound system: it opens the sound archive and sets up the players, plays the BGM (with the BGM stack of
// snd_bgm_stack.c, which pushes and pops a BGM with its sequence and banks loaded) and the sound effects, and changes
// the BGM with a fade, loading the next one in a thread while the old one fades out. The file's name is descriptive,
// a guess: the ROM has no string for it. Function names from swan (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0), except func_02005d14, func_02005f5c, func_02005f70, func_02005f90, func_02005fbc, func_02005ff4,
// func_02006300, func_02006368 and func_02006394. The data's names are swan's (g_SndPlayerStates,
// g_GFLSndThread and g_GFLSndHeap) or ours, as are the types, constants and fields

// The players of sound effects and the like, whose sequences GFL_SndSEPlay starts
#define SND_PLAYER_COUNT 5
#define SND_TRACK_COUNT 16
#define SND_THREAD_STACK_SIZE 0x400
#define SND_THREAD_PRIORITY 17
// The sound heap's (WORK_AREA_SIZE: its block headers hold pointers, 0x60 bytes each on a PC target, 0x20 on the DS)
#define SND_HEAP_SIZE WORK_AREA_SIZE(0x95ad0)
// The size GFL_SndInit creates the sound heap with, the array's
#define SND_HEAP_CREATE_SIZE WORK_AREA_SIZE(0x95ad0)
// What NNS_SndArcSetLoadBlockSize reads at a time while a thread loads the next BGM
#define SND_LOAD_BLOCK_SIZE 0x1300
// The player of BGM2, which plays over the BGM's
#define SND_PLAYER_BGM2 6

// How the next BGM loads: its sequence and bank, then its wave archives, each in the thread
enum {
    BGM_LOAD_IDLE,
    BGM_LOAD_SEQ,
    BGM_LOAD_WAVE,
};

// The kind of file the loading thread loads
enum {
    SND_THREAD_LOAD_SEQ,
    SND_THREAD_LOAD_WAVE,
};

// A change of BGM with a fade: the next BGM and its channel mask, how far it has loaded, and the fade's frames
typedef struct {
    BOOL active;
    s32 loadState;
    u16 fadeCounter;
    u16 channelMask;
    u32 seq;
    s32 fadeInFrames;
    s32 fadeOutFrames;
} BGMChange;

// A sound effect player: the sequence it was last started with, and its handle
typedef struct {
    u32 seq;
    NNSSndHandle handle;
} SndPlayerState;

void GFL_SndInitCore(BOOL loadGroup);
void func_02704364(void);
static BOOL GFL_SndSeqVerify(u32 seq);
static void func_02005d14(void);
static BOOL GFL_SndBGMPlayCore(u32 seq, u32 channelMask);
static void GFL_SndBGMFadeInEx(u16 frames, s32 volume);
void GFL_SndBGMSetChannelMask(u32 channelMask);
static void func_02005f5c(void);
static void func_02005f70(u32 seq, u16 channelMask, s32 fadeOutFrames, s32 fadeInFrames);
static void func_02005f90(void);
static void func_02005fbc(void);
static void func_02005ff4(void);
static void GFL_SndResetPlayerStates(void);
static void GFL_SndSEPlayCore(u32 seq, u32 volume);
static void GFL_SndThreadProc2(void *arg);
static void GFL_SndThreadProc1(void *arg);
static void func_02006300(u32 seq, int kind);
static void func_02006368(void);
static BOOL func_02006394(void);

// The tracks of the BGM that were playing in the last frame
static u16 data_020aa260;
// The sequence the loading thread loads
static u32 data_020aa26c;
// The loading thread, once started
static OSThread *sThread;
// Written once, never read
static u32 sUnused;
// The frames left of the BGM's fade in or out
static s32 sFadeFrames;
static BOOL (*sSeqVerifyCallback)(u32 seq);
static NNSSndHeapHandle data_020aa270;
static BGMChange sBGMChange;
static SndPlayerState g_SndPlayerStates[SND_PLAYER_COUNT];
static NNSSndArc sSndArc;
OSThread g_GFLSndThread;
static SNDTrackInfo data_020aa414[SND_TRACK_COUNT];
static u32 sThreadStack[SND_THREAD_STACK_SIZE / sizeof(u32)];
u8 g_GFLSndHeap[SND_HEAP_SIZE];


void GFL_SndInit(void) {
    NNS_SndInit();
    data_020aa270 = NNS_SndHeapCreate(g_GFLSndHeap, SND_HEAP_CREATE_SIZE);
    NNS_SndArcInit(&sSndArc, "wb_sound_data.sdat", data_020aa270, FALSE);
    GFL_SndInitCore(TRUE);
    if (OS_IsRunOnTwl() == TRUE) {
        func_02704364();
    }
}

void GFL_SndInitCore(BOOL loadGroup) {
    NNS_SndArcPlayerSetup(data_020aa270);
    func_020057dc(&data_020aa270);
    if (loadGroup) {
        // The group's info goes unused
        NNS_SndArcGetGroupInfo(0);
        NNS_SndArcLoadGroup(0, data_020aa270);
        sUnused = 0;
    }
    GFL_SndResetPlayerStates();
    func_02005888(0);
    func_02005f5c();
    data_020aa260 = 0;
    sFadeFrames = 0;
    sThread = NULL;
    sSeqVerifyCallback = NULL;
}

void GFL_SndUpdate(void) {
    NNSSndHandle *handle;
    int i;

    func_02005ff4();
    NNS_SndMain();
    NNS_SndUpdateDriverInfo();
    handle = func_02005c3c();
    data_020aa260 = 0;
    for (i = 0; i < SND_TRACK_COUNT; i++) {
        if (NNS_SndPlayerReadDriverTrackInfo(handle, i, &data_020aa414[i]) == TRUE) {
            data_020aa260 |= (u16)(1 << i);
        }
    }
    if (sFadeFrames != 0) {
        sFadeFrames--;
    }
}

// pokebw2's GFL_SndDestroyHeap
void func_02005c18(void) {
    NNS_SndHeapDestroy(data_020aa270);
}

void func_02005c28(BOOL stereo) {
    NNS_SndSetMonoFlag(stereo == TRUE ? FALSE : TRUE);
}

NNSSndHandle *func_02005c3c(void) {
    return func_0200590c();
}

u32 GFL_SndBGMGetID(void) {
    return GFL_SndGetLastResID();
}

u32 func_02005c4c(void) {
    if (sBGMChange.active == FALSE) {
        return GFL_SndGetLastResID();
    }
    return sBGMChange.seq;
}

BOOL func_02005c64(void) {
    if (sThread == NULL) {
        return FALSE;
    }
    if (OS_IsThreadTerminated(&g_GFLSndThread) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL GFL_IRQCartDataTransferIsEnabled(void) {
    if (reg_OS_IE & OS_IE_CARD_DATA) {
        return TRUE;
    }
    return FALSE;
}

void GFL_SndPlayerSetMuteStateEx(BOOL on, u32 playerMask) {
    if (on == TRUE) {
        GFL_SndPlayerSetVolumeEx(SND_VOLUME_MAX, playerMask);
    } else {
        GFL_SndPlayerSetVolumeEx(0, playerMask);
    }
}

void GFL_SndPlayerSetVolumeEx(u32 volume, u32 playerMask) {
    int i;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        if ((1 << i) & playerMask) {
            NNS_SndPlayerSetPlayerVolume(i, volume);
        }
    }
}

static BOOL GFL_SndSeqVerify(u32 seq) {
    if (func_02005c64() == FALSE) {
        return TRUE;
    }
    if (sSeqVerifyCallback == NULL) {
        return FALSE;
    }
    return sSeqVerifyCallback(seq);
}

// pokebw2's GFL_SndSetSeqVerifyCallback
void func_02005d08(BOOL (*callback)(u32 seq)) {
    sSeqVerifyCallback = callback;
}

// pokebw2's StopBGM
static void func_02005d14(void) {
    NNS_SndPlayerStopSeq(func_0200590c(), 0);
    func_020059c8();
}

void func_02005d28(void) {
    func_02005f90();
    func_02005d14();
}

static BOOL GFL_SndBGMPlayCore(u32 seq, u32 channelMask) {
    BOOL result;

    if (GFL_SndSeqVerify(seq) == FALSE) {
        return FALSE;
    }
    func_02005d14();
    if (func_02005924(seq) == FALSE) {
        return FALSE;
    }
    result = NNS_SndArcPlayerStartSeqEx(func_0200590c(), 0, -1, -1, seq);
    if (result == FALSE) {
        return FALSE;
    }
    if (channelMask != SND_CHANNEL_MASK_ALL) {
        GFL_SndBGMSetChannelMask(channelMask);
    }
    return result;
}

void GFL_SndBGMPlay(u32 bgm, u32 channelMask) {
    func_02005f90();
    GFL_SndBGMPlayCore(bgm, channelMask);
}

void func_02005da4(u32 bgm, u16 channelMask, s32 fadeOutFrames, s32 fadeInFrames) {
    func_02005f70(bgm, channelMask, fadeOutFrames, fadeInFrames);
}

void GFL_SndBGMStop(u32 bgm) {
    func_02005f90();
    if (GFL_SndSeqVerify(bgm)) {
        func_02005d14();
        if (func_02005924(bgm)) {
            NNS_SndArcPlayerStartSeqEx(func_0200590c(), SND_PLAYER_BGM2, -1, -1, bgm);
        }
    }
}

void GFL_SndBGMSetPaused(BOOL paused) {
    func_02005fbc();
    NNS_SndPlayerPause(func_0200590c(), paused);
}

void GFL_SndBGMFadeIn(u16 frames) {
    GFL_SndBGMFadeInEx(frames, SND_VOLUME_MAX);
}

static void GFL_SndBGMFadeInEx(u16 frames, s32 volume) {
    func_02005fbc();
    NNS_SndPlayerMoveVolume(func_0200590c(), 0, 0);
    NNS_SndPlayerMoveVolume(func_0200590c(), volume, frames);
    sFadeFrames = frames;
}

void GFL_SndBGMFadeOut(u16 frames) {
    func_02005fbc();
    NNS_SndPlayerMoveVolume(func_0200590c(), 0, frames);
    sFadeFrames = frames;
}

BOOL GFL_SndBGMIsFading(void) {
    if (sFadeFrames != 0) {
        return TRUE;
    }
    return FALSE;
}

void GFL_SndBGMPush(void) {
    func_02005fbc();
    NNS_SndPlayerSetTrackMute(func_0200590c(), SND_CHANNEL_MASK_ALL, FALSE);
    NNS_SndPlayerSetTrackModDepth(func_0200590c(), SND_CHANNEL_MASK_ALL, 0);
    NNS_SndPlayerSetTrackModSpeed(func_0200590c(), SND_CHANNEL_MASK_ALL, 16);
    func_02005a00();
}

void GFL_SndBGMPop(void) {
    func_02005fbc();
    if (func_02005900() != 0) {
        func_02005d14();
        func_02005a4c();
    }
}

void GFL_SndBGMSetChannelMask(u32 channelMask) {
    u16 muted = channelMask ^ SND_CHANNEL_MASK_ALL;

    NNS_SndPlayerSetTrackMute(func_0200590c(), SND_CHANNEL_MASK_ALL, FALSE);
    if (muted != 0) {
        NNS_SndPlayerSetTrackMuteEx(func_0200590c(), muted, 1);
    }
}

void GFL_SndBGMSetParams(u16 trackMask, s32 tempoRatio, s32 pitch, s32 pan) {
    NNSSndHandle *handle = func_0200590c();

    if (tempoRatio != -1) {
        NNS_SndPlayerSetTempoRatio(handle, tempoRatio);
    }
    if (pitch != -1) {
        NNS_SndPlayerSetTrackPitch(handle, trackMask, pitch);
    }
    if (pan != -1) {
        NNS_SndPlayerSetTrackPan(handle, trackMask, pan);
    }
}

void GFL_SndBGMSetVolume(u16 trackMask, s32 volume) {
    NNS_SndPlayerSetTrackVolume(func_0200590c(), trackMask, volume);
}

BOOL GFL_SndBGMIsPlaying(void) {
    if (GFL_SndGetLastResID() == 0) {
        return FALSE;
    }
    if (NNS_SndPlayerCountPlayingSeqBySeqNo(GFL_SndGetLastResID()) != 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's InitBGMChange
static void func_02005f5c(void) {
    sBGMChange.fadeInFrames = 60;
    sBGMChange.fadeOutFrames = 60;
    func_02005f90();
}

// pokebw2's RequestBGMChange
static void func_02005f70(u32 seq, u16 channelMask, s32 fadeOutFrames, s32 fadeInFrames) {
    sBGMChange.channelMask = channelMask;
    sBGMChange.seq = seq;
    sBGMChange.fadeInFrames = fadeInFrames;
    sBGMChange.fadeOutFrames = fadeOutFrames;
    sBGMChange.fadeCounter = fadeOutFrames;
    func_02006368();
    sBGMChange.loadState = BGM_LOAD_IDLE;
    sBGMChange.active = TRUE;
}

// pokebw2's ResetBGMChange
static void func_02005f90(void) {
    sBGMChange.active = FALSE;
    sBGMChange.channelMask = SND_CHANNEL_MASK_ALL;
    sBGMChange.seq = 0;
    sBGMChange.fadeCounter = 0;
    NNS_SndPlayerSetVolume(func_0200590c(), SND_VOLUME_MAX);
    sBGMChange.loadState = BGM_LOAD_IDLE;
    func_02006368();
}

// pokebw2's FinishBGMChange
static void func_02005fbc(void) {
    u32 seq;

    if (sBGMChange.active == TRUE) {
        seq = GFL_SndGetLastResID();
        if (seq != sBGMChange.seq) {
            while (sBGMChange.loadState != BGM_LOAD_IDLE) {
                func_02005ff4();
            }
            GFL_SndBGMPlay(sBGMChange.seq, sBGMChange.channelMask);
        }
        func_02005f90();
    }
}

// pokebw2's UpdateBGMChange
static void func_02005ff4(void) {
    NNSSndHandle *handle = func_0200590c();
    u32 seq = GFL_SndGetLastResID();

    if (sBGMChange.active == FALSE) {
        return;
    }
    switch (sBGMChange.loadState) {
    case BGM_LOAD_IDLE:
        if (seq != 0) {
            if (seq != sBGMChange.seq) {
                if (sBGMChange.fadeCounter != 0) {
                    sBGMChange.fadeCounter--;
                    NNS_SndPlayerSetVolume(handle, SND_VOLUME_MAX * sBGMChange.fadeCounter / sBGMChange.fadeOutFrames);
                    return;
                }
                NNS_SndPlayerSetVolume(handle, 0);
            } else {
                if (sBGMChange.fadeCounter < sBGMChange.fadeInFrames) {
                    sBGMChange.fadeCounter++;
                    NNS_SndPlayerSetVolume(handle, SND_VOLUME_MAX * sBGMChange.fadeCounter / sBGMChange.fadeInFrames);
                    return;
                }
                func_02005f90();
                return;
            }
        }
        func_02005d14();
        if (sBGMChange.seq == 0) {
            func_02005f90();
            return;
        }
        NNS_SndArcSetLoadBlockSize(SND_LOAD_BLOCK_SIZE);
        func_02006300(sBGMChange.seq, SND_THREAD_LOAD_SEQ);
        sBGMChange.fadeCounter = 0;
        sBGMChange.loadState = BGM_LOAD_SEQ;
        break;
    case BGM_LOAD_SEQ:
        if (func_02006394() == TRUE) {
            func_0200597c();
            func_02006300(sBGMChange.seq, SND_THREAD_LOAD_WAVE);
            sBGMChange.loadState = BGM_LOAD_WAVE;
        } else {
            OS_Sleep(1);
        }
        break;
    case BGM_LOAD_WAVE:
        if (func_02006394() == TRUE) {
            NNS_SndArcSetLoadBlockSize(0);
            func_020059a0(sBGMChange.seq);
            NNS_SndArcPlayerStartSeqEx(func_0200590c(), 0, -1, -1, sBGMChange.seq);
            NNS_SndPlayerSetVolume(handle, 0);
            sBGMChange.loadState = BGM_LOAD_IDLE;
        } else {
            OS_Sleep(1);
        }
        break;
    }
}

static void GFL_SndResetPlayerStates(void) {
    int i;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        g_SndPlayerStates[i].seq = 0;
        NNS_SndHandleInit(&g_SndPlayerStates[i].handle);
    }
}

NNSSndHandle *func_02006138(s32 player) {
    return &g_SndPlayerStates[player].handle;
}

s32 GFL_SndSeqGetPlayerIndex(u32 seq) {
    if (seq < SEQ_SE_DUMMY || seq >= SEQ_SE_END) {
        return 1;
    }
    return NNS_SndArcGetSeqParam(seq)->playerNo - 1;
}

void GFL_SEPlayKeepVol(u32 se, s32 player) {
    SndPlayerState *state;

    if (GFL_SndSeqVerify(se)) {
        state = &g_SndPlayerStates[player];
        state->seq = 0;
        if (NNS_SndArcPlayerStartSeqEx(&state->handle, player + 1, -1, -1, se) == TRUE) {
            state->seq = se;
        }
    }
}

static void GFL_SndSEPlayCore(u32 seq, u32 volume) {
    s32 player;
    SndPlayerState *state;

    if (GFL_SndSeqVerify(seq)) {
        player = GFL_SndSeqGetPlayerIndex(seq);
        state = &g_SndPlayerStates[player];
        state->seq = 0;
        if (NNS_SndArcPlayerStartSeq(&state->handle, seq) == TRUE) {
            state->seq = seq;
            if (volume < 128) {
                GFL_SndPlayerSetVolume(player, volume);
            }
        }
    }
}

void GFL_SndSEPlay(u32 se) {
    GFL_SndSEPlayCore(se, -1);
}

void GFL_SndSEPlayEx(u32 se, u32 volume) {
    GFL_SndSEPlayCore(se, volume);
}

void GFL_SndPlayerStop(s32 player) {
    NNS_SndPlayerStopSeq(&g_SndPlayerStates[player].handle, 0);
}

void GFL_SndStop(void) {
    int i;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        GFL_SndPlayerStop(i);
    }
}

BOOL GFL_SndPlayerIsActive(s32 player) {
    if (NNS_SndPlayerCountPlayingSeqByPlayerNo(player + 1) != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL GFL_SndPlayerIsActiveAny(void) {
    int i;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        if (GFL_SndPlayerIsActive(i) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL GFL_SndIsPlaying(u32 seq) {
    if (NNS_SndPlayerCountPlayingSeqBySeqNo(seq) != 0) {
        return TRUE;
    }
    return FALSE;
}

void GFL_SndPlayerSetParams(s32 player, s32 tempoRatio, s32 pitch, s32 pan) {
    if (tempoRatio != -1) {
        NNS_SndPlayerSetTempoRatio(&g_SndPlayerStates[player].handle, tempoRatio);
    }
    if (pitch != -1) {
        NNS_SndPlayerSetTrackPitch(&g_SndPlayerStates[player].handle, SND_CHANNEL_MASK_ALL, pitch);
    }
    if (pan != -1) {
        NNS_SndPlayerSetTrackPan(&g_SndPlayerStates[player].handle, SND_CHANNEL_MASK_ALL, pan);
    }
}

void GFL_SndPlayerSetVolume(s32 player, u32 volume) {
    if (volume > SND_VOLUME_MAX) {
        volume = SND_VOLUME_MAX;
    }
    NNS_SndPlayerSetVolume(&g_SndPlayerStates[player].handle, volume);
}

static void GFL_SndThreadProc2(void *arg) {
    NNS_SndArcLoadSeqEx(*(u32 *)arg, NNS_SND_ARC_LOAD_SEQ | NNS_SND_ARC_LOAD_BANK, data_020aa270);
}

static void GFL_SndThreadProc1(void *arg) {
    NNS_SndArcLoadSeqEx(*(u32 *)arg, NNS_SND_ARC_LOAD_WAVE, data_020aa270);
}

// pokebw2's StartLoadThread
static void func_02006300(u32 seq, int kind) {
    data_020aa26c = seq;
    func_02006368();
    switch (kind) {
    case SND_THREAD_LOAD_SEQ:
        OS_CreateThread(&g_GFLSndThread, GFL_SndThreadProc2, &data_020aa26c, sThreadStack + NELEMS(sThreadStack),
                              SND_THREAD_STACK_SIZE, SND_THREAD_PRIORITY);
        break;
    case SND_THREAD_LOAD_WAVE:
        OS_CreateThread(&g_GFLSndThread, GFL_SndThreadProc1, &data_020aa26c, sThreadStack + NELEMS(sThreadStack),
                              SND_THREAD_STACK_SIZE, SND_THREAD_PRIORITY);
        break;
    }
    OS_WakeupThreadDirect(&g_GFLSndThread);
    sThread = &g_GFLSndThread;
}

// pokebw2's StopLoadThread
static void func_02006368(void) {
    if (sThread != NULL && OS_IsThreadTerminated(&g_GFLSndThread) == FALSE) {
        OS_DestroyThread(&g_GFLSndThread);
        sThread = NULL;
    }
}

// pokebw2's IsLoadThreadDone
static BOOL func_02006394(void) {
    if (sThread == NULL) {
        return TRUE;
    }
    return OS_IsThreadTerminated(&g_GFLSndThread);
}

BOOL func_020063b4(u32 seq, u32 *step, BOOL start) {
    if (start == TRUE) {
        func_02005d14();
        func_02006368();
        *step = 0;
    } else {
        switch (*step) {
        case 0:
            NNS_SndArcSetLoadBlockSize(SND_LOAD_BLOCK_SIZE);
            func_02006300(seq, SND_THREAD_LOAD_SEQ);
            (*step)++;
            break;
        case 1:
            if (func_02006394() == TRUE) {
                func_0200597c();
                func_02006300(seq, SND_THREAD_LOAD_WAVE);
                (*step)++;
            } else {
                OS_Sleep(2);
            }
            break;
        case 2:
            if (func_02006394() == TRUE) {
                NNS_SndArcSetLoadBlockSize(0);
                func_020059a0(seq);
                NNS_SndArcPlayerStartSeqEx(func_0200590c(), 0, -1, -1, seq);
                (*step)++;
                return TRUE;
            }
            OS_Sleep(2);
            break;
        }
    }
    return FALSE;
}

BOOL func_02006448(void *seqData, void *bankData, u32 seq) {
    const NNSSndArcSeqInfo *seqInfo = NNS_SndArcGetSeqInfo(seq);
    u32 seqFileId = seqInfo->fileId;
    const NNSSndArcBankInfo *bankInfo = NNS_SndArcGetBankInfo(seqInfo->param.bankNo);
    u32 bankFileId = bankInfo->fileId;
    int i;

    func_0200597c();
    for (i = 0; i < NNS_SND_ARC_BANK_TO_WAVEARC_NUM; i++) {
        if (bankInfo->waveArcNo[i] != NNS_SND_ARC_INVALID_WAVEARC_NO) {
            if (NNS_SndArcLoadWaveArc(bankInfo->waveArcNo[i], data_020aa270) == FALSE) {
                return FALSE;
            }
        }
    }
    NNS_SndArcSetFileAddress(bankFileId, bankData);
    NNS_SndArcSetFileAddress(seqFileId, seqData);
    func_020059a0(seq);
    return TRUE;
}

BOOL func_020064b8(u16 waveArc, u16 index, const SNDWaveArc *srcWaveArc, u16 srcIndex) {
    const NNSSndArcWaveArcInfo *info = NNS_SndArcGetWaveArcInfo(waveArc);
    SNDWaveArc *loaded;

    if (info == NULL) {
        return FALSE;
    }
    loaded = (SNDWaveArc *)NNS_SndArcGetFileAddress(info->fileId);
    if (loaded == NULL) {
        return FALSE;
    }
    SND_SetWaveDataAddress(loaded, index, SND_GetWaveDataAddress(srcWaveArc, srcIndex));
    return TRUE;
}

void func_020064f4(u16 seq) {
    NNS_SndArcPlayerStartSeq(func_0200590c(), seq);
}

void func_02006504(void) {
    NNS_SndPlayerStopSeq(func_0200590c(), 0);
    func_020059c8();
}

// Clears the addresses of the files of bank and sequence 0x3f2 (SEQ_BGM_SHINKA), as func_02006448 sets them to data
// in memory
void func_02006518(void) {
    u32 bankFileId = NNS_SndArcGetBankInfo(0x3f2)->fileId;
    u32 seqFileId = NNS_SndArcGetSeqInfo(0x3f2)->fileId;

    NNS_SndArcSetFileAddress(bankFileId, NULL);
    NNS_SndArcSetFileAddress(seqFileId, NULL);
}
