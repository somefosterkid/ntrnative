#include "global.h"

#include "constants/heap.h"

#include "heap.h"

struct SoundSystem;
struct WaveOutParam;
enum WaveOutChannel {
    WAVE_OUT_CHANNEL_PRIMARY = 14,
    WAVE_OUT_CHANNEL_SECONDARY = 15,
};
NNSSndWaveOutHandle *sub_020055AC(enum WaveOutChannel channel);
BOOL sub_020378CC(void);
enum SoundSystemParam {
    SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_HANDLE = 0,
    SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_HANDLE,
    SOUND_SYSTEM_PARAM_CURRENT_BANK_INFO,
    SOUND_SYSTEM_PARAM_CAPTURE_BUFFER,
    SOUND_SYSTEM_PARAM_FILTER_CALLBACK_PARAM,
    SOUND_SYSTEM_PARAM_BGM_FIXED,
    SOUND_SYSTEM_PARAM_ACTIVE_CRY,
    SOUND_SYSTEM_PARAM_FADE_COUNTER,
    SOUND_SYSTEM_PARAM_FOLLOW_UP_WAIT_FRAMES,
    SOUND_SYSTEM_PARAM_FOLLOW_UP_FADE_FRAMES,
    SOUND_SYSTEM_PARAM_CURRENT_BGM,
    SOUND_SYSTEM_PARAM_NEXT_BGM,
    SOUND_SYSTEM_PARAM_FIELD_BGM_PAUSED,
    SOUND_SYSTEM_PARAM_BGM_PAUSED,
    SOUND_SYSTEM_PARAM_FANFARE_DELAY,
    SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSED_PLAYBACK,
    SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_ALLOCATED,
    SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_ALLOCATED,
    SOUND_SYSTEM_PARAM_ECHO_ENABLED,
    SOUND_SYSTEM_PARAM_FIELD_BGM_BANK_STATE,
    SOUND_SYSTEM_PARAM_FILTER_SIZE,

    SOUND_SYSTEM_PARAM_MAIN_SCENE,
    SOUND_SYSTEM_PARAM_SUB_SCENE,

    SOUND_SYSTEM_PARAM_HEAP_STATE_EMPTY,
    SOUND_SYSTEM_PARAM_HEAP_STATE_PERSISTENT,
    SOUND_SYSTEM_PARAM_HEAP_STATE_BGM_BANK,
    SOUND_SYSTEM_PARAM_HEAP_STATE_SFX,
    SOUND_SYSTEM_PARAM_HEAP_STATE_BGM,
    SOUND_SYSTEM_PARAM_HEAP_STATE_SUB_SFX,
    SOUND_SYSTEM_PARAM_HEAP_STATE_FANFARE,

    SOUND_SYSTEM_PARAM_CHATOT_CRY_PLAYING,
    SOUND_SYSTEM_PARAM_DEFAULT_CHATOT_CRY,
    SOUND_SYSTEM_PARAM_FIELD_BGM,
    SOUND_SYSTEM_PARAM_CURRENT_WAVE_DATA,
    SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSE_BUFFER,
    SOUND_SYSTEM_PARAM_CRY_DURATION_TASK,
    SOUND_SYSTEM_PARAM_CHATOT_CRY,

    SOUND_SYSTEM_PARAM_UNUSED_37,
    SOUND_SYSTEM_PARAM_UNUSED_38,
    SOUND_SYSTEM_PARAM_UNUSED_39,
    SOUND_SYSTEM_PARAM_UNUSED_40,

    SOUND_SYSTEM_PARAM_CRY_A_MOD,
    SOUND_SYSTEM_PARAM_CRY_A_PAN,
    SOUND_SYSTEM_PARAM_CRY_A_VOLUME,
    SOUND_SYSTEM_PARAM_CRY_A_HEAP_ID,
    SOUND_SYSTEM_PARAM_CRY_A_WAVE_ID,
    SOUND_SYSTEM_PARAM_CRY_A_DELAY,

    SOUND_SYSTEM_PARAM_CRY_B_MOD,
    SOUND_SYSTEM_PARAM_CRY_B_PAN,
    SOUND_SYSTEM_PARAM_CRY_B_VOLUME,
    SOUND_SYSTEM_PARAM_CRY_B_HEAP_ID,
    SOUND_SYSTEM_PARAM_CRY_B_WAVE_ID,
    SOUND_SYSTEM_PARAM_CRY_B_DELAY,

