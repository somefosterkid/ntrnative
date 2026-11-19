#include "global.h"

#include "sound.h"

void GF_SndHandleMoveVolume(s32 arg0, s32 arg1, s32 arg2);

enum SoundHandleType {
    SOUND_HANDLE_TYPE_FIELD_BGM = 0,
    SOUND_HANDLE_TYPE_POKEMON_CRY,
    SOUND_HANDLE_TYPE_FANFARE,
    SOUND_HANDLE_TYPE_SFX_1,
    SOUND_HANDLE_TYPE_SFX_2,
    SOUND_HANDLE_TYPE_SFX_3,
    SOUND_HANDLE_TYPE_SFX_4,
    SOUND_HANDLE_TYPE_BGM,
    SOUND_HANDLE_TYPE_ECHO,

    SOUND_HANDLE_TYPE_COUNT,

    NUM_SFX_HANDLES = SOUND_HANDLE_TYPE_SFX_4 - SOUND_HANDLE_TYPE_SFX_1 + 1,
};
void GF_SndHandleSetInitialVolume(enum SoundHandleType handleType, int volume);

void GF_SndHandleMoveVolume(s32 playerNo, s32 arg1, s32 frames) {
    NNS_SndPlayerMoveVolume(GF_GetSoundHandle(playerNo), arg1, frames);
    if (playerNo == 0) {
        GF_SndWorkSetGbSoundsVolume((u8)arg1);
    }
}

void GF_SndHandleSetInitialVolume(enum SoundHandleType handleType, int volume) {
    if (volume < 0) {
        volume = 0;
    }

    if (volume > 127) {
        volume = 127;
    }

    NNS_SndPlayerSetInitialVolume(GF_GetSoundHandle(handleType), volume);
}
