#include "global.h"

#include "comm_server_client.h"

BOOL sub_02034BF8(const u8 *param0, const u8 *param1, int param2);

void sub_02034C94(void);

void sub_02034C94(void) {
    WMBssDesc *v0 = &_021D4134.sCommServerClient->unk_54;

    if (!_021D4134.sCommServerClient->unk_D95_6) {
        return;
    }

    _021D4134.sCommServerClient->unk_D95_6 = 0;
    int v1;

    for (v1 = 0; v1 < 16; ++v1) {
        if (_021D4134.sCommServerClient->unk_D44[v1] == 0) {
            continue;
        }

        if (sub_02034BF8(_021D4134.sCommServerClient->unk_114[v1].bssid, v0->bssid, 6)) {
            _021D4134.sCommServerClient->unk_D44[v1] = (30 * 10);
            MI_CpuCopy8(v0, &_021D4134.sCommServerClient->unk_114[v1], sizeof(WMBssDesc));
            return;
        }
    }

    for (v1 = 0; v1 < 16; ++v1) {
        if (_021D4134.sCommServerClient->unk_D44[v1] == 0) {
            break;
        }
    }

    if (v1 >= 16) {
        return;
    }

    _021D4134.sCommServerClient->unk_D44[v1] = (30 * 10);
    MI_CpuCopy8(v0, &_021D4134.sCommServerClient->unk_114[v1], sizeof(WMBssDesc));
    _021D4134.sCommServerClient->unk_D74 = 1;
}
