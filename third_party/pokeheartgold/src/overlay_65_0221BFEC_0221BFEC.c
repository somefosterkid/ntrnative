#include "global.h"

#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221FB4C(void *, String *, s32, s32, s32, s32);

void ov65_0221BFEC(void *arg0);

void ov65_0221BFEC(void *arg0) {
    PlayerProfile *profile;
    PlayerProfile *playerProfile;
    String *string;
    String *v0;
    String *v1;

    profile = sub_02034818((u32)sub_0203769C());
    playerProfile = sub_02034818(sub_0203769C() ^ 1);
    string = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_26);
    v0 = PlayerProfile_GetPlayerName_NewString(playerProfile, HEAP_ID_26);
    v1 = NewString_ReadMsgData(((MsgData *)((TradeRoom *)arg0)->msgLoader), 0x32);
    ov65_0221FB4C(arg0 + 0x444, string, 0xA, 0, 1, 1);
    ov65_0221FB4C(arg0 + 0x454, v0, 0xA, 0, 1, 1);
    ov65_0221FB4C(arg0 + 0x4A4, v1, 5, 0, 1, 1);
    String_Delete(v1);
    String_Delete(v0);
    String_Delete(string);
}
