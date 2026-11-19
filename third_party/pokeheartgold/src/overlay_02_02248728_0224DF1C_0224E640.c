#include "global.h"

#include "player_data.h"
#include "safari_zone.h"

s32 ov02_0224E640(SaveData *arg0);

s32 ov02_0224E640(SaveData *saveData) {
    s32 v4;
    u32 v1;
    u32 v3;
    u8 v0;
    u8 v2;

    v3 = PlayerProfile_GetTrainerID(Save_PlayerData_GetProfile(saveData));
    v2 = SafariZone_GetObjectUnlockLevel(Save_SafariZone_Get(saveData));
    v0 = (u8)(v3 % 10U);
    if ((u32)v0 < 6U) {
        v4 = (s32)v0 / 3;
    } else {
        v1 = v0 - 6;
        v4 = ((s32)(v1 + (v1 >> 0x1F)) >> 1) + 2;
    }
    if ((s32)v2 >= (s32)((u8)(3 - (u8)v4) + 1)) {
        return 1;
    }
    return 0;
}
