#include "global.h"
#include "battle/battle_setup.h"

u32 NPCTrainerGetBattleIntroAndMusicParam(s32);

u32 WildPokemonGetBattleIntroAndMusicParam(void *, s32);

u32 BattleSetup_GetTransitionAndMusicParam(void *arg0);

u32 BattleSetup_GetTransitionAndMusicParam(void *arg0) {
    s32 v0;
    u32 v1;

    v0 = *(s32 *)arg0;
    if (1 & v0) {
        v1 = NPCTrainerGetBattleIntroAndMusicParam(((BattleSetup *)arg0)->trainer[1].data.trainerClass);
        if (0x80 & v0) {
            if (v1 != 0x27) {
                if (2 & v0) {
                    return 0x25U;
                }
                return 0x23U;
            }
            return v1;
        }
        if (((v1 < 0x1DU) || (v1 > 0x22U)) && (v1 != 0x2B) && (v1 != 0x2C)) {
            if (2 & v0) {
                return 0x25U;
            }
            if (4 & v0) {
                return 0x24U;
            }
            return v1;
        }
        return v1;
    }
    v1 = WildPokemonGetBattleIntroAndMusicParam(((void *)((BattleSetup *)arg0)->party[1]), *((s32 *)&((BattleSetup *)arg0)->mapNumber));
    if ((v1 >= 0x2AU) && (2 & v0)) {
        v1 = 0x26;
    }
    return v1;
}
