#include "global.h"

struct SoundSystem;
struct WaveOutParam;
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
enum WaveOutChannel {
    WAVE_OUT_CHANNEL_PRIMARY = 14,
    WAVE_OUT_CHANNEL_SECONDARY = 15,
};
typedef struct WaveOutParam WaveOutParam;
BOOL sub_020056E8(WaveOutParam *param, enum WaveOutChannel channel);
NNSSndWaveOutHandle *sub_020055AC(enum WaveOutChannel channel);
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
void sub_02005680(enum WaveOutChannel channel);

NNSSndWaveOutHandle *sub_020055AC(enum WaveOutChannel channel);
BOOL sub_02005600(enum WaveOutChannel channel);
void sub_02005680(enum WaveOutChannel channel);
BOOL sub_020056E8(WaveOutParam *param, enum WaveOutChannel channel);

NNSSndWaveOutHandle *sub_020055AC(enum WaveOutChannel channel) {
    (void)(GetSoundDataPointer());
    u8 *primaryAllocated = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_ALLOCATED);
    u8 *secondaryAllocated = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_ALLOCATED);

    if (channel != WAVE_OUT_CHANNEL_PRIMARY && channel != WAVE_OUT_CHANNEL_SECONDARY) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    if (channel == WAVE_OUT_CHANNEL_PRIMARY && *primaryAllocated == 0) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    if (channel == WAVE_OUT_CHANNEL_SECONDARY && *secondaryAllocated == 0) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    if (channel == WAVE_OUT_CHANNEL_PRIMARY) {
        return GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_HANDLE);
    } else {
        return GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_HANDLE);
    }
}

BOOL sub_02005600(enum WaveOutChannel channel) {
    NNSSndWaveOutHandle *handle;

    (void)(GetSoundDataPointer());
    u8 *primaryAllocated = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_ALLOCATED);
    u8 *secondaryAllocated = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_ALLOCATED);

    if (channel != WAVE_OUT_CHANNEL_PRIMARY && channel != WAVE_OUT_CHANNEL_SECONDARY) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    if (channel == WAVE_OUT_CHANNEL_PRIMARY) {
        if (*primaryAllocated == 0) {
            handle = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_HANDLE);
            *handle = NNS_SndWaveOutAllocChannel(channel);

            if (*handle == 0L) {
                return 0;
            }

            *primaryAllocated = 1;
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }
    } else {
        if (*secondaryAllocated == 0) {
            handle = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_HANDLE);
            *handle = NNS_SndWaveOutAllocChannel(channel);

            if (*handle == 0L) {
                return 0;
            }

            *secondaryAllocated = 1;
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }
    }

    return 1;
}

void sub_02005680(enum WaveOutChannel channel) {
    (void)(GetSoundDataPointer());

    u8 *primaryAllocated = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_ALLOCATED);
    u8 *secondaryAllocated = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_ALLOCATED);

    if (channel != WAVE_OUT_CHANNEL_PRIMARY && channel != WAVE_OUT_CHANNEL_SECONDARY) {
        ((0) ? (void)0 : GF_AssertFail());
        return;
    }

    if (channel == WAVE_OUT_CHANNEL_PRIMARY) {
        if (*primaryAllocated == 1) {
            NNS_SndWaveOutFreeChannel(*sub_020055AC(channel));
            *primaryAllocated = 0;
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }
    } else {
        if (*secondaryAllocated == 1) {
            NNS_SndWaveOutFreeChannel(*sub_020055AC(channel));
            *secondaryAllocated = 0;
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }
    }
}

BOOL sub_020056E8(WaveOutParam *param, enum WaveOutChannel channel) {
    BOOL success = NNS_SndWaveOutStart(
        *param->handle,
        param->format,
        param->data,
        param->loop,
        param->loopStartSample,
        param->samples,
        param->sampleRate,
        param->volume,
        param->speed,
        param->pan);

    if (success == 0) {
        sub_02005680(channel);
    }

    return success;
}