    SOUND_SYSTEM_PARAM_ALLOW_2_POKEMON_CRIES,

    SOUND_SYSTEM_PARAM_54,
};
typedef struct SoundSystem SoundSystem;
SoundSystem *GetSoundDataPointer(void);
void *GF_SdatGetAttrPtr(enum SoundSystemParam param);
typedef struct WaveOutParam WaveOutParam;
struct WaveOutParam {
    NNSSndWaveOutHandle *handle;
    NNSSndWaveFormat format;
    const void *data;
    BOOL loop;
    int loopStartSample;
    int samples;
    int sampleRate;
    int volume;
    int speed;
    int pan;
};
BOOL sub_020056E8(WaveOutParam *param, enum WaveOutChannel channel);
void sub_02005774(enum WaveOutChannel channel, int volume);
static void sub_02005898(u8 *buffer, u32 size);
void sub_02005728(enum WaveOutChannel channel);

void sub_02005774(enum WaveOutChannel channel, int volume);
BOOL sub_020057AC(u16 waveArcID, int volume, int pan, enum WaveOutChannel channel, enum HeapID heapID);
static void sub_02005898(u8 *buffer, u32 size);
void sub_020058B8(enum WaveOutChannel channel);

void sub_02005774(enum WaveOutChannel channel, int volume) {
    if (sub_020378CC() == 1) {
        NNS_SndWaveOutSetVolume(*sub_020055AC(channel), volume / 5);
    } else {
        NNS_SndWaveOutSetVolume(*sub_020055AC(channel), volume);
    }
}

BOOL sub_020057AC(u16 waveArcID, int volume, int pan, enum WaveOutChannel channel, enum HeapID heapID) {
    (void)(GetSoundDataPointer());
    void **reverseBuffer = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSE_BUFFER);

    if (channel != WAVE_OUT_CHANNEL_PRIMARY && channel != WAVE_OUT_CHANNEL_SECONDARY) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    const NNSSndArcWaveArcInfo *info = NNS_SndArcGetWaveArcInfo(waveArcID);
    if (info == 0L) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    u32 fileSize = NNS_SndArcGetFileSize(info->fileId);
    if (fileSize == 0) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    if (channel == WAVE_OUT_CHANNEL_PRIMARY) {
        *reverseBuffer = Heap_Alloc(heapID, fileSize);
        if (*reverseBuffer == 0L) {
            ((0) ? (void)0 : GF_AssertFail());
            return 0;
        }

        memset(*reverseBuffer, 0, fileSize);

        if (NNS_SndArcReadFile(info->fileId, *reverseBuffer, fileSize, 0) == -1) {
            ((0) ? (void)0 : GF_AssertFail());
            return 0;
        }

        sub_02005898(*reverseBuffer, fileSize);
    }

    WaveOutParam param;
    param.handle = sub_020055AC(channel);
    param.format = NNS_SND_WAVE_FORMAT_PCM8;
    param.data = *reverseBuffer;
    param.loop = 0;
    param.loopStartSample = 0;
    param.samples = fileSize;
    param.sampleRate = 13379;
    param.volume = volume;
    param.speed = (int)((0.75) * 32768);
    param.pan = pan;

    BOOL result = sub_020056E8(&param, channel);

    sub_02005774(channel, volume);

    u8 *reversedPlayback = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSED_PLAYBACK);
    *reversedPlayback = 1;

    return result;
}

static void sub_02005898(u8 *buffer, u32 size) {
    for (int i = 0; i < (size / 2); i++) {
        u8 tmp = buffer[i];
        buffer[i] = buffer[size - 1 - i];
        buffer[size - 1 - i] = tmp;
    }
}

void sub_020058B8(enum WaveOutChannel channel) {
    (void)(GetSoundDataPointer());
    u8 *reversedPlaybackEnabled = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSED_PLAYBACK);
    void **reverseBuffer = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSE_BUFFER);

    if (channel != WAVE_OUT_CHANNEL_PRIMARY && channel != WAVE_OUT_CHANNEL_SECONDARY) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    sub_02005728(channel);

    if (*reversedPlaybackEnabled == 1) {
        *reversedPlaybackEnabled = 0;
        Heap_Free(*reverseBuffer);
    }
}
