#include "global.h"

#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "overlay_65_trade_room.h"

void ov65_0221FB4C(Window *window, String *str, int unused, u32 textSpeed, int xOrCenter, int yOffset);

void ov65_0221BFEC(TradeRoom *arg0);

void ov65_0221BFEC(TradeRoom *arg0) {
    PlayerProfile *profile;
    PlayerProfile *playerProfile;
    String *string;
    String *v0;
    String *v1;

    profile = sub_02034818((u32)sub_0203769C());
    playerProfile = sub_02034818(sub_0203769C() ^ 1);
    string = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_26);
    v0 = PlayerProfile_GetPlayerName_NewString(playerProfile, HEAP_ID_26);
    v1 = NewString_ReadMsgData(((MsgData *)arg0->msgLoader), 0x32);
    ov65_0221FB4C(&arg0->windows[0], string, 0xA, 0, 1, 1);
    ov65_0221FB4C(&arg0->windows[1], v0, 0xA, 0, 1, 1);
    ov65_0221FB4C(&arg0->windows[6], v1, 5, 0, 1, 1);
    String_Delete(v1);
    String_Delete(v0);
    String_Delete(string);
}
