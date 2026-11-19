#include "global.h"
#include "ppw_lobby.h"

struct PPW_LobbyRecruitInfo;

void ov45_02230384(const PPW_LobbyRecruitInfo *lobbyRecruitInfo, s64 *param1);

void ov45_02230384(const PPW_LobbyRecruitInfo *lobbyRecruitInfo, s64 *param1) {
    const s64 *v0 = (const s64 *)lobbyRecruitInfo->paramBuf;
    *param1 = *v0;
}
