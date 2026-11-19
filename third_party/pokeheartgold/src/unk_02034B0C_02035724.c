#include "global.h"

#include "comm_server_client.h"
#include "error_handling.h"
#include "pokemon_types_def.h"

u16 sub_02035724(u16 param0);

WMBssDesc *sub_02035754(int param0);
PlayerProfile *sub_02035784(void);
PlayerProfile *sub_02035798(int param0);
void sub_020357C4(u8 *param0, int param1);
BOOL sub_020357FC(void);
void sub_0203581C(void);

u16 sub_02035724(u16 param0) {
    u16 v0 = WM_GetDispersionBeaconPeriod();

    GF_ASSERT(param0 < 41);

    if (10 == param0) {
        return v0 / 4;
    }

    if (9 == param0 || 13 == param0) {
        return v0 / 4;
    }

    return v0;
}

WMBssDesc *sub_02035754(int param0) {
    if (_021D4134.sCommServerClient && (_021D4134.sCommServerClient->unk_D44[param0] != 0)) {
        return &_021D4134.sCommServerClient->unk_114[param0];
    }

    return NULL;
}

PlayerProfile *sub_02035784(void) {
    return _021D4134.sCommServerClient->unk_D78;
}

PlayerProfile *sub_02035798(int param0) {
    if (_021D4134.sCommServerClient->unk_D44[param0] == 0) {
        return NULL;
    }

    CommGameInfo *v1 = (CommGameInfo *)_021D4134.sCommServerClient->unk_114[param0].gameInfo.userGameInfo;
    PlayerProfile *v0 = &v1->unk_10;

    return v0;
}

void sub_020357C4(u8 *param0, int param1) {
    if (_021D4134.sCommServerClient) {
        GF_ASSERT(param1 < (7 + 1));
        MI_CpuCopy8(param0, _021D4134.sCommServerClient->unk_D14[param1], WM_SIZE_BSSID);
    }
}

BOOL sub_020357FC(void) {
    if (_021D4134.sCommServerClient) {
        return _021D4134.sCommServerClient->unk_D95_4;
    }

    return 0;
}

void sub_0203581C(void) {
    if (_021D4134.sCommServerClient) {
        _021D4134.sCommServerClient->unk_D95_4 = 1;
    }
}
