#include "global.h"

#include "pokemon.h"

s32 ov112_021EE8BC(BoxPokemon *arg0, Pokemon *arg1, s32 arg2);

s32 ov112_021EE8BC(BoxPokemon *boxMon, Pokemon *mon, s32 arg2) {
    u32 ptr;
    s32 v0;
    s32 v2;
    s32 v3;
    u32 v1;

    v3 = 0;
    v2 = arg2;
    if (GetBoxMonData(boxMon, 0xA1, NULL) < 0x64U) {
        if (v2 < 0) {
            v2 = 0;
        }
        v0 = (s32)CalcBoxMonExpToNextLevel(boxMon);
        if (v2 >= v0) {
            v2 = v0;
            v3 = 1;
        }
        v1 = GetBoxMonData(boxMon, 8, NULL);
        ptr = v1;
        ptr = v1 + v2;
        CopyBoxPokemonToPokemon(boxMon, mon);
        SetMonData(mon, 8, &ptr);
        CalcMonLevelAndStats(mon);
        if (v3 != 0) {
            MonApplyFriendshipMod(mon, 0U, 8U);
        }
    }
    return v3;
}
