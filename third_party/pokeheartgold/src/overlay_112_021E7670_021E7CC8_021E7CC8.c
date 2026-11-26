#include "global.h"

#include "constants/heap.h"

#include "player_data.h"
#include "pm_string.h"
#include "pokewalker.h"
#include "save.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E5D68(u8 *);

void ov112_021E5EB8(void *, void *);

void ov112_021E73C8(u8 *, u32, u32);

void ov112_021E7CC8(Ov112PokewalkerApp *param_1);

void ov112_021E7CC8(Ov112PokewalkerApp *param_1) {
    PlayerProfile *profile;
    String *string;
    u32 uVar1;
    void *srcp;

    profile = Save_PlayerData_GetProfile(param_1->saveData);
    string = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_154);
    param_1->unk_1024 = 1;
    param_1->unk_102C = GAME_VERSION;
    param_1->unk_107F = param_1->unk_107F & ~1;
    param_1->unk_107E = 0;
    uVar1 = PlayerProfile_GetTrainerID(param_1->profile);
    param_1->unk_1030 = uVar1;
    CopyStringToU16Array(string, param_1->unk_106C, 8);
    param_1->unk_1028 = 1;
    param_1->unk_102E = GAME_VERSION;
    srcp = sub_02032728(param_1->pokewalker);
    MIi_CpuCopyFast(srcp, param_1->unk_1034, 0x28);
    ov112_021E73C8(param_1->unk_24, 0x1000, 0xf);
    ov112_021E5EB8(&param_1->unk_1024, param_1->unk_108C);
    ov112_021E5D68(param_1->unk_10834);
    String_Delete(string);
    return;
}
