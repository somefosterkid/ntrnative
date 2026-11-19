#include "global.h"

#include "pokemon.h"
#include "script_pokemon_util.h"

extern u16 _020FC3CA;

u32 NPCTrainerGetBattleIntroAndMusicParam(s32 arg0);

u32 NPCTrainerGetBattleIntroAndMusicParam(s32 arg0) {
    u32 v0;
    u16 *v1;

    v1 = &_020FC3CA;
    v0 = 0;
loop_1:
    if (arg0 == ((u32)(*v1 << 0x16) >> 0x16)) {
        return (u32)((&_020FC3CA)[v0] << 0x10) >> 0x1A;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 0x20U) {
        return 0x29U;
    }
    goto loop_1;
}

extern u16 _020FC3B4;

u32 WildPokemonGetBattleIntroAndMusicParam(Party *arg0, s32 arg1);

u32 WildPokemonGetBattleIntroAndMusicParam(Party *party, s32 arg1) {
    u32 v0;
    u32 v1;
    u16 *v2;
    v0 = GetMonData(GetFirstAliveMonInParty_CrashIfNone(party), 5, 0L);
    if (arg1 == 0x6D) {
        return 0x2AU;
    }
    v2 = &_020FC3B4;
    v1 = 0;
loop_3:
    if (v0 == (((u32)((*v2) << 0x16)) >> 0x16)) {
        return ((u32)((&_020FC3B4)[v1] << 0x10)) >> 0x1A;
    }

    v1 += 1;
    v2 += 1;
    if (v1 >= 0xBU) {
        return 0x2AU;
    }
    goto loop_3;
}
