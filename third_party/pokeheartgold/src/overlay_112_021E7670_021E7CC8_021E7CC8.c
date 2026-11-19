#include "global.h"

#include "constants/heap.h"

#include "player_data.h"
#include "pm_string.h"
#include "pokewalker.h"
#include "save.h"

void ov112_021E5D68(u8 *);

void ov112_021E5EB8(u8 *, u8 *);

void ov112_021E73C8(u8 *, u32, u32);

void ov112_021E7CC8(u8 *param_1);

void ov112_021E7CC8(u8 *param_1) {
    PlayerProfile *profile;
    String *string;
    u32 uVar1;
    void *srcp;

    profile = Save_PlayerData_GetProfile(*(SaveData **)(param_1 + 0x20));
    string = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_154);
    *(u32 *)(param_1 + 0x1024) = 1;
    *(u16 *)(param_1 + 0x102c) = GAME_VERSION;
    *(u8 *)(param_1 + 0x107f) = *(u8 *)(param_1 + 0x107f) & ~1;
    *(u8 *)(param_1 + 0x107e) = 0;
    uVar1 = PlayerProfile_GetTrainerID(*(PlayerProfile **)(param_1 + 0x1e438));
    *(u32 *)(param_1 + 0x1030) = uVar1;
    CopyStringToU16Array(string, (u16 *)(param_1 + 0x106c), 8);
    *(u32 *)(param_1 + 0x1028) = 1;
    *(u16 *)(param_1 + 0x102e) = GAME_VERSION;
    srcp = sub_02032728(*(POKEWALKER **)(param_1 + 0x1e440));
    MIi_CpuCopyFast(srcp, (void *)(param_1 + 0x1034), 0x28);
    ov112_021E73C8(param_1 + 0x24, 0x1000, 0xf);
    ov112_021E5EB8(param_1 + 0x1024, param_1 + 0x108c);
    ov112_021E5D68(param_1 + 0x10834);
    String_Delete(string);
    return;
}
