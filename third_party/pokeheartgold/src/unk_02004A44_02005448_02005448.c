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

enum SoundHandleType GF_GetSndHandleByPlayerNo(int playerID);

void sub_02005464(int seqID, enum SoundHandleType handleType);

u8 GF_GetPlayerNoBySeq(u16 seqID);

BOOL Sound_PlaySequenceWithPlayer(enum SoundHandleType handleType, int playerID, u16 seqID);

void sub_02005448(int seqID);

void sub_02005448(int seqID) {
    u8 playerID = GF_GetPlayerNoBySeq(seqID);
    enum SoundHandleType handleType = GF_GetSndHandleByPlayerNo(playerID);

    sub_02005464(seqID, handleType);
}
