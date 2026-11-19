#include "global.h"

#include "friend_group.h"
#include "player_data.h"
#include "save_link_ruleset.h"
#include "save_wifi_history.h"
#include "unk_0202C034.h"
#include "unk_02034354.h"
#include "unk_02034354_internal.h"
#include "unk_02035900.h"

void sub_02034434(void) {
    SAV_FRIEND_GRP *savFriendGrp;
    const LinkBattleRuleset *src;
    PlayerProfile *playerProfile;
    SaveWiFiHistory *wifiHistory;
    WiFiList *a0;
    u16 netId;

    netId = sub_0203769C();
    savFriendGrp = Save_FriendGroup_Get(_021D4130->saveData);
    a0 = sub_0202C6F4(_021D4130->saveData);
    wifiHistory = Save_WiFiHistory_Get(_021D4130->saveData);
    playerProfile = _021D4130->personalTrainerInfo;
    if (playerProfile == NULL) {
        playerProfile = Save_PlayerData_GetProfile(_021D4130->saveData);
    }
    PlayerProfile_Copy(playerProfile, _021D4130->trainerInfo[netId]);
    OS_GetMacAddress(_021D4130->playerInfo[netId].macAddress);
    MI_CpuCopy8(sub_0202C7E0(savFriendGrp, 1, 0), _021D4130->playerInfo[netId].unk_4C, 0x10U);
    _021D4130->playerInfo[netId].country = WifiHistory_GetPlayerCountry(wifiHistory);
    _021D4130->playerInfo[netId].region = WiFiHistory_GetPlayerRegion(wifiHistory);
    _021D4130->playerInfo[netId].hasGiftPenalty = 0;
    DWC_CreateExchangeToken(sub_0202C08C(a0), &_021D4130->playerInfo[netId].friendData);
    MI_CpuFill8(&_021D4130->playerInfo[netId].regulationBuffer, 0U, sizeof(CommRegulationBuffer));
    src = _021D4130->regulation;
    if (src != NULL) {
        LinkBattleRuleset_Copy(src, &_021D4130->playerInfo[netId].regulationBuffer.ruleset);
    }
    sub_02037030(3, &_021D4130->playerInfo[netId], sizeof(CommPlayerInfo));
}
