#include "global.h"
#include "constants/heap.h"

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

BOOL PlaySE(u16 seqID);

void sub_020061B4(u16 seqID, u16 tracks, int pan);

void sub_02005DA0(s32 seqID, enum SoundHandleType handleType);

BOOL Sound_Impl_PlayBGM(u16 seqID, u8 playerID, enum SoundHandleType handleType);

BOOL Sound_Impl_PlayFieldBGM(u16 seqID, u8 playerID, enum SoundHandleType handleType);

void sub_02006820(s32 species, enum SoundHandleType handleType, int volume);

BOOL sub_02006AF4(u16 species, s8 unused, int volume, int pan, enum HeapID heapID);

BOOL sub_0200602C(u16 seqID, int pan);

BOOL sub_0200602C(u16 seqID, int pan) {
    int result = PlaySE(seqID);
    sub_020061B4(seqID, 0xFFFF, pan);

    return result;
}
