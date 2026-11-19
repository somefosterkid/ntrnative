#include "global.h"

#include "mail_message.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "string_util.h"
#include "unk_0202D230.h"

void ov112_021F336C(u32 *param_1, SaveData *param_2);

void ov112_021F336C(u32 *param_1, SaveData *param_2) {
    u8 uVar1;
    PlayerProfile *profile;
    u32 uVar2;
    u16 *src;
    MailMessage *src_00;
    MailMessage *dst;

    profile = Save_PlayerData_GetProfile(param_2);
    uVar2 = PlayerProfile_GetTrainerID(profile);
    *param_1 = uVar2;
    uVar2 = PlayerProfile_GetTrainerGender(profile);
    *(s8 *)((u8 *)param_1 + 7) = (s8)uVar2;
    uVar1 = PlayerProfile_GetLanguage(profile);
    *(u8 *)((u8 *)param_1 + 5) = uVar1;
    uVar1 = PlayerProfile_GetVersion(profile);
    *(u8 *)((u8 *)param_1 + 6) = uVar1;
    uVar1 = PlayerProfile_GetAvatar(profile);
    *(u8 *)(param_1 + 1) = uVar1;
    src = (unsigned short *)(PlayerProfile_GetNamePtr(profile));
    CopyU16StringArrayN((u16 *)(param_1 + 2), src, 7);
    uVar2 = 0;
    dst = (MailMessage *)(param_1 + 6);
    do {
        sub_0202D660(param_2, uVar2);
        src_00 = sub_0202D660(param_2, uVar2);
        MailMsg_Copy(dst, src_00);
        uVar2 = uVar2 + 1;
        dst = dst + 1;
    } while ((s32)uVar2 < 3);
    return;
}
