#include "global.h"

#include "sound.h"
#include "unk_02005D10.h"

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

void sub_020061EC(s32 arg0);

BOOL GF_IsAnySEPlaying(void) {
    for (int i = 0; i < NUM_SFX_HANDLES; i++) {
        if (GF_SndPlayerCountPlayingSeqByPlayerNo(PLAYER_SE_1 + i) == 1) {
            return TRUE;
        }
    }

    return FALSE;
}

void sub_020061B4(u16 seqID, u16 param_2, s32 param_3);

void sub_020061B4(u16 seqID, u16 param_2, s32 param_3) {
    s32 playerNo;
    enum SoundHandleNo playerNo_00;
    NNSSndHandle *handle;

    playerNo = GF_GetPlayerNoBySeq(seqID);
    playerNo_00 = GF_GetSndHandleByPlayerNo(playerNo);
    handle = GF_GetSoundHandle(playerNo_00);
    NNS_SndPlayerSetTrackPan(handle, param_2, param_3);
    return;
}

void sub_020061D0(s32 param_1, s32 param_2);

void sub_020061D0(s32 param_1, s32 param_2) {
    enum SoundHandleNo playerNo;
    NNSSndHandle *handle;

    playerNo = GF_GetSndHandleByPlayerNo(param_1);
    handle = GF_GetSoundHandle(playerNo);
    NNS_SndPlayerSetTrackPan(handle, 0xffff, param_2);
    return;
}

void sub_020061EC(s32 pan) {
    enum SoundHandleNo v0;
    int v1;

    v0 = GF_GetSndHandleByPlayerNo(3);
    v1 = 0;
    do {
        NNS_SndPlayerSetTrackPan(GF_GetSoundHandle(v0 + v1), 0xFFFFU, pan);
        v1 += 1;
    } while (v1 < 4);
}
