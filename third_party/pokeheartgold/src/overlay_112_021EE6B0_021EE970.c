#include "global.h"

#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"

void ov112_021EE970(BoxPokemon *arg0, s32 arg1);

void ov112_021EE970(BoxPokemon *boxMon, s32 arg1) {
    u8 ptr;
    u8 v0;

    if (arg1 >= 0x1388) {
        ptr = (u8)GetBoxMonData(boxMon, 9, NULL);
        v0 = ptr;
        if ((u32)v0 < 0xFFU) {
            ptr = v0 + 1;
            SetBoxMonData(boxMon, 9, &ptr);
        }
    }
}

s32 ov112_021EE9E4(Pokemon *, u32, void *, s32, s32);

void ov112_021EE9A4(Pokemon *arg0, PlayerProfile *arg1, void *arg2, s32 arg3, s32 arg4);

void ov112_021EE9A4(Pokemon *mon, PlayerProfile *profile, void *arg2, s32 arg3, s32 arg4) {
    s32 v0;
    String *string;

    v0 = arg3;
    string = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_154);
    ov112_021EE9E4(mon, PlayerProfile_GetTrainerID(profile), arg2, arg3, arg4);
    SetMonData(mon, 0x91, string);
    String_Delete(string);
}
