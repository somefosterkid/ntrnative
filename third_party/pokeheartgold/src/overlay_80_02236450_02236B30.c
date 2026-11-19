#include "global.h"
#include "player_data.h"

struct PlayerProfile;
enum BattleTowerMode {
    BATTLE_TOWER_MODE_SINGLE = 0,
    BATTLE_TOWER_MODE_DOUBLE = 1,
    BATTLE_TOWER_MODE_MULTI = 2,
    BATTLE_TOWER_MODE_LINK_MULTI = 3,
    BATTLE_TOWER_MODE_WIFI = 4,
    BATTLE_TOWER_MODE_5 = 5,
    BATTLE_TOWER_MODE_6 = 6,
};
PlayerProfile *sub_02034818(int netId);

int ov80_02236B30(u8 challengeMode);

int ov80_02236B30(u8 challengeMode) {
    switch (challengeMode) {
    case BATTLE_TOWER_MODE_LINK_MULTI:
    case BATTLE_TOWER_MODE_6: {
        PlayerProfile *hostInfo = sub_02034818(0);

        if (hostInfo == 0L) {
            ((0) ? (void)0 : GF_AssertFail());
        }

        PlayerProfile *partnerInfo = sub_02034818(1);

        if (partnerInfo == 0L) {
            ((0) ? (void)0 : GF_AssertFail());
        }

        u8 hostGame = PlayerProfile_GetVersion(hostInfo);
        u8 partnerGame = PlayerProfile_GetVersion(partnerInfo);

        if (hostGame == 0 || partnerGame == 0) {
            return 0;
        }

        return 1;
    }
    }

    return 1;
}
