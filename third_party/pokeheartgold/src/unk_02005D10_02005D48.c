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

enum SoundHandleType GF_GetSndHandleByPlayerNo(int playerID);

u8 GF_GetPlayerNoBySeq(u16 seqID);

void Sound_SetFieldBGMBankState(int state);

void sub_02005DA0(s32 param0, enum SoundHandleType param1);

BOOL Sound_Impl_PlayBGM(u16 seqID, u8 playerID, enum SoundHandleType handleType);

BOOL Sound_Impl_PlayFieldBGM(u16 seqID, u8 playerID, enum SoundHandleType handleType);

void sub_02006820(s32 species, enum SoundHandleType handleType, int volume);

BOOL sub_02006AF4(u16 species, s8 unused, int volume, int pan, enum HeapID heapID);

BOOL PlayBGM(u16 bgmID);

BOOL sub_02005DC4(s32 seqID, s32 playerID, enum SoundHandleType handleType);

BOOL sub_02005DF4(u32 seqID, u32 playerID, enum SoundHandleType handleType);

void sub_020059E0(u8 state);

void sub_02004920(u16 unk);

BOOL PlayBGM(u16 bgmID) {
    u8 player = GF_GetPlayerNoBySeq(bgmID);
    enum SoundHandleType handleType = GF_GetSndHandleByPlayerNo(player);
    BOOL result;

    if (player == 7) {
        result = sub_02005DC4(bgmID, player, handleType);
    } else if (player == 1) {
        result = sub_02005DF4(bgmID, player, handleType);
    } else {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    sub_020059E0(0);
    if (result == 0) {
        sub_02004920(bgmID);
    }

    sub_02005DA0(bgmID, handleType);
    return result;
}
