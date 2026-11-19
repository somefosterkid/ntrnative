#include "global.h"

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
BOOL sub_020378CC(void);

void sub_02005464(int seqID, enum SoundHandleType handleType);

void sub_02005464(int seqID, enum SoundHandleType handleType) {
    u8 volume;
    const NNSSndSeqParam *param = NNS_SndArcGetSeqParam(seqID);

    switch (handleType) {
    case SOUND_HANDLE_TYPE_POKEMON_CRY:
    case SOUND_HANDLE_TYPE_ECHO:
        volume = 127;
        break;

    default:
        if (param == 0L) {
            return;
        }

        volume = param->volume;
        break;
    }

    if (sub_020378CC() == 1) {
        GF_SndHandleSetInitialVolume(handleType, (volume / 5));
    }
}
